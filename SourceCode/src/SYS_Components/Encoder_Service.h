#ifndef _ENCODER_SERVICE_H
#define _ENCODER_SERVICE_H

#include <Arduino.h>
#include <lvgl.h>
#include "ui/ui.h"

#include <RotaryEncoder.h>
#include <GPIO_Service.h>  // 确保包含蜂鸣器控制函数

#define ENCODER_PIN_SW 15 // 编码器按键引脚
#define ENCODER_PIN_A 16  // 编码器引脚 A
#define ENCODER_PIN_B 17  // 编码器引脚 B

void encoder_tick();  // Interrupt Service Routine for the encoder
void encoder_init();
void encoder_lvgl_init();
lv_indev_t* get_encoder_indev();
int get_encoder_position();

#endif