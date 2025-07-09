#include <flash.h>

void WriteFlash() {
    EEPROM.begin(256);

    EEPROM.put(2, SolderingTargetTempMin);
    EEPROM.put(4, SolderingTargetTempMax);
    EEPROM.put(6, SolderingStandbyTemp);
    EEPROM.put(8, SolderingStandbyTime);

    EEPROM.put(14, HeatgunTargetTempMin);
    EEPROM.put(16, HeatgunTargetTempMax);

    EEPROM.write(24, Brightness);

    EEPROM.write(50, Buzzer_Enabled ? 1 : 0);
    EEPROM.write(51, CoolingFan_Enabled ? 1 : 0);

    EEPROM.end();
}

void WriteSoldering()
{
    EEPROM.begin(256);

    EEPROM.put(0, SolderingTargetTemp);

    EEPROM.end();
}

void WriteHeatgun() {
    EEPROM.begin(256);

    EEPROM.put(12, HeatgunTargetTemp);
    EEPROM.put(18, HeatgunWindSpeed);

    EEPROM.end();
}

void WritePID() {
    EEPROM.begin(256);

    uint32_t SolderingKp = (uint32_t)(Soldering_KP * 1000.0f + 0.5f); 
    uint32_t SolderingKi = (uint32_t)(Soldering_KI * 1000.0f + 0.5f); 
    uint32_t SolderingKd = (uint32_t)(Soldering_KD * 1000.0f + 0.5f); 

    uint32_t HeatgunKp = (uint32_t)(Heatgun_KP * 1000.0f + 0.5f);
    uint32_t HeatgunKi = (uint32_t)(Heatgun_KI * 1000.0f + 0.5f);
    uint32_t HeatgunKd = (uint32_t)(Heatgun_KD * 1000.0f + 0.5f);

    EEPROM.put(26, SolderingKp);
    EEPROM.put(30, SolderingKi);
    EEPROM.put(34, SolderingKd);

    EEPROM.put(38, HeatgunKp);
    EEPROM.put(42, HeatgunKi);
    EEPROM.put(46, HeatgunKd);

    EEPROM.end();
}

//—————————————————————————————————————————//
// Read from flash memory

void ReadFlash() {
    EEPROM.begin(256);

    EEPROM.get(2, SolderingTargetTempMin);
    if(SolderingTargetTempMin > 100 || SolderingTargetTempMin < 50) {
        SolderingTargetTempMin = 50; // Ensure minimum value
    }
    EEPROM.get(4, SolderingTargetTempMax);
    if(SolderingTargetTempMax > 450 || SolderingTargetTempMax < 100) {
        SolderingTargetTempMax = 400; // Ensure maximum value
    }
    EEPROM.get(0, SolderingTargetTemp);
    if(SolderingTargetTemp < SolderingTargetTempMin || SolderingTargetTemp > SolderingTargetTempMax) {
        SolderingTargetTemp = 100; // Default value
    }
    EEPROM.get(6, SolderingStandbyTemp);
    if(SolderingStandbyTemp < 50 || SolderingStandbyTemp > SolderingTargetTempMax) {
        SolderingStandbyTemp = 100; // Default value
    }
    EEPROM.get(8, SolderingStandbyTime);
    if(SolderingStandbyTime < 0 || SolderingStandbyTime > 60) {
        SolderingStandbyTime = 10; // Default value
    }

    EEPROM.get(14, HeatgunTargetTempMin);
    if(HeatgunTargetTempMin > 100 || HeatgunTargetTempMin < 50) {
        HeatgunTargetTempMin = 100; // Ensure minimum value
    }
    EEPROM.get(16, HeatgunTargetTempMax);
    if(HeatgunTargetTempMax > 450 || HeatgunTargetTempMax < 100) {
        HeatgunTargetTempMax = 400; // Ensure maximum value
    }
    EEPROM.get(12, HeatgunTargetTemp);
    if(HeatgunTargetTemp < HeatgunTargetTempMin || HeatgunTargetTemp > HeatgunTargetTempMax) {
        HeatgunTargetTemp = 100; // Default value
    }

    EEPROM.get(18, HeatgunWindSpeed);
    if(HeatgunWindSpeed < HeatgunWindSpeedMin || HeatgunWindSpeed > HeatgunWindSpeedMax) {
        HeatgunWindSpeed = 50; // Default value
    }

    Brightness = EEPROM.read(24);
    if(Brightness < 10 || Brightness > 100) {
        Brightness = 50; // Default value
    }

    Buzzer_Enabled = EEPROM.read(50);
    if(Buzzer_Enabled > 1) {
        Buzzer_Enabled = true; // Default value
    } else {
        Buzzer_Enabled = (Buzzer_Enabled == 1);
    }

    CoolingFan_Enabled = EEPROM.read(51);
    if(CoolingFan_Enabled > 1) {
        CoolingFan_Enabled = true; // Default value
    } else {
        CoolingFan_Enabled = (CoolingFan_Enabled == 1);
    }

    EEPROM.end();
}

void ReadPID() {
    EEPROM.begin(256);

    uint32_t SolderingKp, SolderingKi, SolderingKd;
    uint32_t HeatgunKp, HeatgunKi, HeatgunKd;

    EEPROM.get(26, SolderingKp);
    if(SolderingKp == 0) {
        SolderingKp = 20000; // Default value
    }
    EEPROM.get(30, SolderingKi);
    if(SolderingKi == 0) {
        SolderingKi = 5000; // Default value
    }
    EEPROM.get(34, SolderingKd);
    if(SolderingKd == 0) {
        SolderingKd = 100; // Default value
    }

    EEPROM.get(38, HeatgunKp);
    if(HeatgunKp == 0) {
        HeatgunKp = 60000; // Default value
    }
    EEPROM.get(42, HeatgunKi);
    if(HeatgunKi == 0) {
        HeatgunKi = 3000; // Default value
    }
    EEPROM.get(46, HeatgunKd);
    if(HeatgunKd == 0) {
        HeatgunKd = 1000; // Default value
    }

    Soldering_KP = (float)SolderingKp / 1000.0f;
    Soldering_KI = (float)SolderingKi / 1000.0f;
    Soldering_KD = (float)SolderingKd / 1000.0f;

    Heatgun_KP = (float)HeatgunKp / 1000.0f;
    Heatgun_KI = (float)HeatgunKi / 1000.0f;
    Heatgun_KD = (float)HeatgunKd / 1000.0f;

    EEPROM.end();
}

void ClearFlash(){
    EEPROM.begin(256);
    for (int i = 0; i < 256; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.end();
}