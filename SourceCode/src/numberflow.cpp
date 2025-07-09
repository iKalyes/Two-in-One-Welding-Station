#include "numberflow.h"

DigitRoller::DigitRoller(lv_obj_t* parent) {
    // 初始化布局参数
    digit_height = 88;      // 单个数字的高度
    digit_width = 44;       // 单个数字的宽度
    digit_spacing = 0;      // 数字之间的间距
    container_padding = 10;  // 容器内边距
    digit_x_offset = -26;     // 数字X轴偏移
    digit_y_offset = -26;     // 数字Y轴偏移
    
    // 创建主容器 - 宽度根据数字宽度和间距计算
    int container_width = digit_width * 3 + digit_spacing * 2 + container_padding * 2;
    container = lv_obj_create(parent);
    lv_obj_set_size(container, container_width, digit_height + container_padding * 2);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(container, container_padding, 0);
    
    // 创建3个数字位
    for(int i = 0; i < 3; i++) {
        // 创建数字位容器（用于裁剪显示区域）
        digit_containers[i] = lv_obj_create(container);
        lv_obj_set_size(digit_containers[i], digit_width, digit_height);
        lv_obj_clear_flag(digit_containers[i], LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_opa(digit_containers[i], LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(digit_containers[i], LV_OPA_TRANSP, 0);
        lv_obj_set_style_pad_all(digit_containers[i], 0, 0);
        lv_obj_set_style_clip_corner(digit_containers[i], true, 0); // 启用裁剪
        
        // 设置位置（手动布局以获得更精确的控制）
        int x_pos = container_padding + i * (digit_width + digit_spacing) + digit_x_offset;
        int y_pos = container_padding + digit_y_offset;
        lv_obj_set_pos(digit_containers[i], x_pos, y_pos);
        
        // 优化：减少重绘次数
        lv_obj_add_flag(digit_containers[i], LV_OBJ_FLAG_OVERFLOW_VISIBLE);
        
        // 创建数字条
        createDigitStrip(i);
        
        // 初始化数据
        current_digits[i] = 0;
        target_digits[i] = 0;
        
        // 设置动画数据
        anim_data[i].digit_strip = digit_strips[i];
        anim_data[i].digit_index = i;
        anim_data[i].roller = this;
    }
}

void DigitRoller::createDigitStrip(int digit_index) {
    // 创建数字条（包含0-9的垂直列表）
    digit_strips[digit_index] = lv_obj_create(digit_containers[digit_index]);
    lv_obj_set_size(digit_strips[digit_index], digit_width, digit_height * 10);
    lv_obj_clear_flag(digit_strips[digit_index], LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(digit_strips[digit_index], LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(digit_strips[digit_index], LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(digit_strips[digit_index], 0, 0);
    lv_obj_set_pos(digit_strips[digit_index], 0, 0); // 初始位置显示数字0
    
    // 优化：预分配所有标签，减少运行时内存分配
    for(int j = 0; j < 10; j++) {
        digit_labels[digit_index][j] = lv_label_create(digit_strips[digit_index]);
        lv_label_set_text_fmt(digit_labels[digit_index][j], "%d", j);
        lv_obj_set_style_text_font(digit_labels[digit_index][j], &ui_font_ASCII88, 0);
        lv_obj_set_style_text_color(digit_labels[digit_index][j], lv_color_white(), 0);
        lv_obj_set_style_text_align(digit_labels[digit_index][j], LV_TEXT_ALIGN_CENTER, 0);
        
        // 设置每个数字的位置和大小
        lv_obj_set_pos(digit_labels[digit_index][j], 0, j * digit_height);
        lv_obj_set_size(digit_labels[digit_index][j], digit_width, digit_height);
        
        // 优化：减少标签的重绘
        lv_obj_add_flag(digit_labels[digit_index][j], LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    }
}

void DigitRoller::updateLayout() {
    // 更新容器大小
    int container_width = digit_width * 3 + digit_spacing * 2 + container_padding * 2;
    lv_obj_set_size(container, container_width, digit_height + container_padding * 2);
    lv_obj_set_style_pad_all(container, container_padding, 0);
    
    // 更新每个数字位的位置和大小
    for(int i = 0; i < 3; i++) {
        // 更新数字位容器大小和位置
        lv_obj_set_size(digit_containers[i], digit_width, digit_height);
        int x_pos = container_padding + i * (digit_width + digit_spacing) + digit_x_offset;
        int y_pos = container_padding + digit_y_offset;
        lv_obj_set_pos(digit_containers[i], x_pos, y_pos);
        
        // 更新数字条大小
        lv_obj_set_size(digit_strips[i], digit_width, digit_height * 10);
        
        // 更新所有数字标签的大小和位置
        for(int j = 0; j < 10; j++) {
            lv_obj_set_pos(digit_labels[i][j], 0, j * digit_height);
            lv_obj_set_size(digit_labels[i][j], digit_width, digit_height);
        }
    }
}

void DigitRoller::digit_anim_cb(void* var, int32_t value) {
    DigitAnimData* data = (DigitAnimData*)var;
    
    // 优化：直接设置Y坐标，避免触发不必要的布局计算
    lv_obj_set_y(data->digit_strip, value);
    
    // 优化：只标记需要重绘的区域
    lv_obj_invalidate(data->digit_strip);
}

void DigitRoller::extractDigits(int32_t value, int32_t* digits) {
    // 确保值在0-999范围内
    value = LV_MAX(0, LV_MIN(999, value));
    
    digits[0] = value / 100;          // 百位
    digits[1] = (value / 10) % 10;    // 十位
    digits[2] = value % 10;           // 个位
}

void DigitRoller::setValue(int32_t value) {
    int32_t new_digits[3];
    extractDigits(value, new_digits);
    
    // 优化：批量处理动画，减少动画对象创建
    int anim_count = 0;
    
    // 为每一位创建动画
    for(int i = 0; i < 3; i++) {
        if(current_digits[i] != new_digits[i]) {
            // 计算起始和目标Y坐标
            int32_t start_y = -current_digits[i] * digit_height;
            int32_t target_y = -new_digits[i] * digit_height;
            
            // 保存目标值
            target_digits[i] = new_digits[i];
            
            // 优化：重用动画对象
            lv_anim_t* anim = &anim_data[i].anim;
            lv_anim_init(anim);
            lv_anim_set_var(anim, &anim_data[i]);
            lv_anim_set_exec_cb(anim, digit_anim_cb);
            
            // 设置动画：从当前Y坐标滚动到目标Y坐标
            lv_anim_set_values(anim, start_y, target_y);
            lv_anim_set_time(anim, 200); // 200ms动画时间
            
            // 使用先快后慢的缓动函数
            lv_anim_set_path_cb(anim, lv_anim_path_ease_out);
            
            lv_anim_start(anim);
            anim_count++;
            
        } else {
            // 如果数字没有变化，确保位置正确
            int32_t correct_y = -new_digits[i] * digit_height;
            lv_obj_set_y(digit_strips[i], correct_y);
            target_digits[i] = new_digits[i];
        }
    }
    
    // 更新当前数字
    for(int i = 0; i < 3; i++) {
        current_digits[i] = new_digits[i];
    }
}

void DigitRoller::setPosition(lv_coord_t x, lv_coord_t y) {
    lv_obj_set_pos(container, x, y);
}

void DigitRoller::setStyle(const lv_font_t* font, lv_color_t color) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 10; j++) {
            lv_obj_set_style_text_font(digit_labels[i][j], font, 0);
            lv_obj_set_style_text_color(digit_labels[i][j], color, 0);
        }
    }
}

// 新增布局控制函数
void DigitRoller::setDigitSize(int width, int height) {
    digit_width = width;
    digit_height = height;
    updateLayout();
}

void DigitRoller::setDigitSpacing(int spacing) {
    digit_spacing = spacing;
    updateLayout();
}

void DigitRoller::setContainerPadding(int padding) {
    container_padding = padding;
    updateLayout();
}

void DigitRoller::setDigitOffset(int x_offset, int y_offset) {
    digit_x_offset = x_offset;
    digit_y_offset = y_offset;
    updateLayout();
}

void DigitRoller::setAnimationTime(uint32_t time_ms) {
    // 这个函数会在下次setValue时生效
    // 可以添加一个成员变量来存储动画时间
}

void DigitRoller::setAnimationDelay(uint32_t delay_ms) {
    // 这个函数会在下次setValue时生效
    // 可以添加一个成员变量来存储动画延迟
}

lv_obj_t* DigitRoller::getContainer() {
    return container;
}