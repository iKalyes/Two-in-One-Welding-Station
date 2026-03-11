#include <main.h>

// 1: 仅执行 ClearFlash()；0: 正常运行系统
#ifndef RUN_CLEAR_FLASH_ONLY
#define RUN_CLEAR_FLASH_ONLY 0
#endif

// GUI task handle 定义
TaskHandle_t s_lvglTaskHandle = NULL;
TaskHandle_t s_ina226TaskHandle = NULL;
TaskHandle_t s_tmp102TaskHandle = NULL;
TaskHandle_t s_max6675TaskHandle = NULL;
TaskHandle_t s_pidUiTaskHandle = NULL;

mutex_t i2c_mutex;
mutex_t spi_mutex;
volatile bool bSystemInitialized = false;

static void vLvglTask(void *pvParameters)
{
  // 系统与 LVGL 初始化均在 GUI 任务中执行，确保所有 LVGL API 只在单一任务内调用
  System_Init();

  // 初始化 I2C 互斥锁
  mutex_init(&i2c_mutex);
  // 初始化 SPI 互斥锁（MAX6675 由 core0/core1 共享）
  mutex_init(&spi_mutex);

  bSystemInitialized = true;

  // 周期性处理 LVGL 任务
  for (;;)
  {
    lvgl_task_handler();
    vTaskDelay(pdMS_TO_TICKS(5)); // 5~10ms 周期更平滑
  }
}

// INA226 采样任务：100ms 周期 (运行在 Core 0)
static void vINA226Task(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if (bSystemInitialized)
    {
      // 使用 Pico SDK 互斥锁保护 I2C
      mutex_enter_blocking(&i2c_mutex);
      INA226_Task();
      mutex_exit(&i2c_mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// TMP102 采样任务：1000ms 周期 (运行在 Core 0)
static void vTMP102Task(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if (bSystemInitialized)
    {
      // 使用 Pico SDK 互斥锁保护 I2C
      mutex_enter_blocking(&i2c_mutex);
      TMP102_Task();
      mutex_exit(&i2c_mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// MAX6675 周期任务：10ms 周期 (运行在 Core 0)
static void vMAX6675Task(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if (bSystemInitialized)
    {
      MAX6675_Read_Task();
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// PID UI 同步任务：20ms 周期 (运行在 Core 0)
static void vPIDUiTask(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if (bSystemInitialized)
    {
      PID_UI_Sync_Task();
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void setup()
{
  Serial.begin( 115200 ); /* prepare for possible serial debug */

#if RUN_CLEAR_FLASH_ONLY
  ClearFlash();
  // 清空后进入空循环，避免启动任何业务任务
  while (true)
  {
    delay(1000);
  }
#else

  // 创建并启动 GUI 任务
  xTaskCreate(
      vLvglTask,
      "LVGL",
      4096, // 视 UI 复杂度适当调节
      nullptr,
      tskIDLE_PRIORITY + 5, // 较高优先级以保证流畅性
      &s_lvglTaskHandle);

  // 创建 INA226 周期任务（100ms）
  xTaskCreate(
      vINA226Task,
      "INA226",
      512,
      nullptr,
      tskIDLE_PRIORITY + 1,
      &s_ina226TaskHandle);

  // 创建 TMP102 周期任务（1000ms）
  xTaskCreate(
      vTMP102Task,
      "TMP102",
      512,
      nullptr,
      tskIDLE_PRIORITY + 1,
      &s_tmp102TaskHandle);

    // 创建 MAX6675 周期任务（20ms）
    xTaskCreate(
      vMAX6675Task,
      "MAX6675",
      768,
      nullptr,
      tskIDLE_PRIORITY + 2,
      &s_max6675TaskHandle);

    // 创建 PID->LVGL UI 同步任务（20ms）
    xTaskCreate(
      vPIDUiTask,
      "PID_UI",
      768,
      nullptr,
      tskIDLE_PRIORITY + 2,
      &s_pidUiTaskHandle);
#endif
}

void loop()
{
#if RUN_CLEAR_FLASH_ONLY
  delay(1000);
#else
  // 主循环留空，业务逻辑由 FreeRTOS 任务驱动
  vTaskDelay(pdMS_TO_TICKS(1000));
#endif
}

void setup1()
{

}

void loop1()
{
#if RUN_CLEAR_FLASH_ONLY
  delay(1);
  return;
#else
  if (!bSystemInitialized)
  {
    delay(1);
    return;
  }

  Soldering_PID_Compute();
  Heatgun_PID_Compute();
#endif
}