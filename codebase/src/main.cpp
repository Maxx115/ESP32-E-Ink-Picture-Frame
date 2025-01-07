/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"

#include "say_hello.hpp"
#include "wifi_init.hpp"
#include "SPI_Module.hpp"
#include "imagedata.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include <SPIFFS.h>
#include "SD.h"

#include "EPD_7IN3F_Module.hpp"
#include "EPD_nvm_man.hpp"
#include "EPD_wifi_interface.hpp"
#include "EPD_server_application.hpp"
#include "EPD_SD_Card.hpp"


/* ---------------- DEFINES / CONSTANTS ---------------- */


/* ---------------- HELP FUNCTION DECLARATION SECTION ---------------- */


/* ---------------- LOCAL VARIABLE SECTION ---------------- */


/* ---------------- RTOS SETUP SECTION ---------------- */
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif

void loopTask(void *pvParameters);

TaskHandle_t loopTaskHandle = NULL;
TaskHandle_t OTATaskHandle = NULL;

bool loopTaskWDTEnabled;

extern "C" void app_main()
{
  loopTaskWDTEnabled = false;
  initArduino();
  Serial.begin(9600);
  initNVM();
  SD_init();
  createDir(SD, "/EPD_IMAGES");
  
  #if ERASE_WIFI==1
  if(nvm_erase_namespace(WIFI_NVM))
  {
    Serial.println("WiFi NVM Erased!");
  }
  else
  {
    Serial.println("WiFi NVM not Erased!");
  }
  #endif

  /* --- START Check for first boot --- */

  if(nvm_contains(WIFI_NVM, AP_SSID_NVM) == false)
  {
    nvm_write_string(WIFI_NVM, AP_SSID_NVM, SETUP_AP_SSID);
    nvm_write_string(WIFI_NVM, AP_PASSWORD_NVM, SETUP_AP_PASSWORD);
  }

  wl_status_t wifi_attempt = WL_NO_SHIELD;
  if(nvm_contains(WIFI_NVM, SSID_NVM) == true)
  {
    wifi_attempt = wifiInit_default();
    if(wifi_attempt == WL_CONNECTED)
    {
      serverInit(HOME_INIT);
    }
  }

  if(wifi_attempt != WL_CONNECTED)
  {
    APstartupMode();
    serverInit(SETUP_INIT);
  }

  /* --- END Check for first boot --- */

  xTaskCreateUniversal(loopTask, "loopTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(OTATask, "OTATask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &OTATaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
}

/* ---------------- RTOS TASK SECTION ---------------- */
void loopTask(void *pvParameters)
{
  copyFile(SD, "/testImage.jpg", "/EPD_IMAGES/testImage_EPD.jpg");
  /*
  Serial.println("Starting init:...");
  epd_init();

  Serial.println("Starting SD Card testing...");
  SD_init();

  Serial.println("Show an image: TEST...");
  epd_showImage(gImage_7in3f, 1, 250, 150, 300, 180);
  //vTaskDelay(2000);

  Serial.println("Show an image: TEST...");
  epd_showImage(gImage_test, 1);
  //vTaskDelay(2000);
  
  Serial.println("Going to sleep:...");
  epd_deepSleep();
  */

  for(;;)
  {
    vTaskDelay(5000);
  }
}
