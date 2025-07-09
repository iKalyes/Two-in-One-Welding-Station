#ifndef NUMBERFLOW_H
#define NUMBERFLOW_H

#include "lvgl.h"
#include "ui/ui.h"

class DigitRoller {
private:
    struct DigitAnimData {
        lv_obj_t* digit_strip;
        int digit_index;
        DigitRoller* roller;
        lv_anim_t anim;  // 内置动画对象，避免重复创建
    };
    
    lv_obj_t* container;
    lv_obj_t* digit_containers[3];  // 3个数字位的容器（用于裁剪）
    lv_obj_t* digit_strips[3];      // 3个数字条（包含0-9的标签）
    lv_obj_t* digit_labels[3][10];  // 每个数字条包含10个数字标签
    int32_t current_digits[3];
    int32_t target_digits[3];
    DigitAnimData anim_data[3];
    
    // 布局参数
    int digit_height;               // 单个数字的高度
    int digit_width;                // 单个数字的宽度
    int digit_spacing;              // 数字之间的间距
    int container_padding;          // 容器内边距
    int digit_x_offset;             // 数字X轴偏移
    int digit_y_offset;             // 数字Y轴偏移
    
    static void digit_anim_cb(void* var, int32_t value);
    void extractDigits(int32_t value, int32_t* digits);
    void createDigitStrip(int digit_index);
    void updateLayout();            // 更新布局
    
public:
    DigitRoller(lv_obj_t* parent);
    void setValue(int32_t value);
    void setPosition(lv_coord_t x, lv_coord_t y);
    void setStyle(const lv_font_t* font, lv_color_t color);
    
    // 新增布局控制函数
    void setDigitSize(int width, int height);
    void setDigitSpacing(int spacing);
    void setContainerPadding(int padding);
    void setDigitOffset(int x_offset, int y_offset);
    void setAnimationTime(uint32_t time_ms);
    void setAnimationDelay(uint32_t delay_ms);
    
    lv_obj_t* getContainer();
};

#endif