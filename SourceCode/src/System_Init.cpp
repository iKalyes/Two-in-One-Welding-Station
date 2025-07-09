#include <System_Init.h>

// 定义延时时间 (毫秒)
#define INIT_STEP_DELAY_MS 250

// LVGL 友好的延时函数
void lvgl_friendly_delay(uint32_t ms) {
    if (ms == 0) {
        lv_task_handler(); // 即使延时为0，也至少处理一次任务
        return;
    }
    uint32_t start = millis();
    while (millis() - start < ms) {
        lv_task_handler();
    }
    lv_task_handler(); // 确保在延时结束后再次处理一次
}

// 初始化失败处理函数
void handle_init_failure(const char* error_msg) {
    // 设置进度条为红色错误状态
    if (ui_InitializeStatusBar) {
        lv_obj_set_style_bg_color(ui_InitializeStatusBar, lv_color_hex(0xFF0000), LV_PART_INDICATOR);
    }
    
    // 显示错误信息
    if (ui_InitializeStatus) {
        lv_textarea_add_text(ui_InitializeStatus, error_msg);
        lv_textarea_add_text(ui_InitializeStatus, "Initialization STOPPED due to critical error!\n");
    }
    
    // 错误提示音
    if(Buzzer_Enabled == true)
    {
        for (int i = 0; i < 3; i++) {
            Buzzer_ON();
            lvgl_friendly_delay(150);
            Buzzer_OFF();
            lvgl_friendly_delay(150);
        }
    }
}

void System_Init()
{
    char status_buffer[128];
    bool init_success = true;

    // 1. display_init
    Soldering_GPIO_Init();
    Heatgun_GPIO_Init();
    Cooling_FAN_GPIO_Init();
    Buzzer_GPIO_Init();
    Soldering_Set_PWM(0); // 确保在初始化时电烙铁关闭
    Heatgun_Set_PWM(0); // 确保在初始化时热风枪关闭
    Heatgun_Set_FAN_PWM(0); // 确保在初始化时热风枪风扇关闭
    display_init(); // 假设 display_init() 已声明或定义
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 8, LV_ANIM_ON);
    if (ui_InitializeStatus) lv_textarea_add_text(ui_InitializeStatus, "Display initialized.\n");
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 16, LV_ANIM_ON);
    if (ui_InitializeStatus) lv_textarea_add_text(ui_InitializeStatus, "All GPIOs initialized.\n");
    if(Buzzer_Enabled == true)
    {
        Buzzer_ON();
        lvgl_friendly_delay(INIT_STEP_DELAY_MS * 2);
        Buzzer_OFF();
    }
    else
    {
        lvgl_friendly_delay(INIT_STEP_DELAY_MS * 2); // 如果蜂鸣器未启用，仍然延时以保持一致性
    }

    // 2. encoder_lvgl_init
    encoder_lvgl_init();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 24, LV_ANIM_ON);
    if (ui_InitializeStatus) lv_textarea_add_text(ui_InitializeStatus, "LVGL Encoder initialized.\n");
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 3. lvgl_group_init
    lvgl_group_init();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 32, LV_ANIM_ON);
    if (ui_InitializeStatus) lv_textarea_add_text(ui_InitializeStatus, "LVGL Group initialized.\n");
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 4. INA226_Init - 关键初始化步骤
    bool ina226_ok = INA226_Init();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 40, LV_ANIM_ON);
    if (ui_InitializeStatus) {
        sprintf(status_buffer, "INA226 initialization: %s.\n", ina226_ok ? "SUCCESS" : "FAILED!");
        lv_textarea_add_text(ui_InitializeStatus, status_buffer);
    }
    
    if (!ina226_ok) {
        handle_init_failure("ERROR: INA226 initialization failed!\n");
        return; // 停止初始化
    }
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 5. TMP102_Init - 关键初始化步骤
    bool tmp102_ok = TMP102_Init();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 48, LV_ANIM_ON);
    if (ui_InitializeStatus) {
        sprintf(status_buffer, "TMP102 initialization: %s.\n", tmp102_ok ? "SUCCESS" : "FAILED!");
        lv_textarea_add_text(ui_InitializeStatus, status_buffer);
    }
    
    if (!tmp102_ok) {
        handle_init_failure("ERROR: TMP102 initialization failed!\n");
        return; // 停止初始化
    }
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 6. MAX6675_Init
    MAX6675_Init();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 56, LV_ANIM_ON);
    if (ui_InitializeStatus) lv_textarea_add_text(ui_InitializeStatus, "MAX6675 initialized.\n");
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 7. MAX6675_Read_Soldering_Status - 关键检测步骤
    uint8_t soldering_status = MAX6675_Read_Soldering_Status();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 64, LV_ANIM_ON);
    bool soldering_ok = (soldering_status == 0 || soldering_status == 4);
    if (ui_InitializeStatus) {
        sprintf(status_buffer, "Soldering thermocouple: %s (status: %d).\n", soldering_ok ? "OK" : "ERROR", soldering_status);
        lv_textarea_add_text(ui_InitializeStatus, status_buffer);
    }
    
    if (!soldering_ok) {
        handle_init_failure("ERROR: Soldering thermocouple check failed!\n");
        return; // 停止初始化
    }
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 8. MAX6675_Read_Heatgun_Status - 关键检测步骤
    uint8_t heatgun_status = MAX6675_Read_Heatgun_Status();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 72, LV_ANIM_ON);
    bool heatgun_ok = (heatgun_status == 0 || heatgun_status == 4);
    if (ui_InitializeStatus) {
        sprintf(status_buffer, "Heatgun thermocouple: %s (status: %d).\n", heatgun_ok ? "OK" : "ERROR", heatgun_status);
        lv_textarea_add_text(ui_InitializeStatus, status_buffer);
    }
    
    if (!heatgun_ok) {
        handle_init_failure("ERROR: Heatgun thermocouple check failed!\n");
        return; // 停止初始化
    }
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 9. Soldering_PID_Compute_Init
    Soldering_PID_Compute_Init();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 86, LV_ANIM_ON);
    if (ui_InitializeStatus) lv_textarea_add_text(ui_InitializeStatus, "Soldering PID initialized.\n");
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 10. Heatgun_PID_Compute_Init
    Heatgun_PID_Compute_Init();
    if (ui_InitializeStatusBar) lv_bar_set_value(ui_InitializeStatusBar, 99, LV_ANIM_ON);
    if (ui_InitializeStatus) lv_textarea_add_text(ui_InitializeStatus, "Heatgun PID initialized.\n");
    lvgl_friendly_delay(INIT_STEP_DELAY_MS);

    // 所有初始化完成
    if (ui_InitializeStatus) {
        lv_textarea_add_text(ui_InitializeStatus, "All initializations done.\nLoading main screen...\n");
    }
    // 允许用户看到完成消息，然后再切换屏幕
    if(Buzzer_Enabled == true)
    {
        lvgl_friendly_delay(400);
        Buzzer_ON();
        lvgl_friendly_delay(100);
        Buzzer_OFF();

    }
    else
    {
        lvgl_friendly_delay(500); // 如果蜂鸣器未启用，仍然延时以保持一致性
    }

    // 跳转到主屏幕
    if (ui_MainScreen) {
        // lv_scr_load_anim 的第四个参数是动画开始前的延时，这个延时由 LVGL 内部处理，是无阻塞的。
        // 将其设置为0，因为我们已经在上面用 lvgl_friendly_delay 添加了延时。
        lv_scr_load_anim(ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    } else {
         if (ui_InitializeStatus) {
             lv_textarea_add_text(ui_InitializeStatus, "Error: Main screen (ui_MainScreen) is NULL!\n");
         }
    }
}