#include <PID_Service.h>

QuickPID Soldering_PID(&soldering_temp_float, &Soldering_DutyCycle, &soldering_target_temp_float,
                        Soldering_KP, Soldering_KI, Soldering_KD,
                        QuickPID::pMode::pOnErrorMeas, 
                        QuickPID::dMode::dOnMeas, 
                        QuickPID::iAwMode::iAwClamp, 
                        QuickPID::Action::direct);

// 添加全局变量用于温度读取时序管理
static unsigned long heatgun_last_temp_read_time = 0;
static bool soldering_fault_shutdown_pending = false;
static bool heatgun_fault_shutdown_pending = false; // 添加热风枪故障关闭等待状态
static float soldering_smoothed_duty = 0.0f;
static int soldering_prev_temp = 0;
static unsigned long soldering_prev_temp_time = 0;

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// 斜率限制：限制单次控制周期内占空比变化量，避免采样稀疏导致的大跳变
static float rate_limit_step(float current, float target, float step_up, float step_down)
{
    if (target > current) {
        const float next = current + step_up;
        return (next < target) ? next : target;
    }
    const float next = current - step_down;
    return (next > target) ? next : target;
}

// 自适应平滑：误差大时响应更快，误差小时更平滑
static float adaptive_smooth(float current, float target, int temp_error)
{
    float alpha = DutySmoothAlphaBase;
    if (temp_error >= (int)DutySmoothErrHigh) alpha = DutySmoothAlphaHigh;
    else if (temp_error >= (int)DutySmoothErrMid) alpha = DutySmoothAlphaMid;
    else if (temp_error >= (int)DutySmoothErrLow) alpha = DutySmoothAlphaLow;

    return current + alpha * (target - current);
}

volatile uint8_t g_soldering_ui_color_req = PID_UI_TEXT_WHITE;
volatile uint8_t g_heatgun_ui_color_req = PID_UI_TEXT_WHITE;
volatile uint32_t g_soldering_switch_off_req_seq = 0;
volatile uint32_t g_heatgun_switch_off_req_seq = 0;

static lv_color_t ui_color_from_req(uint8_t req)
{
    switch (req) {
        case PID_UI_TEXT_BLUE:
            return lv_color_hex(0x0080FF);
        case PID_UI_TEXT_BLACK:
            return lv_color_black();
        case PID_UI_TEXT_WHITE:
        default:
            return lv_color_white();
    }
}

static inline void request_soldering_ui_color(uint8_t color)
{
    g_soldering_ui_color_req = color;
}

static inline void request_heatgun_ui_color(uint8_t color)
{
    g_heatgun_ui_color_req = color;
}

void PID_UI_Sync_Task()
{
    static uint8_t last_soldering_color = 0xFF;
    static uint8_t last_heatgun_color = 0xFF;
    static uint32_t handled_soldering_switch_off_seq = 0;
    static uint32_t handled_heatgun_switch_off_seq = 0;

    const uint8_t soldering_color = g_soldering_ui_color_req;
    if (soldering_color != last_soldering_color) {
        const lv_color_t c = ui_color_from_req(soldering_color);
        lv_obj_set_style_text_color(ui_TextSolderingTemp, c, LV_PART_MAIN);
        lv_obj_set_style_text_color(ui_SolderingTemp, c, LV_PART_MAIN);
        last_soldering_color = soldering_color;
    }

    const uint8_t heatgun_color = g_heatgun_ui_color_req;
    if (heatgun_color != last_heatgun_color) {
        const lv_color_t c = ui_color_from_req(heatgun_color);
        lv_obj_set_style_text_color(ui_TextHeatgunTemp, c, LV_PART_MAIN);
        lv_obj_set_style_text_color(ui_HeatgunTemp, c, LV_PART_MAIN);
        last_heatgun_color = heatgun_color;
    }

    const uint32_t soldering_seq = g_soldering_switch_off_req_seq;
    if (soldering_seq != handled_soldering_switch_off_seq) {
        lv_obj_clear_state(ui_SolderingSwitch, LV_STATE_CHECKED);
        handled_soldering_switch_off_seq = soldering_seq;
    }

    const uint32_t heatgun_seq = g_heatgun_switch_off_req_seq;
    if (heatgun_seq != handled_heatgun_switch_off_seq) {
        lv_obj_clear_state(ui_HeatgunSwitch, LV_STATE_CHECKED);
        handled_heatgun_switch_off_seq = heatgun_seq;
    }
}

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
    request_soldering_ui_color(PID_UI_TEXT_WHITE);
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
        soldering_smoothed_duty = 0.0f;
        Soldering_Set_PWM(0);
        soldering_state.was_disabled = true;
        
        // 重置待机状态和UI，并重置防抖状态
        standby_state.in_standby_mode = false;
        standby_state.prev_raw_sleep_signal = false; 
        standby_state.debounced_sleep_signal = false;
        standby_state.last_raw_signal_change_time = millis(); 
        standby_state.last_processed_debounced_signal = false; 

        if (standby_state.ui_color_changed) {
            // 恢复为白色（由 core0 UI 同步任务执行）
            request_soldering_ui_color(PID_UI_TEXT_WHITE);
            standby_state.ui_color_changed = false;
        }
        
        // 统一处理蜂鸣器停止
        if (buzzer_state.short_active || buzzer_state.long_active) {
            Buzzer_OFF();
            buzzer_state.short_active = buzzer_state.long_active = false;
        }
        soldering_fault_shutdown_pending = false; // 重置故障关闭状态
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

                // 设置UI为黑色（由 core0 UI 同步任务执行）
                request_soldering_ui_color(PID_UI_TEXT_BLACK);
                // 触发进入休眠的蜂鸣器短鸣
                if (Buzzer_Enabled && !buzzer_state.short_active && !buzzer_state.long_active) {
                    Buzzer_ON();
                    buzzer_state.start_time = current_time;
                    buzzer_state.short_active = true;
                }
            } else {
                // 进入待机状态，继续正常循环但目标温度改为待机温度
                pid_state = PID_POWER_OFF;  // 重新开始循环以应用待机温度
                // 设置UI为蓝色（由 core0 UI 同步任务执行）
                request_soldering_ui_color(PID_UI_TEXT_BLUE);
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
            // 恢复UI颜色为白色
            if (standby_state.ui_color_changed) {
                request_soldering_ui_color(PID_UI_TEXT_WHITE);
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

            // 设置UI为黑色（由 core0 UI 同步任务执行）
            request_soldering_ui_color(PID_UI_TEXT_BLACK);
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

        // 电烙铁从禁用切到启用时，短鸣一次
        if (soldering_state.was_disabled && Buzzer_Enabled &&
            !buzzer_state.short_active && !buzzer_state.long_active) {
            Buzzer_ON();
            buzzer_state.start_time = current_time;
            buzzer_state.short_active = true;
        }

        soldering_state.was_disabled = false;
    }
    
    // 蜂鸣器管理 - 优化分支预测
    if (buzzer_state.short_active || buzzer_state.long_active) {
        if (!Buzzer_Enabled) {
            Buzzer_OFF();
            buzzer_state.short_active = buzzer_state.long_active = false;
        } else {
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

    if (soldering_fault_shutdown_pending) {
        const bool buzzer_active = buzzer_state.short_active || buzzer_state.long_active;
        if (!Buzzer_Enabled || !buzzer_active) {
            Soldering_Set_PWM(0);
            Soldering_DutyCycle = 0.0f;
            soldering_smoothed_duty = 0.0f;
            Soldering_Enabled = false;
            g_soldering_switch_off_req_seq++;
            soldering_fault_shutdown_pending = false;
        }
        return;
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
            uint8_t sensor_status = 0;
            int current_temp = 0;
            
            // SPI 总线互斥保护温度读取
            if (MAX6675_TryLockBus()) {
                sensor_status = MAX6675_Read_Soldering_Status();
                
                if (sensor_status == 0) {
                    current_temp = (int)MAX6675_Read_Soldering_Temperature();
                    MAX6675_NotifySolderingPIDRead();
                }
                MAX6675_UnlockBus();
            } else {
                // 如果互斥锁被占用，使用上次读取的温度值
                sensor_status = 0; // 假设传感器正常
                current_temp = Soldering_Temp; // 使用上次的温度值
            }
            
            if (sensor_status != 0) {
                if (!soldering_fault_shutdown_pending) {
                    Soldering_DutyCycle = 0.0f;
                    Soldering_Set_PWM(0);
                    pid_state = PID_POWER_OFF;

                    if (Buzzer_Enabled && !buzzer_state.long_active && !buzzer_state.short_active) {
                        Buzzer_ON();
                        buzzer_state.start_time = current_time;
                        buzzer_state.long_active = true;
                    }
                    soldering_fault_shutdown_pending = true;
                }
                return;
            }
            
            // 传感器正常，更新温度
            Soldering_Temp = current_temp;
            
            // 安全检查 - 使用常量避免重复计算
            const int safety_threshold = SolderingTargetTempMax + 15;
            if (current_temp > safety_threshold) {
                if (!soldering_fault_shutdown_pending) {
                    Soldering_DutyCycle = 0.0f;
                    Soldering_Set_PWM(0);
                    pid_state = PID_POWER_OFF;

                    if (Buzzer_Enabled && !buzzer_state.long_active && !buzzer_state.short_active) {
                        Buzzer_ON();
                        buzzer_state.start_time = current_time;
                        buzzer_state.long_active = true;
                    }
                    soldering_fault_shutdown_pending = true;
                }
                return;
            }
            
            // 根据当前模式选择目标温度
            const uint16_t target_temp = standby_state.in_standby_mode ? SolderingStandbyTemp : SolderingTargetTemp;

            // 估算升温斜率（C/s），用于接近目标时预刹车
            float rise_rate_c_per_s = 0.0f;
            if (soldering_prev_temp_time > 0 && current_time > soldering_prev_temp_time) {
                const float dt_s = (float)(current_time - soldering_prev_temp_time) / 1000.0f;
                if (dt_s > 0.02f) {
                    rise_rate_c_per_s = (current_temp - soldering_prev_temp) / dt_s;
                }
            }
            soldering_prev_temp = current_temp;
            soldering_prev_temp_time = current_time;
            
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

            // 电烙铁联动控制：接近目标和超温时动态限制功率，抑制过冲
            const int signed_error = (int)target_temp - current_temp;
            const int temp_error = abs(signed_error);
            float heating_cap = (float)SolderingMaxPower;
            if (signed_error <= -8) {
                heating_cap = 0.0f;
            } else if (signed_error <= -4) {
                heating_cap = 10.0f;
            } else if (signed_error <= -1) {
                heating_cap = 20.0f;
            } else if (temp_error <= 8) {
                heating_cap = clampf(26.0f, 16.0f, (float)SolderingMaxPower);
            }

            // 接近目标且升温斜率较高时提前刹车，降低首次冲顶
            if (signed_error > 0 && signed_error <= 14) {
                if (rise_rate_c_per_s >= 12.0f) {
                    heating_cap = clampf(heating_cap, 0.0f, 12.0f);
                } else if (rise_rate_c_per_s >= 8.0f) {
                    heating_cap = clampf(heating_cap, 0.0f, 18.0f);
                } else if (rise_rate_c_per_s >= 5.0f) {
                    heating_cap = clampf(heating_cap, 0.0f, 22.0f);
                }
            }

            // 维持阶段若明显低于目标，给轻微功率托底，避免长期低温偏差
            if (signed_error >= 6 && rise_rate_c_per_s < 2.0f) {
                heating_cap = clampf(heating_cap, 24.0f, (float)SolderingMaxPower);
            }

            // 对 PID 输出做自适应平滑 + 斜率限制，减少占空比大跳变
            const float raw_duty = clampf(Soldering_DutyCycle, 0.0f, heating_cap);
            const float pre_smooth = adaptive_smooth(soldering_smoothed_duty, raw_duty, temp_error);
            const float up_step =
                (temp_error >= (int)DutySmoothErrHigh) ? SolderingDutyStepUpHighErr :
                (temp_error >= (int)DutySmoothErrMid) ? SolderingDutyStepUpMidErr :
                (temp_error >= (int)DutySmoothErrLow) ? SolderingDutyStepUpLowErr :
                SolderingDutyStepUpBase;
            float down_step =
                (temp_error >= (int)DutySmoothErrHigh) ? SolderingDutyStepDownHighErr :
                (temp_error >= (int)DutySmoothErrMid) ? SolderingDutyStepDownMidErr :
                (temp_error >= (int)DutySmoothErrLow) ? SolderingDutyStepDownLowErr :
                SolderingDutyStepDownBase;

            // 超温时放大下降斜率，尽快卸载功率
            if (signed_error <= -8) down_step += 30.0f;
            else if (signed_error <= -4) down_step += 20.0f;
            else if (signed_error <= -1) down_step += 6.0f;

            soldering_smoothed_duty = rate_limit_step(soldering_smoothed_duty, pre_smooth, up_step, down_step);
            soldering_smoothed_duty = clampf(soldering_smoothed_duty, 0.0f, (float)SolderingMaxPower);
            Soldering_DutyCycle = soldering_smoothed_duty;
            
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
        
        case PID_HEATING: {
            // 基于实时温度与目标温度的80%联动加热时长
            const uint16_t target_temp_for_cmp = standby_state.in_standby_mode ? SolderingStandbyTemp : SolderingTargetTemp;
            const uint16_t threshold_80 = (uint16_t)((target_temp_for_cmp * 8U) / 10U);
            const uint16_t threshold_92 = (uint16_t)((target_temp_for_cmp * 92U) / 100U);
            const uint16_t threshold_97 = (uint16_t)((target_temp_for_cmp * 97U) / 100U);

            unsigned long heat_duration = 200UL;
            if (Soldering_Temp >= (int)threshold_97) {
                heat_duration = 55UL;
            } else if (Soldering_Temp >= (int)threshold_92) {
                heat_duration = 85UL;
            } else if (Soldering_Temp >= (int)threshold_80) {
                heat_duration = 100UL;
            }

            if (current_time - state_start_time >= heat_duration) {
                pid_state = PID_POWER_OFF;
            }
            break;
        }
            
        case PID_SLEEP:
            // 休眠状态 - 关闭输出，每200ms读取一次温度用于显示
            Soldering_Set_PWM(0);
            Soldering_DutyCycle = 0.0f;
            soldering_smoothed_duty = 0.0f;
            
            // 每200ms读取一次温度
            if (current_time - standby_state.sleep_read_time >= 200) {
                if (MAX6675_TryLockBus()) {
                    if (MAX6675_Read_Soldering_Status() == 0) {
                        Soldering_Temp = (int)MAX6675_Read_Soldering_Temperature();
                        MAX6675_NotifySolderingPIDRead();
                    }
                    MAX6675_UnlockBus();
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
    request_heatgun_ui_color(PID_UI_TEXT_WHITE);


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
            // 恢复为白色（由 core0 UI 同步任务执行）
            request_heatgun_ui_color(PID_UI_TEXT_WHITE);
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
        heatgun_fault_shutdown_pending = false; // 重置故障关闭状态
        return;
    }

    // --- 热风枪温度读取 - 每200ms读取一次，独立于状态机 ---
    if (current_time - heatgun_last_temp_read_time >= 200) {
        if (MAX6675_TryLockBus()) {
            Heatgun_Status = MAX6675_Read_Heatgun_Status();
            if (Heatgun_Status == 0) {
                Heatgun_Temp = (int)MAX6675_Read_Heatgun_Temperature();
                MAX6675_NotifyHeatgunPIDRead();
            }
            MAX6675_UnlockBus();
        }
        heatgun_last_temp_read_time = current_time;
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

        // 热风枪从禁用切到启用时，短鸣一次
        if (Buzzer_Enabled && !heatgun_buzzer_state.short_active && !heatgun_buzzer_state.long_active) {
            Buzzer_ON();
            heatgun_buzzer_state.start_time = current_time;
            heatgun_buzzer_state.short_active = true;
        }

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

    // 处理热风枪故障关闭等待状态
    if (heatgun_fault_shutdown_pending) {
        const bool buzzer_active = heatgun_buzzer_state.short_active || heatgun_buzzer_state.long_active;
        if (!Buzzer_Enabled || !buzzer_active) {
            Heatgun_Set_PWM(0);
            Heatgun_Set_FAN_PWM(0);
            Heatgun_DutyCycle = 0.0f;
            Heatgun_Enabled = false;
            g_heatgun_switch_off_req_seq++;
            heatgun_fault_shutdown_pending = false;
        }
        return;
    }
    
    // --- 休眠状态转换 (基于消抖后信号的边沿检测) ---
    if (debounced_sleep_is_low != heatgun_sleep_state.last_processed_debounced_signal_is_low) {
        if (debounced_sleep_is_low) { 
            heatgun_sleep_state.in_sleep_mode_active = true;
            heatgun_pid_state = HEATGUN_PID_SLEEP_COOLING; 
            Heatgun_Set_PWM(0);
            Heatgun_DutyCycle = 0.0f;
            // 风扇将在 SLEEP_COOLING 状态中根据温度设置

            // 设置为蓝色（由 core0 UI 同步任务执行）
            request_heatgun_ui_color(PID_UI_TEXT_BLUE);
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
                // 恢复为白色（由 core0 UI 同步任务执行）
                request_heatgun_ui_color(PID_UI_TEXT_WHITE);
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
                     // 恢复为白色（由 core0 UI 同步任务执行）
                     request_heatgun_ui_color(PID_UI_TEXT_WHITE);
                     heatgun_sleep_state.ui_color_changed = false;
                }
            }
            break;

        case HEATGUN_PID_HEATING: {
            if (heatgun_sleep_state.in_sleep_mode_active) { // 被休眠信号中断
                heatgun_pid_state = HEATGUN_PID_SLEEP_COOLING; 
                // UI颜色和初始风扇已在边沿检测中处理或将在SLEEP_COOLING中处理
                break; 
            }

            if (Heatgun_Status != 0) { 
                if (!heatgun_fault_shutdown_pending) {
                    Heatgun_Set_PWM(0);
                    Heatgun_DutyCycle = 0.0f;
                    Heatgun_Set_FAN_PWM(0);
                    heatgun_pid_state = HEATGUN_PID_OFF;

                    if (Buzzer_Enabled && !heatgun_buzzer_state.long_active && !heatgun_buzzer_state.short_active) {
                        Buzzer_ON();
                        heatgun_buzzer_state.start_time = current_time;
                        heatgun_buzzer_state.long_active = true;
                    }
                    heatgun_fault_shutdown_pending = true;
                }
                return;
            }

            if (Heatgun_Temp > (HeatgunTargetTempMax + HEATGUN_SAFETY_MAX_TEMP_OFFSET)) { 
                if (!heatgun_fault_shutdown_pending) {
                    Heatgun_Set_PWM(0);
                    Heatgun_DutyCycle = 0.0f;
                    Heatgun_Set_FAN_PWM(0);
                    heatgun_pid_state = HEATGUN_PID_OFF;

                    if (Buzzer_Enabled && !heatgun_buzzer_state.long_active && !heatgun_buzzer_state.short_active) {
                        Buzzer_ON();
                        heatgun_buzzer_state.start_time = current_time;
                        heatgun_buzzer_state.long_active = true;
                    }
                    heatgun_fault_shutdown_pending = true;
                }
                return;
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
        }

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
                // 转换到 SLEEP_IDLE 状态，改变UI颜色为黑色（由 core0 UI 同步任务执行）
                request_heatgun_ui_color(PID_UI_TEXT_BLACK);
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