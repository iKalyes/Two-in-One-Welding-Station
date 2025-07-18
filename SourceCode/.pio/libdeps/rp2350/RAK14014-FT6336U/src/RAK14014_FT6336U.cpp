/**************************************************************************/
/*!
  @file     FT6336U.cpp
  Author: Atsushi Sasaki (https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#include "RAK14014_FT6336U.h"

static uint8_t intSattus = false; // TP interrupt generation flag.

void tpIntHandle();

FT6336U::FT6336U(byte addr) 
{
  _deviceAddress = addr;
}

bool FT6336U::begin(TwoWire &wirePort)
{
  _deviceAddress = I2C_ADDR_FT6336U;
  _i2cPort = &wirePort;
  _i2cPort->setSDA(8);
  _i2cPort->setSCL(9);
  _i2cPort->begin(); 
  pinMode(7, INPUT_PULLUP);
  if(read_device_type() == 0x02)
  {
    writeByte(0x02, 0x01);
    attachInterrupt(digitalPinToInterrupt(7), tpIntHandle, FALLING);
    return true;
  }
  return false;
}

void tpIntHandle(void)
{
  intSattus = true;
}

bool FT6336U::available(void)
{
    if(intSattus)
    {
        scan();
        intSattus = false;
        return true;
    }
    return false;
}

uint8_t FT6336U::read_device_type(void) 
{
    return readByte(FT6336U_ADDR_CIPHER_LOW);
}

uint8_t FT6336U::read_device_mode(void) 
{
    return (readByte(FT6336U_ADDR_DEVICE_MODE) & 0x70) >> 4;
}
void FT6336U::write_device_mode(DEVICE_MODE_Enum mode) 
{
    writeByte(FT6336U_ADDR_DEVICE_MODE, (mode & 0x07) << 4);
}
uint8_t FT6336U::read_gesture_id(void) 
{
    return readByte(FT6336U_ADDR_GESTURE_ID);
}
uint8_t FT6336U::read_td_status(void) 
{
    return readByte(FT6336U_ADDR_TD_STATUS);
}
uint8_t FT6336U::read_touch_number(void) 
{
    return readByte(FT6336U_ADDR_TD_STATUS) & 0x0F;
}
// Touch 1 functions
uint16_t FT6336U::read_touch1_x(void) 
{
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_X);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_X + 1);
		return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint16_t FT6336U::read_touch1_y(void) 
{
		uint8_t read_buf[2];
		read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_Y);
		read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_Y + 1);
		return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t FT6336U::read_touch1_event(void) 
{
    return readByte(FT6336U_ADDR_TOUCH1_EVENT) >> 6;
}
uint8_t FT6336U::read_touch1_id(void) 
{
    return readByte(FT6336U_ADDR_TOUCH1_ID) >> 4;
}
uint8_t FT6336U::read_touch1_weight(void) 
{
    return readByte(FT6336U_ADDR_TOUCH1_WEIGHT);
}
uint8_t FT6336U::read_touch1_misc(void) 
{
    return readByte(FT6336U_ADDR_TOUCH1_MISC) >> 4;
}
// Touch 2 functions
uint16_t FT6336U::read_touch2_x(void) 
{
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_X);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_X + 1);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint16_t FT6336U::read_touch2_y(void) 
{
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_Y);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_Y + 1);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t FT6336U::read_touch2_event(void) 
{
    return readByte(FT6336U_ADDR_TOUCH2_EVENT) >> 6;
}
uint8_t FT6336U::read_touch2_id(void) 
{
    return readByte(FT6336U_ADDR_TOUCH2_ID) >> 4;
}
uint8_t FT6336U::read_touch2_weight(void) 
{
    return readByte(FT6336U_ADDR_TOUCH2_WEIGHT);
}
uint8_t FT6336U::read_touch2_misc(void) 
{
    return readByte(FT6336U_ADDR_TOUCH2_MISC) >> 4;
}

// Mode Parameter Register
void FT6336U::write_power_mode(uint8_t mode) 
{
    writeByte(FT6336U_ADDR_POWER_MODE, mode);
}

void FT6336U::write_monitor_mode_period(uint8_t mode) 
{
    writeByte(FT6336U_ADDR_MONITOR_MODE_RATE, mode);
}

void FT6336U::disable_face_dec_mode() 
{
    writeByte(0xB0, 0x00);
}

uint8_t FT6336U::read_touch_threshold(void) 
{
    return readByte(FT6336U_ADDR_THRESHOLD);
}
uint8_t FT6336U::read_filter_coefficient(void) 
{
    return readByte(FT6336U_ADDR_FILTER_COE);
}
uint8_t FT6336U::read_ctrl_mode(void) 
{
    return readByte(FT6336U_ADDR_CTRL);
}
void FT6336U::write_ctrl_mode(CTRL_MODE_Enum mode) 
{
    writeByte(FT6336U_ADDR_CTRL, mode);
}
uint8_t FT6336U::read_time_period_enter_monitor(void) 
{
    return readByte(FT6336U_ADDR_TIME_ENTER_MONITOR);
}
void FT6336U::write_time_period_enter_monitor(uint8_t time) 
{
    writeByte(FT6336U_ADDR_TIME_ENTER_MONITOR, time);
}

uint8_t FT6336U::read_active_rate(void) 
{
    return readByte(FT6336U_ADDR_ACTIVE_MODE_RATE);
}
uint8_t FT6336U::read_monitor_rate(void) 
{
    return readByte(FT6336U_ADDR_MONITOR_MODE_RATE);
}

// Gesture Parameters
uint8_t FT6336U::read_radian_value(void) 
{
	return readByte(FT6336U_ADDR_RADIAN_VALUE);
}
void FT6336U::write_radian_value(uint8_t val) 
{
	writeByte(FT6336U_ADDR_RADIAN_VALUE, val); 
}
uint8_t FT6336U::read_offset_left_right(void) 
{
	return readByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT);
}
void FT6336U::write_offset_left_right(uint8_t val) 
{
	writeByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT, val); 
}
uint8_t FT6336U::read_offset_up_down(void) 
{
	return readByte(FT6336U_ADDR_OFFSET_UP_DOWN);
}
void FT6336U::write_offset_up_down(uint8_t val) 
{
	writeByte(FT6336U_ADDR_OFFSET_UP_DOWN, val); 
}
uint8_t FT6336U::read_distance_left_right(void) 
{
	return readByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT);
}
void FT6336U::write_distance_left_right(uint8_t val) 
{
	writeByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT, val); 
}
uint8_t FT6336U::read_distance_up_down(void) 
{
	return readByte(FT6336U_ADDR_DISTANCE_UP_DOWN);
}
void FT6336U::write_distance_up_down(uint8_t val) 
{
	writeByte(FT6336U_ADDR_DISTANCE_UP_DOWN, val); 
}
uint8_t FT6336U::read_distance_zoom(void) 
{
	return readByte(FT6336U_ADDR_DISTANCE_ZOOM);
}
void FT6336U::write_distance_zoom(uint8_t val) 
{
	writeByte(FT6336U_ADDR_DISTANCE_ZOOM, val); 
}


// System Information
uint16_t FT6336U::read_library_version(void) 
{
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_LIBRARY_VERSION_H);
    read_buf[1] = readByte(FT6336U_ADDR_LIBRARY_VERSION_L);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t FT6336U::read_chip_id(void) 
{
    return readByte(FT6336U_ADDR_CHIP_ID);
}
uint8_t FT6336U::read_g_mode(void) 
{
    return readByte(FT6336U_ADDR_G_MODE);
}
void FT6336U::write_g_mode(G_MODE_Enum mode)
{
	writeByte(FT6336U_ADDR_G_MODE, mode); 
}
uint8_t FT6336U::read_pwrmode(void) 
{
    return readByte(FT6336U_ADDR_POWER_MODE);
}
uint8_t FT6336U::read_firmware_id(void) 
{
    return readByte(FT6336U_ADDR_FIRMARE_ID);
}
uint8_t FT6336U::read_focaltech_id(void) 
{
    return readByte(FT6336U_ADDR_FOCALTECH_ID);
}
uint8_t FT6336U::read_release_code_id(void) 
{
    return readByte(FT6336U_ADDR_RELEASE_CODE_ID);
}
uint8_t FT6336U::read_state(void) 
{
    return readByte(FT6336U_ADDR_STATE);
}

FT6336U_TouchPointType FT6336U::scan(void){
    touchPoint.touch_count = read_td_status(); 

    if(touchPoint.touch_count == 0) {
        touchPoint.tp[0].status = release; 
        touchPoint.tp[1].status = release; 
    }
    else if(touchPoint.touch_count == 1) {
        uint8_t id1 = read_touch1_id(); // id1 = 0 or 1
        touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream; 
        touchPoint.tp[id1].x = TOUCH_Width - read_touch1_y(); 
        touchPoint.tp[id1].y = read_touch1_x(); 
        touchPoint.tp[~id1 & 0x01].status = release; 
    }

    return touchPoint; 

}


// Private Function
uint8_t FT6336U::readByte(uint8_t addr) 
{
  uint8_t rdData = 0; 
  uint8_t rdDataCount; 
  do {
    _i2cPort->beginTransmission(_deviceAddress); 
    _i2cPort->write(addr); 
    _i2cPort->endTransmission(false); // Restart
    delay(10); 
    rdDataCount = _i2cPort->requestFrom(_deviceAddress, 1); 
  } while(rdDataCount == 0); 
  if(_i2cPort->available()) 
  {
    rdData = _i2cPort->read(); 
  }
  return rdData; 
}
void FT6336U::writeByte(uint8_t addr, uint8_t data) 
{
  _i2cPort->beginTransmission(_deviceAddress); 
  _i2cPort->write(addr); 
  _i2cPort->write(data); 
  _i2cPort->endTransmission(); 
}