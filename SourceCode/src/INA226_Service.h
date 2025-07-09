#ifndef _INA226_SERVICE_H
#define _INA226_SERVICE_H

#include <Arduino.h>
#include <lvgl.h>
#include <ui/ui.h>
#include <INA226.h>

#define INA226_SCL 19
#define INA226_SDA 18

bool INA226_Init();
void INA226_Task(lv_timer_t *timer);

#endif