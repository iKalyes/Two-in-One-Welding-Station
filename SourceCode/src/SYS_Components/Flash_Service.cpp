#include <Flash_Service.h>

namespace {

constexpr int kEepromSize = 256;

// Keep addresses unchanged to preserve compatibility with existing devices.
namespace EepromAddr {
constexpr int SolderingTargetTemp = 0;
constexpr int SolderingTargetTempMin = 2;
constexpr int SolderingTargetTempMax = 4;
constexpr int SolderingStandbyTemp = 6;
constexpr int SolderingStandbyTime = 8;

constexpr int HeatgunTargetTemp = 12;
constexpr int HeatgunTargetTempMin = 14;
constexpr int HeatgunTargetTempMax = 16;
constexpr int HeatgunWindSpeed = 18;

constexpr int Brightness = 24;

constexpr int SolderingKp = 26;
constexpr int SolderingKi = 30;
constexpr int SolderingKd = 34;
constexpr int HeatgunKp = 38;
constexpr int HeatgunKi = 42;
constexpr int HeatgunKd = 46;

constexpr int BuzzerEnabled = 50;
constexpr int CoolingFanEnabled = 51;
constexpr int DisplayType = 52;
constexpr int DisplayTypeConfigured = 53;
constexpr int SolderingMaxPower = 54;
} // namespace EepromAddr

constexpr float kPidScale = 1000.0f;

template <typename T>
void EepromPutValue(int address, const T& value) {
    EEPROM.put(address, value);
}

template <typename T>
void EepromGetValue(int address, T& value) {
    EEPROM.get(address, value);
}

void EepromWriteBool(int address, bool value) {
    EEPROM.write(address, value ? 1 : 0);
}

bool EepromReadBool(int address, bool defaultValue) {
    const uint8_t rawValue = EEPROM.read(address);
    if (rawValue > 1) {
        return defaultValue;
    }
    return rawValue == 1;
}

bool IsOutOfRange(uint16_t value, uint16_t minValue, uint16_t maxValue) {
    return value < minValue || value > maxValue;
}

uint16_t ReadUint16WithRange(int address, uint16_t minValue, uint16_t maxValue, uint16_t defaultValue) {
    uint16_t value = 0;
    EepromGetValue(address, value);
    return IsOutOfRange(value, minValue, maxValue) ? defaultValue : value;
}

uint16_t ReadUint16WithDynamicMax(int address, uint16_t minValue, uint16_t dynamicMax, uint16_t defaultValue) {
    uint16_t value = 0;
    EepromGetValue(address, value);
    return IsOutOfRange(value, minValue, dynamicMax) ? defaultValue : value;
}

uint16_t ReadSolderingTargetTemp() {
    uint16_t value = 0;
    EepromGetValue(EepromAddr::SolderingTargetTemp, value);
    if (IsOutOfRange(value, SolderingTargetTempMin, SolderingTargetTempMax)) {
        return 100;
    }
    return value;
}

uint16_t ReadHeatgunTargetTemp() {
    uint16_t value = 0;
    EepromGetValue(EepromAddr::HeatgunTargetTemp, value);
    if (IsOutOfRange(value, HeatgunTargetTempMin, HeatgunTargetTempMax)) {
        return 100;
    }
    return value;
}

uint32_t FloatToScaledPid(float value) {
    return static_cast<uint32_t>(value * kPidScale + 0.5f);
}

float ScaledPidToFloat(uint32_t value) {
    return static_cast<float>(value) / kPidScale;
}

uint32_t ReadPidValue(int address, uint32_t defaultValue) {
    uint32_t value = 0;
    EepromGetValue(address, value);
    return value == 0 ? defaultValue : value;
}

} // namespace

void WriteFlash() {
    EEPROM.begin(kEepromSize);

    EepromPutValue(EepromAddr::SolderingTargetTempMin, SolderingTargetTempMin);
    EepromPutValue(EepromAddr::SolderingTargetTempMax, SolderingTargetTempMax);
    EepromPutValue(EepromAddr::SolderingStandbyTemp, SolderingStandbyTemp);
    EepromPutValue(EepromAddr::SolderingStandbyTime, SolderingStandbyTime);
    EepromPutValue(EepromAddr::SolderingMaxPower, SolderingMaxPower);

    EepromPutValue(EepromAddr::HeatgunTargetTempMin, HeatgunTargetTempMin);
    EepromPutValue(EepromAddr::HeatgunTargetTempMax, HeatgunTargetTempMax);

    EEPROM.write(EepromAddr::Brightness, Brightness);
    EepromWriteBool(EepromAddr::BuzzerEnabled, Buzzer_Enabled);
    EepromWriteBool(EepromAddr::CoolingFanEnabled, CoolingFan_Enabled);

    EEPROM.end();
}

void WriteDisplayType() {
    EEPROM.begin(kEepromSize);

    EepromWriteBool(EepromAddr::DisplayType, Display_Type);
    EepromWriteBool(EepromAddr::DisplayTypeConfigured, Display_Type_Configured);

    EEPROM.end();
}

void WriteSoldering() {
    EEPROM.begin(kEepromSize);

    EepromPutValue(EepromAddr::SolderingTargetTemp, SolderingTargetTemp);

    EEPROM.end();
}

void WriteHeatgun() {
    EEPROM.begin(kEepromSize);

    EepromPutValue(EepromAddr::HeatgunTargetTemp, HeatgunTargetTemp);
    EepromPutValue(EepromAddr::HeatgunWindSpeed, HeatgunWindSpeed);

    EEPROM.end();
}

void WritePID() {
    EEPROM.begin(kEepromSize);

    EepromPutValue(EepromAddr::SolderingKp, FloatToScaledPid(Soldering_KP));
    EepromPutValue(EepromAddr::SolderingKi, FloatToScaledPid(Soldering_KI));
    EepromPutValue(EepromAddr::SolderingKd, FloatToScaledPid(Soldering_KD));

    EepromPutValue(EepromAddr::HeatgunKp, FloatToScaledPid(Heatgun_KP));
    EepromPutValue(EepromAddr::HeatgunKi, FloatToScaledPid(Heatgun_KI));
    EepromPutValue(EepromAddr::HeatgunKd, FloatToScaledPid(Heatgun_KD));

    EEPROM.end();
}

void ReadFlash() {
    EEPROM.begin(kEepromSize);

    // Soldering profile
    SolderingTargetTempMin = ReadUint16WithRange(EepromAddr::SolderingTargetTempMin, 50, 100, 50);
    SolderingTargetTempMax = ReadUint16WithRange(EepromAddr::SolderingTargetTempMax, 100, 450, 400);
    SolderingTargetTemp = ReadSolderingTargetTemp();
    SolderingStandbyTemp = ReadUint16WithDynamicMax(EepromAddr::SolderingStandbyTemp, 50, SolderingTargetTempMax, 100);
    SolderingStandbyTime = ReadUint16WithRange(EepromAddr::SolderingStandbyTime, 0, 60, 10);
    SolderingMaxPower = ReadUint16WithRange(EepromAddr::SolderingMaxPower, 20, 100, 100);

    // Heatgun profile
    HeatgunTargetTempMin = ReadUint16WithRange(EepromAddr::HeatgunTargetTempMin, 50, 100, 100);
    HeatgunTargetTempMax = ReadUint16WithRange(EepromAddr::HeatgunTargetTempMax, 100, 450, 400);
    HeatgunTargetTemp = ReadHeatgunTargetTemp();
    HeatgunWindSpeed = ReadUint16WithRange(EepromAddr::HeatgunWindSpeed, HeatgunWindSpeedMin, HeatgunWindSpeedMax, 50);

    // System settings
    Brightness = EEPROM.read(EepromAddr::Brightness);
    if (Brightness < 10 || Brightness > 100) {
        Brightness = 50;
    }

    Buzzer_Enabled = EepromReadBool(EepromAddr::BuzzerEnabled, true);
    CoolingFan_Enabled = EepromReadBool(EepromAddr::CoolingFanEnabled, true);
    Display_Type = EepromReadBool(EepromAddr::DisplayType, true);
    Display_Type_Configured = EepromReadBool(EepromAddr::DisplayTypeConfigured, false);

    // PID parameters
    const uint32_t solderingKp = ReadPidValue(EepromAddr::SolderingKp, 18000);
    const uint32_t solderingKi = ReadPidValue(EepromAddr::SolderingKi, 12000);
    const uint32_t solderingKd = ReadPidValue(EepromAddr::SolderingKd, 2000);

    const uint32_t heatgunKp = ReadPidValue(EepromAddr::HeatgunKp, 58000);
    const uint32_t heatgunKi = ReadPidValue(EepromAddr::HeatgunKi, 14000);
    const uint32_t heatgunKd = ReadPidValue(EepromAddr::HeatgunKd, 20000);

    Soldering_KP = ScaledPidToFloat(solderingKp);
    Soldering_KI = ScaledPidToFloat(solderingKi);
    Soldering_KD = ScaledPidToFloat(solderingKd);

    Heatgun_KP = ScaledPidToFloat(heatgunKp);
    Heatgun_KI = ScaledPidToFloat(heatgunKi);
    Heatgun_KD = ScaledPidToFloat(heatgunKd);

    EEPROM.end();
}

void ClearFlash() {
    EEPROM.begin(kEepromSize);
    for (int i = 0; i < kEepromSize; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.end();
}