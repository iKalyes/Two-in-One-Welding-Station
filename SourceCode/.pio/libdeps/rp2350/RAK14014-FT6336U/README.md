| <center><img src="./assets/rakstar.jpg" alt="RAKstar" width=25%></center>  | ![RAKWireless](./assets/RAK-Whirls.png) | [![Build Status](https://github.com/RAKWireless/RAK13005-TLE7259-Library/workflows/RAK%20Library%20Build%20CI/badge.svg)](https://github.com/RAKWireless/RAK13005-TLE7259-Library/actions) |
| -- | -- | -- |

# <RAK14014-FT6336U>

RAK14014-FT6336U library based on Arduino-FT6336U. The library is modified to add support for RAK4631, RAK11200, RAK11300 Core.

[*RAKWireless <RAK#> <function>*](https://store.RAKWireless.com/products/RAK14014-TFT)

# Documentation

* **[Product Repository](https://github.com/RAKWireless/RAK14014-TFT)** - Product repository for the RAKWireless RAK14014-TFT module.
* **[Documentation](https://docs.RAKWireless.com/Product-Categories/WisBlock/RAK14014/Overview/)** - Documentation and Quick Start Guide for the RAK14014-TFT module.

# Installation

In Arduino IDE open Sketch->Include Library->Manage Libraries then search for RAK14014.

In PlatformIO open PlatformIO Home, switch to libraries and search for RAK14014.
Or install the library project dependencies by adding

```log
lib_deps =
  RAKWireless/RAKWireless RAK14014 TFT library
```

into **`platformio.ini`**

For manual installation download the archive, unzip it and place the RAK14014-FT6336U folder into the library directory.
In Arduino IDE this is usually <arduinosketchfolder>/libraries/
In PlatformIO this is usually <user/.platformio/lib>

# Usage

The library provides FT6336U class, which allows communication with FT6336U via IIC. These examples show how to use RAK14014.

- [RAK14014_TP_FT6336U](./examples/RAK14014_TP_FT6336U) This example is for printing touch locations.

## This class provides the following methods:

**bool FT6336U::begin(TwoWire &wirePort, uint8_t deviceAddress)**

Initalizes the FT6336U.

#### Parameters:

| Direction | Name          | Function                                                     |
| --------- | ------------- | ------------------------------------------------------------ |
| in        | wirePort      | IIC interface used.                                          |
| in        | deviceAddress | Device address should be 0x38.                               |
| return    |               | If the device init successful return true else return false. |

**uint8_t FT6336U::read_device_type(void) **

Get chip code.

#### Parameters:

| Direction | Name | Function                                                     |
| --------- | ---- | ------------------------------------------------------------ |
| return    |      | 0x00: Ft6236G <br />0x01: Ft6336G <br />0x02: Ft6336U<br />0x03: Ft6426 |


**uint8_t FT6336U::read_td_status(void)**

Read the number of reporting points, the maximum support for reporting two points at the same time.

#### Parameters:

| Direction | Name | Function                    |
| --------- | ---- | --------------------------- |
| return    |      | Number of reporting points. |

**uint16_t FT6336U::read_touch1_x(void) **

Read the X-coordinate of the first point.

#### Parameters:

| Direction | Name | Function            |
| --------- | ---- | ------------------- |
| return    |      | x coordinate value. |

**uint16_t FT6336U::read_touch1_y(void) **

Read the Y-coordinate of the first point.

#### Parameters:

| Direction | Name | Function            |
| --------- | ---- | ------------------- |
| return    |      | y coordinate value. |

**uint16_t FT6336U::read_touch2_x(void) **

Read the X-coordinate of the second point.

#### Parameters:

| Direction | Name | Function            |
| --------- | ---- | ------------------- |
| return    |      | x coordinate value. |

**uint16_t FT6336U::read_touch2_y(void) **

Read the Y-coordinate of the second point.

#### Parameters:

| Direction | Name | Function            |
| --------- | ---- | ------------------- |
| return    |      | y coordinate value. |