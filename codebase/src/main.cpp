/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"
#include "say_hello.hpp"
#include "wifi_init.hpp"
#include "SPI_Module.hpp"
#include "EPD_7IN3F_Module.hpp"
#include "imagedata.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include <SPIFFS.h>

/* ---------------- DEFINES / CONSTANTS ---------------- */


/* ---------------- HELP FUNCTION DECLARATION SECTION ---------------- */


/* ---------------- LOCAL VARIABLE SECTION ---------------- */


/* ---------------- RTOS SETUP SECTION ---------------- */
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif

void loopTask(void *pvParameters);
TaskHandle_t loopTaskHandle = NULL;
bool loopTaskWDTEnabled;

extern "C" void app_main()
{
  loopTaskWDTEnabled = false;
  initArduino();
  Serial.begin(9600);

  xTaskCreateUniversal(loopTask, "loopTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
}

/* ---------------- RTOS TASK SECTION ---------------- */
void loopTask(void *pvParameters)
{
  while(!Serial.available())
  {
    vTaskDelay(10);
  }

  Serial.println("Starting init:...");
  epd_init();

  //Serial.println("Show an image: TEST...");
  //epd_showImage(gImage_7in3f, 1, 250, 150, 300, 180);
  //vTaskDelay(2000);

  Serial.println("Show an image: TEST...");
  epd_showImage(gImage_test, 1);
  vTaskDelay(2000);
  
  Serial.println("Going to sleep:...");
  epd_deepSleep();

  for(;;)
  {
    vTaskDelay(5000);
  }
}
