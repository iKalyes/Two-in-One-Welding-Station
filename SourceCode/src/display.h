#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui/ui.h"
#include <RAK14014_FT6336U.h>
#include <hardware/pwm.h>
#include <event_main.h>
#include <event_pid.h>
#include <variables.h>
#include <GPIO_Service.h>


/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf_1[ screenWidth * screenHeight / 5 ];
static lv_color_t buf_2[ screenWidth * screenHeight / 5 ];

void display_init();
void backlight_init();
void backlight_refresh();
void lvgl_task_handler();

void MainScreen_init();
void SystemSettingScreen_init();
void PIDSettingScreen_init();

#endif