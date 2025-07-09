#include <GPIO_Service.h>

void Soldering_GPIO_Init()
{
    analogReadResolution(12);
    pinMode(Soldering_SLEEP, INPUT);
    pinMode(Soldering_ID, INPUT);
    
    // 设置焊台PWM引脚 - 100Hz PWM
    gpio_set_function(Soldering_PWM, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Soldering_PWM);
    uint channel = pwm_gpio_to_channel(Soldering_PWM);
    pwm_set_clkdiv(slice_num, 250.0);  // 分频器
    pwm_set_wrap(slice_num, 10000);     // 最大计数值 (分辨率)
    pwm_set_chan_level(slice_num, channel, 0); // 初始占空比为0
    pwm_set_enabled(slice_num, true);
}

uint16_t Soldering_Read_ID()
{
    // 读取焊台ID
    return (uint16_t)analogRead(Soldering_ID);
}

bool Soldering_Read_SLEEP()
{
    // 读取焊台睡眠状态
    return !digitalRead(Soldering_SLEEP); // LOW表示焊台处于睡眠状态
}

void Soldering_Set_PWM(float value)
{
    // 设置焊台PWM占空比
    uint slice_num = pwm_gpio_to_slice_num(Soldering_PWM);
    uint channel = pwm_gpio_to_channel(Soldering_PWM);
    if (value > SolderingMaxPower) {
        value = SolderingMaxPower; // 限制最大值
    }
    pwm_set_chan_level(slice_num, channel, value * 100); // 设置占空比
}

//——————————————————————————————————————————————————————————————————————//

void Heatgun_GPIO_Init()
{
    pinMode(Heatgun_SLEEP, INPUT);
    
    // 设置热风枪PWM引脚 - 50Hz PWM
    gpio_set_function(Heatgun_PWM, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Heatgun_PWM);
    uint channel = pwm_gpio_to_channel(Heatgun_PWM);
    pwm_set_clkdiv(slice_num, 250.0);  // 分频器
    pwm_set_wrap(slice_num, 20000);     // 最大计数值 (分辨率)
    pwm_set_chan_level(slice_num, channel, 0); // 初始占空比为0
    pwm_set_enabled(slice_num, true);
    
    // 设置热风枪风扇PWM引脚 - 1000Hz PWM
    gpio_set_function(Heatgun_FAN_PWM, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(Heatgun_FAN_PWM);
    channel = pwm_gpio_to_channel(Heatgun_FAN_PWM);
    pwm_set_clkdiv(slice_num, 250.0);  // 分频器
    pwm_set_wrap(slice_num, 1000);     // 最大计数值 (分辨率)
    pwm_set_chan_level(slice_num, channel, 0); // 初始占空比为0
    pwm_set_enabled(slice_num, true);
}

bool Heatgun_Read_SLEEP()
{
    // 读取热风枪睡眠状态
    return digitalRead(Heatgun_SLEEP); // LOW表示热风枪处于睡眠状态
}

void Heatgun_Set_PWM(float value)
{
    // 设置热风枪PWM占空比
    uint slice_num = pwm_gpio_to_slice_num(Heatgun_PWM);
    uint channel = pwm_gpio_to_channel(Heatgun_PWM);
    if (value > 100) {
        value = 100; // 限制最大值
    }
    pwm_set_chan_level(slice_num, channel, value * 200); // 设置占空比
}

void Heatgun_Set_FAN_PWM(float value)
{
    // 设置热风枪风扇PWM占空比
    uint slice_num = pwm_gpio_to_slice_num(Heatgun_FAN_PWM);
    uint channel = pwm_gpio_to_channel(Heatgun_FAN_PWM);
    if (value > 100) {
        value = 100; // 限制最大值
    }
    pwm_set_chan_level(slice_num, channel, value * 10); // 设置占空比
}

//——————————————————————————————————————————————————————————————————————//

void Cooling_FAN_GPIO_Init()
{
    // 设置冷却风扇PWM引脚 - 50Hz PWM
    gpio_set_function(Cooling_FAN_PWM, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Cooling_FAN_PWM);
    uint channel = pwm_gpio_to_channel(Cooling_FAN_PWM);
    pwm_set_clkdiv(slice_num, 250.0);  // 分频器
    pwm_set_wrap(slice_num, 50000);     // 最大计数值 (分辨率)
    pwm_set_chan_level(slice_num, channel, 0); // 初始占空比为0
    pwm_set_enabled(slice_num, true);
}

void Cooling_FAN_Set_PWM(float value)
{
    // 设置冷却风扇PWM占空比
    uint slice_num = pwm_gpio_to_slice_num(Cooling_FAN_PWM);
    uint channel = pwm_gpio_to_channel(Cooling_FAN_PWM);
    if (value > 100) {
        value = 100; // 限制最大值
    }
    pwm_set_chan_level(slice_num, channel, value * 500); // 设置占空比
}

//——————————————————————————————————————————————————————————————————————//

void Buzzer_GPIO_Init()
{
    // 设置蜂鸣器PWM引脚 - 2700Hz PWM
    gpio_set_function(Buzzer_PWM, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Buzzer_PWM);
    uint channel = pwm_gpio_to_channel(Buzzer_PWM);
    pwm_set_clkdiv(slice_num, 34.3);  // 分频器
    pwm_set_wrap(slice_num, 2700);     // 最大计数值 (分辨率)
    pwm_set_chan_level(slice_num, channel, 0); // 初始占空比为0
    pwm_set_enabled(slice_num, true);
}

void Buzzer_Set_PWM(float value)
{
    // 设置蜂鸣器PWM占空比
    uint slice_num = pwm_gpio_to_slice_num(Buzzer_PWM);
    uint channel = pwm_gpio_to_channel(Buzzer_PWM);
    if (value > 100) {
        value = 100; // 限制最大值
    }
    // 将0-100映射到0-2700的计数值
    pwm_set_chan_level(slice_num, channel, (uint16_t)((value / 100.0) * 2700)); // 设置占空比
}

void Buzzer_ON()
{
    // 打开蜂鸣器
    Buzzer_Set_PWM(70.0f); // 设置占空比为70%
}

void Buzzer_OFF()
{
    // 关闭蜂鸣器
    Buzzer_Set_PWM(0.0f); // 设置占空比为0%
}