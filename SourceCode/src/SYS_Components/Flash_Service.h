#ifndef _FLASH_H_
#define _FLASH_H_ 

#include <Arduino.h>
#include <EEPROM.h>

#include <Variables.h>

void WriteFlash();
void WriteDisplayType();
void WriteSoldering();
void WriteHeatgun();
void WritePID();

void ReadFlash();

void ClearFlash();

#endif