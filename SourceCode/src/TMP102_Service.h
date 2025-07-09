#ifndef _TMP102_SERVICE_H
#define _TMP102_SERVICE_H

#include <Arduino.h>
#include <lvgl.h>
#include <ui/ui.h>
#include <SparkFunTMP102.h>
#include <GPIO_Service.h>
#include <variables.h>

#define TMP102_SCL 19
#define TMP102_SDA 18

bool TMP102_Init();
void TMP102_Task(lv_timer_t *timer);

#endif