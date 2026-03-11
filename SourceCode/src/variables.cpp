#include <Variables.h> 

String Version = "Ver: 1.5 - 250311 MAX6675 RTOS"; // 固件版本号

bool Display_Type = true;             // true: IPS屏幕；false: TFT屏幕
bool Display_Type_Configured = false; // 是否已配置显示类型

bool Soldering_Enabled = false;

uint16_t SolderingTargetTemp = 100;
uint16_t SolderingTargetTempMin = 50;
uint16_t SolderingTargetTempMax = 400;
uint16_t SolderingStandbyTemp = 100;
uint16_t SolderingStandbyTime = 10; // Minutes

uint16_t SolderingMaxPower = 100; // Max power in percentage (20-100)

bool Heatgun_Enabled = false;

uint16_t HeatgunTargetTemp = 100;
uint16_t HeatgunTargetTempMin = 100;
uint16_t HeatgunTargetTempMax = 400;

uint16_t HeatgunWindSpeed = 50;
uint16_t HeatgunWindSpeedMin = 10;
uint16_t HeatgunWindSpeedMax = 100;

uint8_t Brightness;

bool Buzzer_Enabled = true;
bool CoolingFan_Enabled = true;

uint8_t Soldering_Status;
uint16_t Soldering_Temp;
float Soldering_DutyCycle;
uint8_t Heatgun_Status;
uint16_t Heatgun_Temp;
float Heatgun_DutyCycle;

//———— Soldering PID For Universual JBC245 ——————//

float Soldering_KP = 16.0f;
float Soldering_KI = 9.0f;
float Soldering_KD = 5.0f;

//———— Heatgun PID For Universual 858D ——————//

float Heatgun_KP = 58.0f;
float Heatgun_KI = 14.0f;
float Heatgun_KD = 20.0f;

// PID占空比平滑参数（误差阈值，单位: 摄氏度）
uint16_t DutySmoothErrHigh = 120;
uint16_t DutySmoothErrMid = 70;
uint16_t DutySmoothErrLow = 35;

// PID占空比平滑参数（EMA系数，误差越大系数越大，响应越快）
float DutySmoothAlphaHigh = 0.62f;
float DutySmoothAlphaMid = 0.46f;
float DutySmoothAlphaLow = 0.30f;
float DutySmoothAlphaBase = 0.18f;

// 电烙铁占空比斜率限制（升功率）
float SolderingDutyStepUpHighErr = 20.0f;
float SolderingDutyStepUpMidErr = 14.0f;
float SolderingDutyStepUpLowErr = 8.0f;
float SolderingDutyStepUpBase = 4.0f;

// 电烙铁占空比斜率限制（降功率）
float SolderingDutyStepDownHighErr = 36.0f;
float SolderingDutyStepDownMidErr = 24.0f;
float SolderingDutyStepDownLowErr = 15.0f;
float SolderingDutyStepDownBase = 10.0f;

