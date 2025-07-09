#ifndef _MAX6675_SERVICE_H
#define _MAX6675_SERVICE_H

#include <Arduino.h>
#include <MAX6675.h>
#include <lvgl.h>
#include <ui/ui.h>
#include <variables.h>
#include <numberflow.h>

extern DigitRoller* soldering_temp_display;
extern DigitRoller* heatgun_temp_display;

#define MAX6675_CS1_PIN 11
#define MAX6675_CS2_PIN 21
#define MAX6675_SO_PIN 12
#define MAX6675_CLK_PIN 14

void MAX6675_Init();
void MAX6675_Read_Task();
void MAX6675_Display_Task(lv_timer_t *timer);

void MAX6675_Soldering_Numberflow();
void MAX6675_Soldering_Normal();
void MAX6675_Heatgun_Numberflow();
void MAX6675_Heatgun_Normal();

uint8_t MAX6675_Read_Soldering_Status();
float MAX6675_Read_Soldering_Temperature();
uint8_t MAX6675_Read_Heatgun_Status();
float MAX6675_Read_Heatgun_Temperature();

#endif // _MAX6675_SERVICE_H