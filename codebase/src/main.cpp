/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

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
    for(;;) 
    {
      vTaskDelay(1000);
      Serial.println("Hello World!");
    }
}
