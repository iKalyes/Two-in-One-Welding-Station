#ifndef _SYSTEM_INIT_H
#define _SYSTEM_INIT_H

#include <Arduino.h>
#include <stdio.h> // 用于 sprintf
#include <lvgl.h>
#include <ui/ui.h>

#include <Display.h>
#include <LVGL_Group.h>

#include <Flash_Service.h>
#include <INA226_Service.h>
#include <TMP102_Service.h>
#include <MAX6675_Service.h>
#include <Encoder_Service.h>
#include <GPIO_Service.h>
#include <PID_Service.h>

#include <Variables.h>

// FreeRTOS 统一包含
#include <FreeRTOS.h>
#include <task.h>

void System_Init();

#endif