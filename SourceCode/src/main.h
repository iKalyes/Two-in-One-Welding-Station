#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <Flash_Service.h>

#include <System_Init.h>
#include <PID_Service.h>

#include <MAX6675_Service.h>
#include <INA226_Service.h>
#include <TMP102_Service.h>

// FreeRTOS 统一在头文件中包含
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <pico/mutex.h>

// GUI 任务句柄对外声明（定义在 main.cpp）
extern TaskHandle_t s_lvglTaskHandle;
extern TaskHandle_t s_ina226TaskHandle;
extern TaskHandle_t s_tmp102TaskHandle;
extern TaskHandle_t s_max6675TaskHandle;
extern TaskHandle_t s_pidUiTaskHandle;

// I2C 互斥锁 (使用 Pico SDK 硬件互斥锁，支持跨核/混合模式)
extern mutex_t i2c_mutex;
// SPI 互斥锁 (MAX6675 跨核访问仲裁)
extern mutex_t spi_mutex;

// 系统初始化标志
extern volatile bool bSystemInitialized;

#endif