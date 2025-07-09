#include <INA226_Service.h>

lv_timer_t* INA226_Timer;

INA226 INA(0x40, &Wire1);

bool INA226_Init()
{
  Wire1.setSCL(INA226_SCL);
  Wire1.setSDA(INA226_SDA);
  Wire1.begin();
  if (!INA.begin())
  {
    return false; // 初始化失败，返回 false
  }
  INA.setMaxCurrentShunt(10, 0.005);
  INA.setAverage(INA226_128_SAMPLES);
  INA.setShuntVoltageConversionTime(INA226_1100_us);
  INA.setBusVoltageConversionTime(INA226_1100_us);
  INA226_Timer = lv_timer_create(INA226_Task, 100, NULL);
  return true; // 初始化成功，返回 true
}


void INA226_Task(lv_timer_t *timer)
{
    // 电压：2位整数，2位小数
    int voltage_full = round(INA.getBusVoltage() * 100);
    int voltage_int = voltage_full / 100;
    int voltage_frac = voltage_full % 100;
    
    // 电流：1位整数，2位小数
    int current_full = round(INA.getCurrent() * 1000);
    int current_int = current_full / 1000;
    int current_frac = current_full % 1000;
    
    // 功率：3位整数，1位小数
    int power_full = round(INA.getPower() * 10);
    int power_int = power_full / 10;
    int power_frac = power_full % 10;

    lv_label_set_text_fmt(ui_INA226Voltage, "%02d.%02dV", voltage_int, voltage_frac);
    lv_label_set_text_fmt(ui_INA226Current, "%01d.%03dA", current_int, current_frac);
    lv_label_set_text_fmt(ui_INA226Power, "%03d.%01dW", power_int, power_frac);

}

