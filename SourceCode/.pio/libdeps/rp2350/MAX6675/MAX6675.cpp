//
//    FILE: MAX6675.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.3.2
// PURPOSE: Arduino library for MAX6675 chip for K type thermocouple
//    DATE: 2022-01-11
//     URL: https://github.com/RobTillaart/MAX6675


#include "MAX6675.h"

MAX6675::MAX6675(uint8_t miso, uint8_t select, uint8_t clock, __SPI_CLASS__ * mySPI, uint32_t speed)
{
  _select = select;
  _miso   = miso;
  _clock  = clock;
  _mySPI  = mySPI;
  _SPIspeed = speed;
  _spi_settings = SPISettings(_SPIspeed, MSBFIRST, SPI_MODE0);
}


void MAX6675::begin()
{
  _lastTimeRead = 0;
  _offset       = 0;
  _status       = STATUS_NOREAD;
  _temperature  = MAX6675_NO_TEMPERATURE;
  _rawData      = 0;

  pinMode(_select, OUTPUT);
  digitalWrite(_select, HIGH);

  _mySPI -> setRX(_miso);
  _mySPI -> setSCK(_clock);
  _mySPI -> begin();
  delay(1);

}

uint8_t MAX6675::read()
{
  //  return value of _read()  page 5 datasheet
  //  BITS       DESCRIPTION
  //  ------------------------------
  //       00    three state ?
  //       01    device ID ?
  //       02    INPUT OPEN
  //  03 - 14    TEMPERATURE (RAW)
  //       15    SIGN
  uint16_t value = _read();

  //  needs a pull up on MISO pin to work properly!
  if (value == 0xFFFF)
  {
    _status = STATUS_NO_COMMUNICATION;
    return _status;
  }

  _lastTimeRead = millis();

  //  process status bit 2
  _status = value & 0x04;

   value >>= 3;

  //  process temperature bits
  _temperature = (value & 0x1FFF) * 0.25;
  //  dummy negative flag set ?
  //  if (value & 0x2000)
  return _status;
}


///////////////////////////////////////////////////
//
//  PRIVATE
//
uint32_t MAX6675::_read(void)
{
  _rawData = 0;
  //  DATA TRANSFER
    _mySPI->beginTransaction(_spi_settings);
    //  must be after mySPI->beginTransaction() - STM32 (#14 MAX31855_RT)
    digitalWrite(_select, LOW);
    _rawData = _mySPI->transfer(0);
    _rawData <<= 8;
    _rawData += _mySPI->transfer(0);
    digitalWrite(_select, HIGH);
    _mySPI->endTransaction();

  return _rawData;
}


//  -- END OF FILE --

