#ifndef _EVENT_SETTING_H
#define _EVENT_SETTING_H

#include <Arduino.h>
#include <lvgl.h>
#include <ui/ui.h>
#include <variables.h>
#include <lvgl_group.h>
#include <flash.h>
#include <PID_Service.h>

void Soldering_PID_Update();
void Soldering_PID_Init();
void Heatgun_PID_Update();
void Heatgun_PID_Init();

#endif // _EVENT_SETTING_H