#include <event_heatgun.h>

// 为全局变量添加heatgun_前缀
uint8_t heatgun_inputmod = 0; //0:不进行输入；1:温度输入；2:风速输入  
char heatgun_tempDisplay[4] = "---";  // 初始化为"---"，仅存储数字部分
uint8_t heatgun_inputPos = 0;        // 输入位置指针

// 添加临时存储变量，记录已修改的值
int heatgun_tempValue = 0;
int heatgun_windSpeedValue = 0;
bool heatgun_tempModified = false;   // 是否修改了温度值
bool heatgun_windSpeedModified = false; // 是否修改了风速值

// 更新显示函数 - 添加heatgun_前缀
void heatgun_updateDisplay() {
    heatgun_tempDisplay[3] = '\0';
    // 根据当前输入模式更新相应显示
    if(heatgun_inputmod == 1) // 温度输入模式
    {
        lv_label_set_text_fmt(ui_HeatgunSetTemp, "%s℃", heatgun_tempDisplay);
        lv_label_set_text_fmt(ui_HeatgunCurrentTemp, "%s℃", heatgun_tempDisplay);
    }
    else if(heatgun_inputmod == 2) // 风速输入模式
    {
        lv_label_set_text_fmt(ui_HeatgunSetWindSpeed, "%s%%", heatgun_tempDisplay);
        lv_label_set_text_fmt(ui_HeatgunCurrentWindSpeed, "%s%%", heatgun_tempDisplay);
    }
}

// 检查输入是否完整（没有'-'字符）
bool heatgun_isInputComplete() {
    for (int i = 0; i < 3; i++) {
        if (heatgun_tempDisplay[i] == '-') {
            return false;
        }
    }
    return true;
}

// 数字输入处理函数 - 添加heatgun_前缀
void heatgun_handleNumberInput(char num) {
    // 如果没有激活输入模式，不处理输入
    if(heatgun_inputmod == 0) {
        return;
    }
    
    // 如果已经输入了3位数字，则重新从第一位开始输入
    if (heatgun_inputPos >= 3) {
        // 重置所有位置为'-'
        for (int i = 0; i < 3; i++) {
            heatgun_tempDisplay[i] = '-';
        }
        // 重置输入位置指针
        heatgun_inputPos = 0;
    }
    
    // 输入新数字
    heatgun_tempDisplay[heatgun_inputPos] = num;
    
    // 确保后面的位置保持为'-'
    for (int i = heatgun_inputPos + 1; i < 3; i++) {
        heatgun_tempDisplay[i] = '-';
    }
    
    heatgun_inputPos++;
    heatgun_updateDisplay();
}

// 保存当前输入的值到临时变量
void heatgun_saveInputValue() {
    if (!heatgun_isInputComplete()) {
        return;  // 如果输入不完整，不保存
    }
    
    // 将输入转换为数值
    int inputValue = 0;
    for (int i = 0; i < 3; i++) {
        if (heatgun_tempDisplay[i] >= '0' && heatgun_tempDisplay[i] <= '9') {
            inputValue = inputValue * 10 + (heatgun_tempDisplay[i] - '0');
        }
    }
    
    // 根据当前输入模式保存到对应的临时变量
    if (heatgun_inputmod == 1) {
        // 温度输入模式
        if (inputValue < HeatgunTargetTempMin) {
            inputValue = HeatgunTargetTempMin;
        } else if (inputValue > HeatgunTargetTempMax) {
            inputValue = HeatgunTargetTempMax;
        }
        heatgun_tempValue = inputValue;
        heatgun_tempModified = true;
    } 
    else if (heatgun_inputmod == 2) {
        // 风速输入模式
        if (inputValue < HeatgunWindSpeedMin) {
            inputValue = HeatgunWindSpeedMin;
        } else if (inputValue > HeatgunWindSpeedMax) {
            inputValue = HeatgunWindSpeedMax;
        }
        heatgun_windSpeedValue = inputValue;
        heatgun_windSpeedModified = true;
    }
}

void ui_event_HeatgunConfirm(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_RELEASED) {
        // 如果当前正在输入，先保存当前输入的值
        if (heatgun_inputmod != 0 && heatgun_isInputComplete()) {
            heatgun_saveInputValue();
        }
        
        // 如果当前正在输入但输入不完整，不允许返回
        if (heatgun_inputmod != 0 && !heatgun_isInputComplete()) {
            // 重置当前输入
            for (int i = 0; i < 3; i++) {
                heatgun_tempDisplay[i] = '-';
            }
            heatgun_inputPos = 0;
            heatgun_updateDisplay();
            return; // 不执行后续操作
        }
        
        // 保存修改前的值，用于检测是否有变化
        uint16_t old_temp = HeatgunTargetTemp;
        uint16_t old_wind_speed = HeatgunWindSpeed;
        bool any_value_changed = false;
        
        // 应用所有已修改的值
        if (heatgun_tempModified) {
            HeatgunTargetTemp = heatgun_tempValue;
            // 更新温度显示控件
            lv_label_set_text_fmt(ui_HeatgunTargetTemp, "%.3d℃", HeatgunTargetTemp);
            // 检查温度是否真正发生变化
            if (old_temp != HeatgunTargetTemp) {
                any_value_changed = true;
            }
        }
        
        if (heatgun_windSpeedModified) {
            HeatgunWindSpeed = heatgun_windSpeedValue;
            // 更新风速显示控件
            lv_label_set_text_fmt(ui_HeatgunWindSpeed, "%.3d%%", HeatgunWindSpeed);
            // 检查风速是否真正发生变化
            if (old_wind_speed != HeatgunWindSpeed) {
                any_value_changed = true;
            }
        }
        
        // 只有当数值实际发生变化时才保存到EEPROM
        if (any_value_changed) {
            WriteHeatgun(); // 保存热风枪设置
        }
        
        // 重置所有状态
        heatgun_inputPos = 0;
        heatgun_inputmod = 0;
        heatgun_tempModified = false;
        heatgun_windSpeedModified = false;
        
        // 将输入显示重置为初始状态
        for (int i = 0; i < 3; i++) {
            heatgun_tempDisplay[i] = '-';
        }
        
        // 重置温度和风速输入控件显示
        lv_label_set_text(ui_HeatgunSetTemp, "---℃");
        lv_label_set_text(ui_HeatgunSetWindSpeed, "---%");
        
        // 清除选中状态
        lv_obj_clear_state(ui_HeatgunSetTemp, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_HeatgunSetWindSpeed, LV_STATE_CHECKED);
        
        // 切换回主屏幕
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_MainScreen_screen_init);
        lvgl_group_to_main();
    }
}

void ui_event_HeatgunDelete(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        if (heatgun_inputmod == 0) {
            // 当未选择输入模式时，清除所有已修改的状态
            heatgun_tempModified = false;
            heatgun_windSpeedModified = false;
            
            // 清空输入显示
            for(int i = 0; i < 3; i++) {
                heatgun_tempDisplay[i] = '-';
            }
            
            // 重置温度和风速输入控件显示
            lv_label_set_text(ui_HeatgunSetTemp, "---℃");
            lv_label_set_text(ui_HeatgunSetWindSpeed, "---%");
            
            // 这里移除了消息框相关代码
        }
        else if (heatgun_inputmod == 1) {
            // 清除温度输入
            for(int i = 0; i < 3; i++) {
                heatgun_tempDisplay[i] = '-';
            }
            heatgun_inputPos = 0;
            heatgun_updateDisplay();
            
            // 标记温度未修改
            heatgun_tempModified = false;
        }
        else if (heatgun_inputmod == 2) {
            // 清除风速输入
            for(int i = 0; i < 3; i++) {
                heatgun_tempDisplay[i] = '-';
            }
            heatgun_inputPos = 0;
            heatgun_updateDisplay();
            
            // 标记风速未修改
            heatgun_windSpeedModified = false;
        }
    }
}

void ui_event_HeatgunNum0(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('0');
    }
}

void ui_event_HeatgunNum1(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('1');
    }
}

void ui_event_HeatgunNum2(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('2');
    }
}

void ui_event_HeatgunNum3(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('3');
    }
}

void ui_event_HeatgunNum4(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('4');
    }
}

void ui_event_HeatgunNum5(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('5');
    }
}

void ui_event_HeatgunNum6(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('6');
    }
}

void ui_event_HeatgunNum7(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('7');
    }
}

void ui_event_HeatgunNum8(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('8');
    }
}

void ui_event_HeatgunNum9(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        heatgun_handleNumberInput('9');
    }
}

void ui_event_HeatgunSetTemp(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if (event_code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(target,LV_STATE_CHECKED)) {
        // 如果之前在输入风速且完成了输入，先保存风速的值
        if (heatgun_inputmod == 2 && heatgun_isInputComplete()) {
            heatgun_saveInputValue();
        }
        
        HeatgunSetTemp(e);
        lv_obj_clear_state(ui_HeatgunSetWindSpeed,LV_STATE_CHECKED);
        heatgun_inputmod = 1; //温度输入
        
        // 清空输入显示，准备新的输入
        for (int i = 0; i < 3; i++) {
            heatgun_tempDisplay[i] = '-';
        }
        heatgun_inputPos = 0;
        heatgun_updateDisplay();
    }
    if (event_code == LV_EVENT_VALUE_CHANGED && !lv_obj_has_state(target,LV_STATE_CHECKED)) {
        // 如果当前在输入温度且完成了输入，保存温度的值
        if (heatgun_inputmod == 1 && heatgun_isInputComplete()) {
            heatgun_saveInputValue();
        }
        
        HeatgunSetTempUN(e);
        heatgun_inputmod = 0; //不进行输入
    }
}

void ui_event_HeatgunSetWindSpeed(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if (event_code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(target,LV_STATE_CHECKED)) {
        // 如果之前在输入温度且完成了输入，先保存温度的值
        if (heatgun_inputmod == 1 && heatgun_isInputComplete()) {
            heatgun_saveInputValue();
        }
        
        HeatgunSetWindSpeed(e);
        lv_obj_clear_state(ui_HeatgunSetTemp,LV_STATE_CHECKED);
        heatgun_inputmod = 2; //风速输入
        
        // 清空输入显示，准备新的输入
        for (int i = 0; i < 3; i++) {
            heatgun_tempDisplay[i] = '-';
        }
        heatgun_inputPos = 0;
        heatgun_updateDisplay();
    }
    if (event_code == LV_EVENT_VALUE_CHANGED && !lv_obj_has_state(target,LV_STATE_CHECKED)) {
        // 如果当前在输入风速且完成了输入，保存风速的值
        if (heatgun_inputmod == 2 && heatgun_isInputComplete()) {
            heatgun_saveInputValue();
        }
        
        HeatgunSetWindSpeedUN(e);
        heatgun_inputmod = 0; //不进行输入
    }
}

void ui_event_HeatgunSetBack(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_RELEASED) {

        // 重置所有状态
        heatgun_inputPos = 0;
        heatgun_inputmod = 0;
        heatgun_tempModified = false;
        heatgun_windSpeedModified = false;
        
        // 将输入显示重置为初始状态
        for (int i = 0; i < 3; i++) {
            heatgun_tempDisplay[i] = '-';
        }
        
        // 重置温度和风速输入控件显示
        lv_label_set_text(ui_HeatgunSetTemp, "---℃");
        lv_label_set_text(ui_HeatgunSetWindSpeed, "---%");
        
        // 清除选中状态
        lv_obj_clear_state(ui_HeatgunSetTemp, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_HeatgunSetWindSpeed, LV_STATE_CHECKED);

        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_MainScreen_screen_init);
        lvgl_group_to_main();
    }
}