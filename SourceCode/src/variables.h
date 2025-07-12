#ifndef _VARIABLES_H
#define _VARIABLES_H

#include <Arduino.h>

extern String Version;

extern bool temp_read_mutex; // 互斥锁，防止同时读取温度

extern bool Soldering_Enabled;

extern uint16_t SolderingTargetTemp;
extern uint16_t SolderingTargetTempMin;
extern uint16_t SolderingTargetTempMax;
extern uint16_t SolderingStandbyTemp;
extern uint16_t SolderingStandbyTime;

extern uint16_t SolderingMaxPower;

extern bool Heatgun_Enabled;

extern uint16_t HeatgunTargetTemp;
extern uint16_t HeatgunTargetTempMin;
extern uint16_t HeatgunTargetTempMax;

extern uint16_t HeatgunWindSpeed;
extern uint16_t HeatgunWindSpeedMin;
extern uint16_t HeatgunWindSpeedMax;

extern uint8_t Brightness;

extern bool Buzzer_Enabled;
extern bool CoolingFan_Enabled;

extern uint8_t Soldering_Status;
extern uint16_t Soldering_Temp;
extern float Soldering_DutyCycle;
extern uint8_t Heatgun_Status;
extern uint16_t Heatgun_Temp;
extern float Heatgun_DutyCycle;

extern float Soldering_KP;
extern float Soldering_KI;
extern float Soldering_KD;

extern float Heatgun_KP;
extern float Heatgun_KI;
extern float Heatgun_KD;

#endif