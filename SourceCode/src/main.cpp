#include <main.h>

void setup()
{
  Serial.begin( 115200 ); /* prepare for possible serial debug */
  ReadFlash();
  ReadPID();

  System_Init();
}

void loop()
{
  lvgl_task_handler();
  MAX6675_Read_Task();
}

void setup1()
{

}

void loop1()
{
  Soldering_PID_Compute();
  Heatgun_PID_Compute();
}