#include <INA226_Service.h>

int voltage_full;
int voltage_int;
int voltage_frac;

int current_full;
int current_int;
int current_frac;

int power_full;
int power_int;
int power_frac;

lv_timer_t *INA226_Timer;

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

  // 采样配置按 100ms 任务周期匹配：
  // 总转换时间约为 128 * (332us + 332us) = 84.99ms，
  // 这样每次 100ms 任务读取时基本都能拿到一组新的平均结果。
  INA.setAverage(INA226_128_SAMPLES);
  INA.setShuntVoltageConversionTime(INA226_332_us);
  INA.setBusVoltageConversionTime(INA226_332_us);
  return true; // 初始化成功，返回 true
}

void INA226_Display_Init()
{
  INA226_Timer = lv_timer_create(INA226_Display_Task, 50, NULL);
}

void INA226_Task()
{
  // 电压：2位整数，2位小数
  voltage_full = round(INA.getBusVoltage() * 100);
  voltage_int = voltage_full / 100;
  voltage_frac = voltage_full % 100;

  // 电流：1位整数，2位小数
  current_full = round(INA.getCurrent() * 1000);
  current_int = current_full / 1000;
  current_frac = current_full % 1000;

  // 功率：3位整数，1位小数
  power_full = round(INA.getPower() * 10);
  power_int = power_full / 10;
  power_frac = power_full % 10;
}

void INA226_Display_Task(lv_timer_t *timer)
{
  lv_label_set_text_fmt(ui_INA226Voltage, "%02d.%02dV", voltage_int, voltage_frac);
  lv_label_set_text_fmt(ui_INA226Current, "%01d.%03dA", current_int, current_frac);
  lv_label_set_text_fmt(ui_INA226Power, "%03d.%01dW", power_int, power_frac);
}
