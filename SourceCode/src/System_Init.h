#ifndef _SYSTEM_INIT_H
#define _SYSTEM_INIT_H

#include <Arduino.h>
#include <stdio.h> // 用于 sprintf
#include <lvgl.h>
#include <ui/ui.h>
#include <display.h>
#include <lvgl_group.h>

#include <INA226_Service.h>
#include <TMP102_Service.h>
#include <MAX6675_Service.h>
#include <Encoder_Service.h>
#include <GPIO_Service.h>
#include <PID_Service.h>

#include <variables.h>

void System_Init();

#endif