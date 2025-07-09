#include <PID_Service.h>

QuickPID Soldering_PID(&soldering_temp_float, &Soldering_DutyCycle, &soldering_target_temp_float,
                        Soldering_KP, Soldering_KI, Soldering_KD,
                        QuickPID::pMode::pOnErrorMeas, 
                        QuickPID::dMode::dOnMeas, 
                        QuickPID::iAwMode::iAwClamp, 
                        QuickPID::Action::direct);

// 添加外部DigitRoller对象声明
extern DigitRoller* soldering_temp_display;
extern DigitRoller* heatgun_temp_display;

void Soldering_PID_Compute_Init()
{
    Soldering_PID.SetOutputLimits(0, SolderingMaxPower);
    Soldering_PID.SetSampleTimeUs(100000); // 100ms
    Soldering_PID.SetTunings(Soldering_KP, Soldering_KI, Soldering_KD);
    Soldering_PID.SetMode(QuickPID::Control::automatic);

    // 初始化防抖状态变量
    bool initial_sleep_signal = Soldering_Read_SLEEP();
    standby_state.prev_raw_sleep_signal = initial_sleep_signal;
    standby_state.debounced_sleep_signal = initial_sleep_signal;
    standby_state.last_processed_debounced_signal = initial_sleep_signal;
    standby_state.last_raw_signal_change_time = millis(); // 初始化时间戳
    
    standby_state.in_standby_mode = false; 
    standby_state.ui_color_changed = false;
}

void Soldering_PID_Update_Tunings(float Kp, float Ki, float Kd)
{
    Soldering_PID.SetTunings(Kp, Ki, Kd);
}

void Soldering_PID_Compute()
{
    // 早期退出：如果焊接被禁用
    if (!Soldering_Enabled) {
        pid_state = PID_POWER_OFF;
        Soldering_DutyCycle = 0.0f;
        Soldering_Set_PWM(0);
        soldering_state.was_disabled = true;
        
        // 重置待机状态和UI，并重置防抖状态
        standby_state.in_standby_mode = false;
        standby_state.prev_raw_sleep_signal = false; 
        standby_state.debounced_sleep_signal = false;
        standby_state.last_raw_signal_change_time = millis(); 
        standby_state.last_processed_debounced_signal = false; 

        if (standby_state.ui_color_changed) {
            // 恢复为白色 - 设置数字流转和文本标签颜色
            lv_obj_set_style_text_color(ui_TextSolderingTemp, lv_color_white(), LV_PART_MAIN);
            if (soldering_temp_display) {
                soldering_temp_display->setStyle(&ui_font_ASCII88, lv_color_white());
            }
            standby_state.ui_color_changed = false;
        }
        
        // 统一处理蜂鸣器停止
        if (buzzer_state.short_active || buzzer_state.long_active) {
            Buzzer_OFF();
            buzzer_state.short_active = buzzer_state.long_active = false;
        }
        
        return;
    }
    
    const unsigned long current_time = millis(); // 将 current_time 提前获取
    
    // --- Sleep Signal Debouncing ---
    const bool current_raw_sleep_signal = Soldering_Read_SLEEP();
    const unsigned long SLEEP_DEBOUNCE_DURATION_MS = 200; // 防抖持续时间 (可根据实际情况调整，例如 200-500ms)

    if (current_raw_sleep_signal != standby_state.prev_raw_sleep_signal) {
        // 如果当前原始信号与上一次不同，更新变化时间戳
        standby_state.last_raw_signal_change_time = current_time;
    }
    standby_state.prev_raw_sleep_signal = current_raw_sleep_signal;

    // 检查信号是否已稳定超过防抖持续时间
    if ((current_time - standby_state.last_raw_signal_change_time) > SLEEP_DEBOUNCE_DURATION_MS) {
        // 信号已稳定，可以更新防抖后的信号状态
        // 只有当防抖后的信号确实发生改变时才更新它
        if (standby_state.debounced_sleep_signal != current_raw_sleep_signal) {
            standby_state.debounced_sleep_signal = current_raw_sleep_signal;
        }
    }
    // 使用防抖处理后的 sleep_mode_active 进行后续判断
    const bool debounced_sleep_mode_active = standby_state.debounced_sleep_signal;
    
    // 检测待机状态变化 (基于防抖后的信号的边沿)
    if (debounced_sleep_mode_active != standby_state.last_processed_debounced_signal) {
        if (debounced_sleep_mode_active) {
            // 进入待机模式或直接进入休眠模式
            standby_state.standby_start_time = current_time; 
            standby_state.in_standby_mode = true; // 初始设置为true，如果直接休眠则后续可能调整
            
            if (SolderingStandbyTime == 0) {
                // 待机时间为0，直接进入休眠
                pid_state = PID_SLEEP;
                standby_state.in_standby_mode = false; // 明确不是在"等待超时"的待机状态
                standby_state.sleep_read_time = current_time;
                // 设置UI为黑色 - 文本标签和数字流转
                lv_obj_set_style_text_color(ui_TextSolderingTemp, lv_color_black(), LV_PART_MAIN);
                if (soldering_temp_display) {
                    soldering_temp_display->setStyle(&ui_font_ASCII88, lv_color_black());
                }
                // 触发进入休眠的蜂鸣器短鸣
                if (Buzzer_Enabled && !buzzer_state.short_active && !buzzer_state.long_active) {
                    Buzzer_ON();
                    buzzer_state.start_time = current_time;
                    buzzer_state.short_active = true;
                }
            } else {
                // 进入待机状态，继续正常循环但目标温度改为待机温度
                pid_state = PID_POWER_OFF;  // 重新开始循环以应用待机温度
                // 设置UI为蓝色 - 文本标签和数字流转
                lv_obj_set_style_text_color(ui_TextSolderingTemp, lv_color_hex(0x0080FF), LV_PART_MAIN);
                if (soldering_temp_display) {
                    soldering_temp_display->setStyle(&ui_font_ASCII88, lv_color_hex(0x0080FF));
                }
                // 触发进入待机的蜂鸣器短鸣
                if (Buzzer_Enabled && !buzzer_state.short_active && !buzzer_state.long_active) {
                    Buzzer_ON();
                    buzzer_state.start_time = current_time;
                    buzzer_state.short_active = true;
                }
            }
            standby_state.ui_color_changed = true;
        } else {
            // 退出待机/休眠模式，回到正常加热流程
            standby_state.in_standby_mode = false;
            pid_state = PID_POWER_OFF;
            // 恢复UI颜色为白色 - 文本标签和数字流转
            if (standby_state.ui_color_changed) {
                lv_obj_set_style_text_color(ui_TextSolderingTemp, lv_color_white(), LV_PART_MAIN);
                if (soldering_temp_display) {
                    soldering_temp_display->setStyle(&ui_font_ASCII88, lv_color_white());
                }
                standby_state.ui_color_changed = false;
            }
            // 重置温度到达提示
            buzzer_state.temperature_reached_played = false;
            // 触发退出待机/休眠的蜂鸣器短鸣
            if (Buzzer_Enabled && !buzzer_state.short_active && !buzzer_state.long_active) {
                Buzzer_ON();
                buzzer_state.start_time = current_time;
                buzzer_state.short_active = true;
            }
        }
        standby_state.last_processed_debounced_signal = debounced_sleep_mode_active; // 更新上次处理的防抖信号状态
    }
    
    // 在待机模式下检查是否需要转入休眠（注意：不管温度是否到达待机温度，计时器都不会被清空）
    if (standby_state.in_standby_mode && SolderingStandbyTime > 0) {
        const unsigned long standby_elapsed = current_time - standby_state.standby_start_time;
        const unsigned long standby_timeout = (unsigned long)SolderingStandbyTime * 60000UL; // 分钟转毫秒
        
        if (standby_elapsed >= standby_timeout) {
            pid_state = PID_SLEEP;
            standby_state.in_standby_mode = false; // 已进入休眠，不再是待机
            standby_state.sleep_read_time = current_time; // 初始化休眠读取时间
            // 设置UI为黑色 - 文本标签和数字流转
            lv_obj_set_style_text_color(ui_TextSolderingTemp, lv_color_black(), LV_PART_MAIN);
            if (soldering_temp_display) {
                soldering_temp_display->setStyle(&ui_font_ASCII88, lv_color_black());
            }
            standby_state.ui_color_changed = true; // 确保颜色已设置为休眠状态的颜色
            // 触发进入休眠的蜂鸣器短鸣
            if (Buzzer_Enabled && !buzzer_state.short_active && !buzzer_state.long_active) {
                Buzzer_ON();
                buzzer_state.start_time = current_time;
                buzzer_state.short_active = true;
            }
        }
    }
    
    // 检查状态变化（启用或目标温度改变）- 但不在待机模式下重置计时器
    if (soldering_state.was_disabled || soldering_state.last_target_temp != SolderingTargetTemp) {
        buzzer_state.temperature_reached_played = false;
        soldering_state.last_target_temp = SolderingTargetTemp;
        soldering_state.was_disabled = false;
    }
    
    // 蜂鸣器管理 - 优化分支预测
    if (buzzer_state.short_active || buzzer_state.long_active) {
        if (!Buzzer_Enabled) {
            // 蜂鸣器被禁用，立即停止
            Buzzer_OFF();
            buzzer_state.short_active = buzzer_state.long_active = false;
        } else {
            // 检查蜂鸣器超时（短鸣叫优先级更高，时间更短）
            const unsigned long elapsed = current_time - buzzer_state.start_time;
            if (buzzer_state.short_active && elapsed >= 50) {
                Buzzer_OFF();
                buzzer_state.short_active = false;
            } else if (buzzer_state.long_active && elapsed >= 1000) {
                Buzzer_OFF();
                buzzer_state.long_active = false;
            }
        }
    }
    
    // 主状态机 - 待机模式复用正常循环
    switch (pid_state) {
        case PID_POWER_OFF:
            Soldering_Set_PWM(0);
            state_start_time = current_time;
            pid_state = PID_WAIT_STABLE;
            break;
            
        case PID_WAIT_STABLE:
            // 内联时间检查以减少函数调用开销
            if (current_time - state_start_time >= 200) {
                pid_state = PID_READ_TEMP;
            }
            break;
            
        case PID_READ_TEMP: {
            // 使用块作用域减少变量生命周期
            const uint8_t sensor_status = MAX6675_Read_Soldering_Status();
            
            if (sensor_status != 0) {
                // 传感器异常 - 立即处理
                Soldering_DutyCycle = 0.0f;
                Soldering_Set_PWM(0);
                
                // 触发应急蜂鸣器
                if (Buzzer_Enabled && !buzzer_state.long_active && !buzzer_state.short_active) {
                    Buzzer_ON();
                    buzzer_state.start_time = current_time;
                    buzzer_state.long_active = true;
                }

                pid_state = PID_POWER_OFF;
                break;
            }
            
            // 传感器正常，读取温度
            const int current_temp = (int)MAX6675_Read_Soldering_Temperature();
            Soldering_Temp = current_temp;
            
            // 安全检查 - 使用常量避免重复计算
            const int safety_threshold = SolderingTargetTempMax + 15;
            if (current_temp > safety_threshold) {
                Soldering_DutyCycle = 0.0f;
                Soldering_Set_PWM(0);
                
                // 触发应急蜂鸣器
                if (Buzzer_Enabled && !buzzer_state.long_active && !buzzer_state.short_active) {
                    Buzzer_ON();
                    buzzer_state.start_time = current_time;
                    buzzer_state.long_active = true;
                }

                pid_state = PID_POWER_OFF;
                break;
            }
            
            // 根据当前模式选择目标温度
            const uint16_t target_temp = standby_state.in_standby_mode ? SolderingStandbyTemp : SolderingTargetTemp;
            
            // 温度到达检查 - 只在正常模式下检查（待机模式不需要提示音）
            if (!standby_state.in_standby_mode && 
                !buzzer_state.temperature_reached_played && 
                !buzzer_state.short_active && !buzzer_state.long_active &&
                abs(current_temp - (int)target_temp) <= 3) {
                
                if (Buzzer_Enabled) {
                    Buzzer_ON();
                    buzzer_state.start_time = current_time;
                    buzzer_state.short_active = true;
                }
                buzzer_state.temperature_reached_played = true;
            }
            
            // PID计算 - 减少类型转换
            soldering_temp_float = (float)current_temp;
            soldering_target_temp_float = (float)target_temp;
            
            Soldering_PID.Compute();
            
            // 功率控制决策
            if (Soldering_DutyCycle > 0.0f) {
                Soldering_Set_PWM(Soldering_DutyCycle);
                state_start_time = current_time;
                pid_state = PID_HEATING;
            } else {
                Soldering_Set_PWM(0);
                pid_state = PID_POWER_OFF;
            }
            break;
        }
        
        case PID_HEATING:
            // 内联时间检查
            if (current_time - state_start_time >= 100) //加热状态持续100ms
            {
                pid_state = PID_POWER_OFF;
            }
            break;
            
        case PID_SLEEP:
            // 休眠状态 - 关闭输出，每200ms读取一次温度用于显示
            Soldering_Set_PWM(0);
            Soldering_DutyCycle = 0.0f;
            
            // 每200ms读取一次温度
            if (current_time - standby_state.sleep_read_time >= 200) {
                if (MAX6675_Read_Soldering_Status() == 0) {
                    Soldering_Temp = (int)MAX6675_Read_Soldering_Temperature();
                }
                standby_state.sleep_read_time = current_time;
            }
            break;
    }
}

//------ 热风枪 PID计算 ------//

QuickPID Heatgun_PID(&heatgun_temp_float, &Heatgun_DutyCycle, &heatgun_target_temp_float,
                         Heatgun_KP, Heatgun_KI, Heatgun_KD,
                         QuickPID::pMode::pOnErrorMeas, 
                         QuickPID::dMode::dOnMeas, 
                         QuickPID::iAwMode::iAwClamp, 
                         QuickPID::Action::direct);

void Heatgun_PID_Compute_Init()
{
    Heatgun_PID.SetOutputLimits(0, 100);
    Heatgun_PID.SetSampleTimeUs(200000);
    Heatgun_PID.SetTunings(Heatgun_KP, Heatgun_KI, Heatgun_KD);
    Heatgun_PID.SetMode(QuickPID::Control::automatic);

    bool is_initially_sleeping_low = (Heatgun_Read_SLEEP() == LOW);
    heatgun_sleep_state.prev_raw_sleep_signal = is_initially_sleeping_low;
    heatgun_sleep_state.debounced_sleep_signal_is_low = is_initially_sleeping_low;
    heatgun_sleep_state.last_processed_debounced_signal_is_low = is_initially_sleeping_low;
    heatgun_sleep_state.last_raw_signal_change_time = millis();
    heatgun_sleep_state.in_sleep_mode_active = is_initially_sleeping_low;
    heatgun_sleep_state.ui_color_changed = false;

    // 初始化蜂鸣器状态
    heatgun_buzzer_state.short_active = false;
    heatgun_buzzer_state.long_active = false;
    heatgun_buzzer_state.temperature_reached_played = false;

    // 初始化内部状态
    heatgun_internal_state.was_disabled = true; // 假设初始为禁用状态，首次启用时会重置蜂鸣器
    heatgun_internal_state.last_target_temp = HeatgunTargetTemp;


    if (is_initially_sleeping_low && Heatgun_Enabled) {
        heatgun_pid_state = HEATGUN_PID_SLEEP_COOLING;
    } else {
        heatgun_pid_state = HEATGUN_PID_OFF;
    }
}

void HeatgunPID_Update_Tunings(float Kp, float Ki, float Kd)
{
     Heatgun_PID.SetTunings(Kp, Ki, Kd);
}

void Heatgun_PID_Compute()
{
    const unsigned long current_time = millis();

    // --- 如果热风枪被禁用，则提前退出 ---
    if (!Heatgun_Enabled) {
        Heatgun_Set_PWM(0);
        Heatgun_Set_FAN_PWM(0); 
        Heatgun_DutyCycle = 0.0f;
        heatgun_pid_state = HEATGUN_PID_OFF;

        if (heatgun_sleep_state.ui_color_changed) {
            // 恢复为白色 - 文本标签和数字流转
            lv_obj_set_style_text_color(ui_TextHeatgunTemp, lv_color_white(), LV_PART_MAIN);
            if (heatgun_temp_display) {
                heatgun_temp_display->setStyle(&ui_font_ASCII88, lv_color_white());
            }
            heatgun_sleep_state.ui_color_changed = false;
        }
        heatgun_sleep_state.in_sleep_mode_active = false; 
        
        if (heatgun_buzzer_state.short_active || heatgun_buzzer_state.long_active) {
            Buzzer_OFF();
            heatgun_buzzer_state.short_active = false;
            heatgun_buzzer_state.long_active = false;
        }
        heatgun_buzzer_state.temperature_reached_played = false; 
        heatgun_internal_state.was_disabled = true; 
        return;
    }

    // --- 获取最新的消抖后的休眠信号 ---
    const bool current_raw_sleep_is_low = (Heatgun_Read_SLEEP() == LOW);
    if (current_raw_sleep_is_low != heatgun_sleep_state.prev_raw_sleep_signal) {
        heatgun_sleep_state.last_raw_signal_change_time = current_time;
    }
    heatgun_sleep_state.prev_raw_sleep_signal = current_raw_sleep_is_low;

    if ((current_time - heatgun_sleep_state.last_raw_signal_change_time) > HEATGUN_SLEEP_DEBOUNCE_DURATION_MS) {
        if (heatgun_sleep_state.debounced_sleep_signal_is_low != current_raw_sleep_is_low) {
            heatgun_sleep_state.debounced_sleep_signal_is_low = current_raw_sleep_is_low;
        }
    }
    const bool debounced_sleep_is_low = heatgun_sleep_state.debounced_sleep_signal_is_low;

    // --- 检查状态变化（启用或目标温度改变）并处理刚启用时的休眠检测 ---
    if (heatgun_internal_state.was_disabled) { 
        heatgun_buzzer_state.temperature_reached_played = false;
        heatgun_internal_state.last_target_temp = HeatgunTargetTemp;

        if (debounced_sleep_is_low) {
            if (heatgun_sleep_state.last_processed_debounced_signal_is_low == debounced_sleep_is_low) {
                 heatgun_sleep_state.last_processed_debounced_signal_is_low = !debounced_sleep_is_low;
            }
        }
        heatgun_internal_state.was_disabled = false; 
    } else if (heatgun_internal_state.last_target_temp != HeatgunTargetTemp) { 
        heatgun_buzzer_state.temperature_reached_played = false;
        heatgun_internal_state.last_target_temp = HeatgunTargetTemp;
    }

    // --- 热风枪蜂鸣器管理 (通用超时) ---
    if (heatgun_buzzer_state.short_active || heatgun_buzzer_state.long_active) {
        if (!Buzzer_Enabled) { 
            Buzzer_OFF();
            heatgun_buzzer_state.short_active = false;
            heatgun_buzzer_state.long_active = false;
        } else {
            const unsigned long elapsed = current_time - heatgun_buzzer_state.start_time;
            if (heatgun_buzzer_state.short_active && elapsed >= 50) { 
                Buzzer_OFF();
                heatgun_buzzer_state.short_active = false;
            } else if (heatgun_buzzer_state.long_active && elapsed >= 1000) { 
                Buzzer_OFF();
                heatgun_buzzer_state.long_active = false;
            }
        }
    }
    
    // --- 休眠状态转换 (基于消抖后信号的边沿检测) ---
    if (debounced_sleep_is_low != heatgun_sleep_state.last_processed_debounced_signal_is_low) {
        if (debounced_sleep_is_low) { 
            heatgun_sleep_state.in_sleep_mode_active = true;
            heatgun_pid_state = HEATGUN_PID_SLEEP_COOLING; 
            Heatgun_Set_PWM(0);
            Heatgun_DutyCycle = 0.0f;
            // 风扇将在 SLEEP_COOLING 状态中根据温度设置

            // 设置为蓝色 - 文本标签和数字流转
            lv_obj_set_style_text_color(ui_TextHeatgunTemp, lv_color_hex(0x0080FF), LV_PART_MAIN); 
            if (heatgun_temp_display) {
                heatgun_temp_display->setStyle(&ui_font_ASCII88, lv_color_hex(0x0080FF));
            }
            heatgun_sleep_state.ui_color_changed = true;

            // 进入休眠冷却蜂鸣器短鸣
            if (Buzzer_Enabled && !heatgun_buzzer_state.short_active && !heatgun_buzzer_state.long_active) {
                Buzzer_ON();
                heatgun_buzzer_state.start_time = current_time;
                heatgun_buzzer_state.short_active = true;
            }
        } else { 
            heatgun_sleep_state.in_sleep_mode_active = false;
            heatgun_pid_state = HEATGUN_PID_OFF; 

            if (heatgun_sleep_state.ui_color_changed) { 
                // 恢复为白色 - 文本标签和数字流转
                lv_obj_set_style_text_color(ui_TextHeatgunTemp, lv_color_white(), LV_PART_MAIN);
                if (heatgun_temp_display) {
                    heatgun_temp_display->setStyle(&ui_font_ASCII88, lv_color_white());
                }
                heatgun_sleep_state.ui_color_changed = false;
            }
            // 确保所有蜂鸣器停止并重置温度到达标志
            if (Buzzer_Enabled && (heatgun_buzzer_state.short_active || heatgun_buzzer_state.long_active)) {
                 Buzzer_OFF();
                 heatgun_buzzer_state.short_active = false;
                 heatgun_buzzer_state.long_active = false;
            }
            // 退出休眠蜂鸣器短鸣
            if (Buzzer_Enabled && !heatgun_buzzer_state.short_active && !heatgun_buzzer_state.long_active) {
                Buzzer_ON();
                heatgun_buzzer_state.start_time = current_time;
                heatgun_buzzer_state.short_active = true;
            }
            heatgun_buzzer_state.temperature_reached_played = false; 
        }
        heatgun_sleep_state.last_processed_debounced_signal_is_low = debounced_sleep_is_low; 
    }

    // --- 热风枪主状态机 ---
    switch (heatgun_pid_state) {
        case HEATGUN_PID_OFF:
            Heatgun_Set_PWM(0);
            Heatgun_Set_FAN_PWM(0); 
            Heatgun_DutyCycle = 0.0f;
            if (Heatgun_Enabled && !debounced_sleep_is_low && !heatgun_sleep_state.in_sleep_mode_active) {
                heatgun_pid_state = HEATGUN_PID_HEATING;
                if (heatgun_sleep_state.ui_color_changed) { 
                     // 恢复为白色 - 文本标签和数字流转
                     lv_obj_set_style_text_color(ui_TextHeatgunTemp, lv_color_white(), LV_PART_MAIN);
                     if (heatgun_temp_display) {
                         heatgun_temp_display->setStyle(&ui_font_ASCII88, lv_color_white());
                     }
                     heatgun_sleep_state.ui_color_changed = false;
                }
            }
            break;

        case HEATGUN_PID_HEATING:
            if (heatgun_sleep_state.in_sleep_mode_active) { // 被休眠信号中断
                heatgun_pid_state = HEATGUN_PID_SLEEP_COOLING; 
                // UI颜色和初始风扇已在边沿检测中处理或将在SLEEP_COOLING中处理
                break; 
            }

            if (Heatgun_Status != 0) { 
                Heatgun_Set_PWM(0);
                Heatgun_DutyCycle = 0.0f;
                Heatgun_Set_FAN_PWM(0); 
                
                if (Buzzer_Enabled && !heatgun_buzzer_state.long_active && !heatgun_buzzer_state.short_active) {
                    Buzzer_ON();
                    heatgun_buzzer_state.start_time = current_time;
                    heatgun_buzzer_state.long_active = true;
                }
                heatgun_pid_state = HEATGUN_PID_OFF;
                break;
            }

            if (Heatgun_Temp > (HeatgunTargetTempMax + HEATGUN_SAFETY_MAX_TEMP_OFFSET)) { 
                Heatgun_Set_PWM(0);
                Heatgun_DutyCycle = 0.0f;
                Heatgun_Set_FAN_PWM(100.0f); 

                if (Buzzer_Enabled && !heatgun_buzzer_state.long_active && !heatgun_buzzer_state.short_active) {
                    Buzzer_ON();
                    heatgun_buzzer_state.start_time = current_time;
                    heatgun_buzzer_state.long_active = true;
                }
                heatgun_pid_state = HEATGUN_PID_OFF; 
                break;
            }

            if (!heatgun_sleep_state.in_sleep_mode_active &&
                !heatgun_buzzer_state.temperature_reached_played &&
                !heatgun_buzzer_state.short_active && !heatgun_buzzer_state.long_active &&
                abs(Heatgun_Temp - (int)HeatgunTargetTemp) <= HEATGUN_TEMP_REACHED_THRESHOLD) {
                
                if (Buzzer_Enabled) {
                    Buzzer_ON();
                    heatgun_buzzer_state.start_time = current_time;
                    heatgun_buzzer_state.short_active = true;
                }
                heatgun_buzzer_state.temperature_reached_played = true;
            }
            
            heatgun_temp_float = (float)Heatgun_Temp; 
            heatgun_target_temp_float = (float)HeatgunTargetTemp;
            Heatgun_PID.Compute(); 

            Heatgun_Set_PWM(Heatgun_DutyCycle);
            Heatgun_Set_FAN_PWM((float)HeatgunWindSpeed); 
            break;

        case HEATGUN_PID_SLEEP_COOLING: 
            if (!heatgun_sleep_state.in_sleep_mode_active) { // 如果在冷却时休眠信号取消
                heatgun_pid_state = HEATGUN_PID_OFF; 
                // 颜色和蜂鸣器已由边沿检测处理
                break;
            }

            Heatgun_Set_PWM(0); 
            Heatgun_DutyCycle = 0.0f;

            if (Heatgun_Temp > HEATGUN_SLEEP_COOL_DOWN_TEMP_STAGE1) { // > 150C
                Heatgun_Set_FAN_PWM(100.0f);
            } else if (Heatgun_Temp > HEATGUN_SLEEP_COOL_DOWN_TEMP_STAGE2) { // (100C, 150C]
                Heatgun_Set_FAN_PWM(50.0f);
            } else { // Temp <= 100C
                // 转换到 SLEEP_IDLE 状态，改变UI颜色为黑色
                lv_obj_set_style_text_color(ui_TextHeatgunTemp, lv_color_black(), LV_PART_MAIN); 
                if (heatgun_temp_display) {
                    heatgun_temp_display->setStyle(&ui_font_ASCII88, lv_color_black());
                }
                // 根据 SLEEP_IDLE 的逻辑立即设置风扇
                if (Heatgun_Temp > HEATGUN_SLEEP_COOL_DOWN_TEMP_STAGE3) { // (60C, 100C]
                    Heatgun_Set_FAN_PWM(30.0f);
                } else { // Temp <= 60C
                    heatgun_pid_state = HEATGUN_PID_SLEEP_IDLE;
                    Heatgun_Set_FAN_PWM(0.0f);
                }
            }
            break;

        case HEATGUN_PID_SLEEP_IDLE: 
             if (!heatgun_sleep_state.in_sleep_mode_active) { // 如果在空闲时休眠信号取消
                heatgun_pid_state = HEATGUN_PID_OFF; 
                // 颜色和蜂鸣器已由边沿检测处理
                break;
            }
            Heatgun_Set_PWM(0); 
            Heatgun_DutyCycle = 0.0f;
            break;
    }
}