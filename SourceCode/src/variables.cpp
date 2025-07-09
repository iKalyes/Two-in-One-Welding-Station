#include <variables.h> 

bool Soldering_Enabled = false;

uint16_t SolderingTargetTemp = 100;
uint16_t SolderingTargetTempMin = 50;
uint16_t SolderingTargetTempMax = 400;
uint16_t SolderingStandbyTemp = 100;
uint16_t SolderingStandbyTime = 10; // Minutes

uint16_t SolderingMaxPower = 100; // Max power in percentage (0-100)

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

float Soldering_KP = 20.0f;
float Soldering_KI = 0.5f;
float Soldering_KD = 0.1f;

//———— Heatgun PID For Universual 858D ——————//

float Heatgun_KP = 60.0f;
float Heatgun_KI = 3.0f;
float Heatgun_KD = 100.0f;
