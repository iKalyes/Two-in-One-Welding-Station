#ifndef _EVENT_PID_H
#define _EVENT_PID_H

#include <Arduino.h>
#include <lvgl.h>
#include <ui/ui.h>

#include <LVGL_Group.h>
#include <Variables.h>

#include <Flash_Service.h>
#include <PID_Service.h>

void Soldering_PID_Update();
void Soldering_PID_Init();
void Heatgun_PID_Update();
void Heatgun_PID_Init();

#endif // _EVENT_PID_H