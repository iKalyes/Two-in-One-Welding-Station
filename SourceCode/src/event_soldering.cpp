#include <event_soldering.h>

char tempDisplay[4] = "---";  // 初始化为"---"，仅存储数字部分
uint8_t inputPos = 0;        // 输入位置指针

// 更新显示函数，添加摄氏度单位
void updateDisplay() {
    tempDisplay[3] = '\0';
    // 添加摄氏度单位
    lv_label_set_text_fmt(ui_SolderingSetTemp, "%s℃", tempDisplay);
}

// 数字输入处理函数 - 修改为支持循环输入
void handleNumberInput(char num) {
    // 如果已经输入了3位数字，则重新从第一位开始输入
    if (inputPos >= 3) {
        // 重置所有位置为'-'
        for (int i = 0; i < 3; i++) {
            tempDisplay[i] = '-';
        }
        // 重置输入位置指针
        inputPos = 0;
    }
    
    // 输入新数字
    tempDisplay[inputPos] = num;
    
    // 确保后面的位置保持为'-'
    for (int i = inputPos + 1; i < 3; i++) {
        tempDisplay[i] = '-';
    }
    
    inputPos++;
    updateDisplay();
}

void ui_event_SolderingConfirm(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_RELEASED) {
        // 检查是否输入了完整的三位数字
        bool isComplete = true;
        for (int i = 0; i < 3; i++) {
            if (tempDisplay[i] == '-') {
                isComplete = false;
                break;
            }
        }
        
        // 如果输入不完整，清空当前输入值，重新开始输入
        if (!isComplete) {
            // 重置所有位置为'-'
            for (int i = 0; i < 3; i++) {
                tempDisplay[i] = '-';
            }
            // 重置输入位置指针
            inputPos = 0;
            updateDisplay();
            return;
        }
        
        // 添加输入验证
        if (!tempDisplay || strlen(tempDisplay) > 3) {
            return;
        }
        
        // 使用更安全的转换方式
        char *endptr;
        long temp_val = strtol(tempDisplay, &endptr, 10);
        // 验证转换结果
        if (endptr == tempDisplay || *endptr != '\0') {
            return;
        }
        
        // 保存旧的温度值，用于判断是否需要保存
        uint16_t old_temp = SolderingTargetTemp;
        bool value_changed = false;
        
        SolderingTargetTemp = (int)temp_val;
        
        if (SolderingTargetTemp == 0) {
            lv_label_set_text(ui_SolderingTargetTemp, "000℃");
            value_changed = (old_temp != SolderingTargetTemp);
        }
        else if(SolderingTargetTemp > SolderingTargetTempMax) {
            SolderingTargetTemp = SolderingTargetTempMax;
            lv_label_set_text_fmt(ui_SolderingTargetTemp, "%.3d℃", SolderingTargetTemp);
            lv_label_set_text_fmt(ui_SolderingCurrentTemp, "%.3d℃", SolderingTargetTemp);
            lv_label_set_text(ui_SolderingSetTemp, "---℃");
            value_changed = (old_temp != SolderingTargetTemp);
        }
        else if(SolderingTargetTemp < SolderingTargetTempMin) {
            SolderingTargetTemp = SolderingTargetTempMin;
            lv_label_set_text_fmt(ui_SolderingTargetTemp, "%.3d℃", SolderingTargetTemp);
            lv_label_set_text_fmt(ui_SolderingCurrentTemp, "%.3d℃", SolderingTargetTemp);
            lv_label_set_text(ui_SolderingSetTemp, "---℃");
            value_changed = (old_temp != SolderingTargetTemp);
        }
        else {
            lv_label_set_text_fmt(ui_SolderingTargetTemp, "%.3d℃", SolderingTargetTemp);
            lv_label_set_text_fmt(ui_SolderingCurrentTemp, "%.3d℃", SolderingTargetTemp);
            value_changed = (old_temp != SolderingTargetTemp);
        }
        
        // 只有当数值实际发生变化时才保存到EEPROM
        if (value_changed) {
            WriteSoldering(); // 保存烙铁温度到EEPROM
        }
        
        // 确认后重置输入状态到初始状态
        for (int i = 0; i < 3; i++) {
            tempDisplay[i] = '-';
        }
        inputPos = 0;
        updateDisplay();
        
        // 切换回主屏幕
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_MainScreen_screen_init);
        lvgl_group_to_main();
    }
}

void ui_event_SolderingDelete( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        for(int i = 0; i < 3; i++) {
            tempDisplay[i] = '-';
        }
        inputPos = 0;
        SolderingTargetTemp = 0;
        updateDisplay();
    }
}

void ui_event_SolderingNum0( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('0');
    }
}

void ui_event_SolderingNum1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('1');
    }
}

void ui_event_SolderingNum2( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('2');
    }
}

void ui_event_SolderingNum3( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('3');
    }
}

void ui_event_SolderingNum4( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('4');
    }
}

void ui_event_SolderingNum5( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('5');
    }
}

void ui_event_SolderingNum6( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('6');
    }
}

void ui_event_SolderingNum7( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('7');
    }
}

void ui_event_SolderingNum8( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('8');
    }
}

void ui_event_SolderingNum9( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED) {
        handleNumberInput('9');
    }
}

void ui_event_SolderingTempSetBack(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_RELEASED) {
        // 重置所有位置为'-'
        for (int i = 0; i < 3; i++) {
            tempDisplay[i] = '-';
        }
        // 重置输入位置指针
        inputPos = 0;
        // 更新显示
        updateDisplay();
        
        // 切换回主屏幕
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_MainScreen_screen_init);
        lvgl_group_to_main();
    }
}