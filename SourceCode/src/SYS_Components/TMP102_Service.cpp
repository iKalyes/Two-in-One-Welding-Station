#include <TMP102_Service.h>

TMP102 sensor;
lv_timer_t* TMP102_Timer;

int temp_full;
int temp_int;
int temp_frac;

bool TMP102_Init()
{
    Wire1.setSDA(TMP102_SDA);
    Wire1.setSCL(TMP102_SCL);
    Wire1.begin();
    
    if(!sensor.begin(0x48, Wire1))
    {
        return false;
    }
    // set the number of consecutive faults before triggering alarm.
    // 0-3: 0:1 fault, 1:2 faults, 2:4 faults, 3:6 faults.
    sensor.setFault(0);  // Trigger alarm immediately
    // set the polarity of the Alarm. (0:Active LOW, 1:Active HIGH).
    sensor.setAlertPolarity(1); // Active HIGH
    // set the sensor in Comparator Mode (0) or Interrupt Mode (1).
    sensor.setAlertMode(0); // Comparator Mode.
    // set the Conversion Rate (how quickly the sensor gets a new reading)
    //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
    sensor.setConversionRate(1);
    //set Extended Mode.
    //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
    sensor.setExtendedMode(0);
    sensor.wakeup();
    
    return true; // 初始化成功，启用TMP102功能
}

void TMP102_Display_Init()
{
    TMP102_Timer = lv_timer_create(TMP102_Display_Task, 1000, NULL); // 每1000ms更新一次显示
}

void TMP102_Task()
{
    // 温度：2位整数，2位小数
    temp_full = round(sensor.readTempC() * 100);
    temp_int = temp_full / 100;
    temp_frac = temp_full % 100;
}

void TMP102_Display_Task(lv_timer_t *timer)
{
    if (temp_full > 0)
    {
        lv_label_set_text_fmt(ui_TMP102Temp, "%02d.%02d℃", temp_int, temp_frac);
        if (CoolingFan_Enabled == true)
        {
            Cooling_FAN_Set_PWM((float)temp_int * 2.0f); // Cooling fan speed based on temperature
        }
        else
        {
            Cooling_FAN_Set_PWM(0); // Turn off cooling fan if not enabled
        }
    }
    else
    {
        lv_label_set_text(ui_TMP102Temp, "ERROR℃");
    }
}