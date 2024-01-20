/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"
#include "say_hello.hpp"
#include "wifi_init.hpp"
#include "SPI_Module.hpp"

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
  unsigned char data = '9';
  SPI_Init();
  digitalWrite(SPI.pinSS(), LOW);
  SPI.transfer(data);
  digitalWrite(SPI.pinSS(), HIGH);
  SPI_Init();
  digitalWrite(SPI.pinSS(), LOW);
  SPI.transfer(data);
  digitalWrite(SPI.pinSS(), HIGH);
  for(;;)
  {
    vTaskDelay(5000);
  }

  /*
    if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount file systen");
    return;
    }
    WiFiInit();
    for(;;) 
    {
      vTaskDelay(5000);
    }
  */
}
