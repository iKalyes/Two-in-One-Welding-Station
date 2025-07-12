#include <MAX6675_Service.h>

MAX6675 sensor1(MAX6675_SO_PIN, MAX6675_CS1_PIN, MAX6675_CLK_PIN, &SPI1, 1000000);
MAX6675 sensor2(MAX6675_SO_PIN, MAX6675_CS2_PIN, MAX6675_CLK_PIN, &SPI1, 1000000);
lv_timer_t* MAX6675_Display_Timer;


// 添加DigitRoller对象
DigitRoller* soldering_temp_display = nullptr;
DigitRoller* heatgun_temp_display = nullptr;

void MAX6675_Init()
{
    sensor1.begin();
    sensor2.begin();
    
    // 创建数字流转显示对象
    // 需要传入原来ui_SolderingTemp和ui_HeatgunTemp标签的父容器
    lv_obj_t* soldering_parent = lv_obj_get_parent(ui_SolderingTemp);
    lv_obj_t* heatgun_parent = lv_obj_get_parent(ui_HeatgunTemp);
    
    // 创建数字流转对象
    soldering_temp_display = new DigitRoller(soldering_parent);
    heatgun_temp_display = new DigitRoller(heatgun_parent);
    
    // 设置位置（替换原来的标签位置）
    lv_coord_t sol_x = lv_obj_get_x(ui_SolderingTemp);
    lv_coord_t sol_y = lv_obj_get_y(ui_SolderingTemp);
    lv_coord_t heat_x = lv_obj_get_x(ui_HeatgunTemp);
    lv_coord_t heat_y = lv_obj_get_y(ui_HeatgunTemp);
    
    soldering_temp_display->setPosition(sol_x, sol_y);
    heatgun_temp_display->setPosition(heat_x, heat_y);
    
    // 隐藏原来的标签
    lv_obj_add_flag(ui_SolderingTemp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_HeatgunTemp, LV_OBJ_FLAG_HIDDEN);
    
    // 设置初始值
    soldering_temp_display->setValue(0);
    heatgun_temp_display->setValue(0);
    
    MAX6675_Display_Timer = lv_timer_create(MAX6675_Display_Task, 200, NULL);
}

void MAX6675_Read_Task()
{
    if(Soldering_Enabled == false)
    {
        // 添加静态变量用于控制200ms间隔读取温度
        static unsigned long last_temp_read_time = 0;
        unsigned long current_time = millis();
        
        if (current_time - last_temp_read_time >= 200) {
            // 互斥锁保护温度读取
            if (!temp_read_mutex) {
                temp_read_mutex = true;
                Soldering_Status = MAX6675_Read_Soldering_Status();
                if (Soldering_Status == 0) {
                    Soldering_Temp = (int)MAX6675_Read_Soldering_Temperature();
                }
                temp_read_mutex = false;
                last_temp_read_time = current_time;
            }
            // 如果互斥锁被占用，跳过本次读取，保持上次的温度值
        }
    }

    if(Heatgun_Enabled == false)
    {
        // 添加静态变量用于控制200ms间隔读取温度
        static unsigned long last_heatgun_temp_read_time = 0;
        unsigned long current_time = millis();
        
        if (current_time - last_heatgun_temp_read_time >= 200) {
            // 互斥锁保护温度读取
            if (!temp_read_mutex) {
                temp_read_mutex = true;
                Heatgun_Status = MAX6675_Read_Heatgun_Status();
                if (Heatgun_Status == 0) {
                    Heatgun_Temp = (int)MAX6675_Read_Heatgun_Temperature();
                }
                temp_read_mutex = false;
                last_heatgun_temp_read_time = current_time;
            }
            // 如果互斥锁被占用，跳过本次读取，保持上次的温度值
        }
    }
}

void MAX6675_Display_Task(lv_timer_t *timer)
{
    if (Soldering_Status == 0)
    {
        // 正常显示温度
        soldering_temp_display->setValue(Soldering_Temp);
        MAX6675_Soldering_Numberflow(); // 确保烙铁温度显示更新
        lv_bar_set_value(ui_BarSolderingDuty, (int)Soldering_DutyCycle, LV_ANIM_ON);
    }
    else
    {
        // 错误时显示ERR文本
        MAX6675_Soldering_Normal(); // 确保烙铁温度显示更新
        lv_label_set_text(ui_SolderingTemp, "ERR");
        lv_bar_set_value(ui_BarSolderingDuty, 0, LV_ANIM_ON);
    }

    if (Heatgun_Status == 0)
    {
        // 正常显示温度
        heatgun_temp_display->setValue(Heatgun_Temp);
        MAX6675_Heatgun_Numberflow(); // 确保热风枪温度显示更新
        lv_bar_set_value(ui_BarHeatgunDuty, (int)Heatgun_DutyCycle, LV_ANIM_ON);
    }
    else
    {
        // 错误时显示ERR文本
        MAX6675_Heatgun_Normal(); // 确保热风枪温度显示更新
        lv_label_set_text(ui_HeatgunTemp, "ERR");
        lv_bar_set_value(ui_BarHeatgunDuty, 0, LV_ANIM_ON);
    }
}

void MAX6675_Soldering_Numberflow()
{
    lv_obj_add_flag(ui_SolderingTemp, LV_OBJ_FLAG_HIDDEN); // 隐藏错误标签
    lv_obj_clear_flag(soldering_temp_display->getContainer(), LV_OBJ_FLAG_HIDDEN); // 显示数字流转
}

void  MAX6675_Soldering_Normal()
{
    lv_obj_clear_flag(ui_SolderingTemp, LV_OBJ_FLAG_HIDDEN); // 显示错误标签
    lv_obj_add_flag(soldering_temp_display->getContainer(), LV_OBJ_FLAG_HIDDEN); // 隐藏数字流转
}

void MAX6675_Heatgun_Numberflow()
{
    lv_obj_add_flag(ui_HeatgunTemp, LV_OBJ_FLAG_HIDDEN); // 隐藏错误标签
    lv_obj_clear_flag(heatgun_temp_display->getContainer(), LV_OBJ_FLAG_HIDDEN); // 显示数字流转
}

void MAX6675_Heatgun_Normal()
{
    lv_obj_clear_flag(ui_HeatgunTemp, LV_OBJ_FLAG_HIDDEN); // 显示错误标签
    lv_obj_add_flag(heatgun_temp_display->getContainer(), LV_OBJ_FLAG_HIDDEN); // 隐藏数字流转
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