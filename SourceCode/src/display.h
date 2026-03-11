#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <lvgl.h>
#include "ui/ui.h"

#include <FT6336U.h>
#include <TFT_eSPI.h>

#include <Event_MAIN.h>
#include <Event_PID.h>
#include <Variables.h>

// FreeRTOS 头文件放在此，供实现文件统一使用
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

// Touch screen pin configuration
#define TOUCH_INT_PIN 7
#define TOUCH_SDA_PIN 8
#define TOUCH_SCL_PIN 9
#define TOUCH_RST_PIN 10

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf_1[ screenWidth * screenHeight / 5 ];
static lv_color_t buf_2[ screenWidth * screenHeight / 5 ];

// LVGL Tick 软件定时器句柄对外声明（定义在 Display.cpp）
extern TimerHandle_t s_lvglTickTimer;

void display_init();
void lvgl_task_handler();
void ChangeDisplayType(bool Type);

void MainScreen_init();
void SystemSettingScreen_init();
void PIDSettingScreen_init();

#endif