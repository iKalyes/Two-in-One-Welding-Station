#ifndef _ENCODER_SERVICE_H
#define _ENCODER_SERVICE_H

#include <Arduino.h>
#include <RotaryEncoder.h>
#include <lvgl.h>
#include "ui/ui.h"
#include <GPIO_Service.h>  // 确保包含蜂鸣器控制函数

void encoder_tick();  // Interrupt Service Routine for the encoder
void encoder_init();
void encoder_lvgl_init();
lv_indev_t* get_encoder_indev();
int get_encoder_position();

#endif