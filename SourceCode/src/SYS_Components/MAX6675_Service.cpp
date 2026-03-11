#include <MAX6675_Service.h>

MAX6675 sensor1(MAX6675_SO_PIN, MAX6675_CS1_PIN, MAX6675_CLK_PIN, &SPI1, 1000000);
MAX6675 sensor2(MAX6675_SO_PIN, MAX6675_CS2_PIN, MAX6675_CLK_PIN, &SPI1, 1000000);
lv_timer_t* MAX6675_Display_Timer;

static volatile uint32_t g_soldering_pid_last_read_ms = 0;
static volatile uint32_t g_heatgun_pid_last_read_ms = 0;

bool MAX6675_TryLockBus()
{
    return mutex_try_enter(&spi_mutex, nullptr);
}

void MAX6675_UnlockBus()
{
    mutex_exit(&spi_mutex);
}

void MAX6675_NotifySolderingPIDRead()
{
    g_soldering_pid_last_read_ms = millis();
}

void MAX6675_NotifyHeatgunPIDRead()
{
    g_heatgun_pid_last_read_ms = millis();
}

void MAX6675_Init()
{
    sensor1.begin();
    sensor2.begin();
    
    MAX6675_Display_Timer = lv_timer_create(MAX6675_Display_Task, 200, NULL);
}

void MAX6675_Read_Task()
{
    static unsigned long last_soldering_bg_read_time = 0;
    static unsigned long last_heatgun_bg_read_time = 0;
    static unsigned long soldering_disabled_time = 0;
    static bool prev_soldering_enabled = false;
    static int soldering_last_valid_temp = 0;

    const unsigned long current_time = millis();

    // 电烙铁温度读取接管逻辑
    if (Soldering_Enabled) {
        prev_soldering_enabled = true;
    } else {
        if (prev_soldering_enabled) {
            soldering_disabled_time = current_time;
            soldering_last_valid_temp = Soldering_Temp;
        }
        prev_soldering_enabled = false;

        // 关闭后增加交接保护时间，兼容 PID 最后一次读温度的边界时序
        const unsigned long handover_anchor =
            (g_soldering_pid_last_read_ms > soldering_disabled_time) ?
            g_soldering_pid_last_read_ms : soldering_disabled_time;

        if ((current_time - handover_anchor >= 300) &&
            (current_time - last_soldering_bg_read_time >= 200)) {
            if (MAX6675_TryLockBus()) {
                Soldering_Status = MAX6675_Read_Soldering_Status();

                if (Soldering_Status == 0) {
                    const int new_temp = (int)MAX6675_Read_Soldering_Temperature();
                    // 异常值过滤：温度变化不应超过50°C
                    if (abs(new_temp - soldering_last_valid_temp) <= 50) {
                        Soldering_Temp = new_temp;
                        soldering_last_valid_temp = new_temp;
                    }
                }

                MAX6675_UnlockBus();
                last_soldering_bg_read_time = current_time;
            }
        }
    }

    // 热风枪温度读取接管逻辑（关闭时由 core0 按 200ms 固定采样）
    const unsigned long heatgun_handover_anchor =
        (g_heatgun_pid_last_read_ms > last_heatgun_bg_read_time) ?
        g_heatgun_pid_last_read_ms : last_heatgun_bg_read_time;

    if (!Heatgun_Enabled && (current_time - heatgun_handover_anchor >= 200)) {
        if (MAX6675_TryLockBus()) {
            Heatgun_Status = MAX6675_Read_Heatgun_Status();
            if (Heatgun_Status == 0) {
                Heatgun_Temp = (int)MAX6675_Read_Heatgun_Temperature();
            }
            MAX6675_UnlockBus();
            last_heatgun_bg_read_time = current_time;
        }
    }
}

void MAX6675_Display_Task(lv_timer_t *timer)
{
    if (Soldering_Status == 0)
    {
        // 正常显示温度
        lv_label_set_text_fmt(ui_SolderingTemp, "%03d", Soldering_Temp);
        lv_bar_set_value(ui_BarSolderingDuty, (int)Soldering_DutyCycle, LV_ANIM_ON);
    }
    else
    {
        // 错误时显示ERR文本
        lv_label_set_text(ui_SolderingTemp, "ERR");
        lv_bar_set_value(ui_BarSolderingDuty, 0, LV_ANIM_ON);
    }

    if (Heatgun_Status == 0)
    {
        // 正常显示温度
        lv_label_set_text_fmt(ui_HeatgunTemp, "%03d", Heatgun_Temp);
        lv_bar_set_value(ui_BarHeatgunDuty, (int)Heatgun_DutyCycle, LV_ANIM_ON);
    }
    else
    {
        // 错误时显示ERR文本
        lv_label_set_text(ui_HeatgunTemp, "ERR");
        lv_bar_set_value(ui_BarHeatgunDuty, 0, LV_ANIM_ON);
    }
}

uint8_t MAX6675_Read_Soldering_Status()
{
    return sensor1.read();
}

float MAX6675_Read_Soldering_Temperature()
{
    return sensor1.getTemperature();
}

uint8_t MAX6675_Read_Heatgun_Status()
{
    return sensor2.read();
}

float MAX6675_Read_Heatgun_Temperature()
{
    return sensor2.getTemperature();
}