#ifndef _FLASH_H_
#define _FLASH_H_ 

#include <Arduino.h>
#include <variables.h>
#include <EEPROM.h>

void WriteFlash();
void WriteSoldering();
void WriteHeatgun();
void WritePID();
void ReadFlash();
void ReadPID();
void ClearFlash();

#endif