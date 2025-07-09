#ifndef _GPIO_SERVICE_H
#define _GPIO_SERVICE_H

#include <Arduino.h>
#include <variables.h>
#include <hardware/pwm.h>

#define Soldering_SLEEP 28
#define Soldering_ID 29
#define Soldering_PWM 24 //PWM4 A

#define Heatgun_SLEEP 22
#define Heatgun_FAN_PWM 23 //PWM3 B - 与屏幕背光共用PWM3，频率必须相同
#define Heatgun_PWM 26 //PWM5 A

#define Cooling_FAN_PWM 20 //PWM2 A
#define Buzzer_PWM 13 //PWM6 B

//Display Backlight PWM3 A

void Soldering_GPIO_Init();
uint16_t Soldering_Read_ID();
bool Soldering_Read_SLEEP();
void Soldering_Set_PWM(float value);

void Heatgun_GPIO_Init();
bool Heatgun_Read_SLEEP();
void Heatgun_Set_PWM(float value);
void Heatgun_Set_FAN_PWM(float value);

void Cooling_FAN_GPIO_Init();
void Cooling_FAN_Set_PWM(float value);

void Buzzer_GPIO_Init();
void Buzzer_Set_PWM(float value);
void Buzzer_ON();
void Buzzer_OFF();

#endif