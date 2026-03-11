#ifndef _VARIABLES_H
#define _VARIABLES_H

#include <Arduino.h>

extern String Version;

extern bool Display_Type;
extern bool Display_Type_Configured;

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

// PID占空比平滑参数（误差阈值，单位: 摄氏度）
extern uint16_t DutySmoothErrHigh;
extern uint16_t DutySmoothErrMid;
extern uint16_t DutySmoothErrLow;

// PID占空比平滑参数（EMA系数，范围: 0.0~1.0）
extern float DutySmoothAlphaHigh;
extern float DutySmoothAlphaMid;
extern float DutySmoothAlphaLow;
extern float DutySmoothAlphaBase;

// 电烙铁占空比斜率限制（每次PID计算允许变化的最大占空比）
extern float SolderingDutyStepUpHighErr;
extern float SolderingDutyStepUpMidErr;
extern float SolderingDutyStepUpLowErr;
extern float SolderingDutyStepUpBase;
extern float SolderingDutyStepDownHighErr;
extern float SolderingDutyStepDownMidErr;
extern float SolderingDutyStepDownLowErr;
extern float SolderingDutyStepDownBase;

#endif