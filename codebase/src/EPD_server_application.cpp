/*
MIT License

Copyright (c) [year] [fullname]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/* 
 * ESP32-E-Ink-Picture-frame
 * Project Owners:
 * Maxx115 (https://github.com/Maxx115)
 * madman1397 (https://github.com/madman1397)
 *
 * Main person in charge for this module: Maxx115 (https://github.com/Maxx115)
 * Creation date: [2024-01-26]
 * Requirements document: EPD_7IN3F-Requirements.txt
 */

#include "EPD_server_application.hpp"

#include "EPD_wifi_interface.hpp"


#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <ElegantOTA.h>

#include "EPD_crypto.hpp"
#include "EPD_nvm_man.hpp"

#include "SD.h"

AsyncWebServer server(80);

unsigned long ota_progress_millis = 0;

#define SERVER_VERSION "0.1"

void setupOTA();

void initSpiffs(void)
{
  if(!SPIFFS.begin(true))
  {
    //Serial.println("An Error has occurred while mounting SPIFFS");
  }
}

String siteProcess(const String& var)
{
  String statement = String();
  if(var == "VERSION")
  {
    statement = "server version: ";
    statement += SERVER_VERSION;
  }
  else if(var == "SSID_LIST")
  {
    statement = "";
    statement += "<thead>";
    statement += "<tr>";

    statement += "<th>ID</th>";
    statement += "<th>SSID</th>";
    statement += "<th>RSSI</th>";

    statement += "</tr>";
    statement += "</thead>";

    statement += "<tbody>";

    for(int i = 0; i < ssidResult.numNetworks; i++)
    {
        statement += "<tr>";

        statement += "<td>";
        statement += i;
        statement += "</td>";

        statement += "<td>";
        statement += ssidResult.ssids[i];
        statement += "</td>";

        statement += "<td>";
        statement += ssidResult.rssis[i];
        statement += "</td>";

        statement += "</tr>";

        if(i >= 5)
        {
            break;
        }
    }

    statement += "</tbody>";
  }
  else if(var == "PUBLIC_KEY")
  {
    statement = "`";
    statement += String(public_key);
    statement.remove(statement.length() - 1);
    statement += "`";
  }

  return statement;
}

void serverInit(bool setupStatus)
{
    initSpiffs();
    crypto_createPair();
    scanSSID();

    /* 
    --------------- Webpage Visuals --------------- 
    */

    /* 
    * Init HTML-File to be send to browser
    */
    if(setupStatus == SETUP_INIT)
    {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/setupPage.html", String(), false, siteProcess);
        });        
        server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/setupPage.html", String(), false, siteProcess);
        });
   }
    else
    {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            request->send(SPIFFS, "/index.html", String(), false, siteProcess);
        });
        server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/setupPage.html", String(), false, siteProcess);
        });
    }
    
    /*
    * Init CSS-File to be send to browser
    */
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/forge.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/forge.min.js", "application/javascript");
    });

    /* 
    --------------- Server Actions --------------- 
    */
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
    { 
        request->send(200, "text/plain", "OK");
        String inputMessage1 = "";
        String inputMessage2 = "";
        String inputMessage3 = "";

        if(request->hasParam("item")) 
        {
            inputMessage1 = request->getParam("item")->value();
            
            if(inputMessage1 == "wifi_connect")
            {
                if(request->hasParam("item"))
                {
                    inputMessage2 = request->getParam("ssid")->value();
                }
                if(request->hasParam("password"))
                {
                    inputMessage3 = request->getParam("password")->value();
                }
                if((inputMessage2 != "") && (inputMessage3 != ""))
                {
                    Serial.println("WiFi Request Receivced");
                    Serial.print("SSID: ");
                    Serial.println(inputMessage2);
                    Serial.print("Password: ");
                    Serial.println(inputMessage3);
                    
                    nvm_write_string(WIFI_NVM, SSID_NVM, inputMessage2);
                    nvm_write_string(WIFI_NVM, PASSWORD_NVM, cryptoDecrypt_Server(inputMessage3.c_str()));
                }
                else
                {
                    Serial.println("SSID or Password could not be received, nothing was stored on NVM");
                }
            }
        }
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request)
    { 
        request->send(200, "text/plain", "OK");
        Serial.println("Request for restart received!");
        Serial.println("Starting restart...");

        esp_restart();

        for(;;)
        {
          Serial.print(".");
        }
    });

    server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
    { 
        request->send(200, "text/plain", "OK");
        Serial.println("Request for restart received!");
        Serial.println("Starting restart...");

        esp_restart();

        for(;;)
        {
          Serial.print(".");
        }
    });

    /* 
    --------------- Setup image ---------------
    */
    server.serveStatic("/testImage.JPG", SD, "/testImage.JPG");

    /* 
    --------------- Start services ---------------
    */
    setupOTA();
    server.begin();
}

void onOTAStart() 
{
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) 
{
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) 
{
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void setupOTA()
{
    ElegantOTA.begin(&server);    // Start ElegantOTA
    // ElegantOTA callbacks
    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);
}

void OTATask(void *pvParameters)
{
  for(;;)
  {
    ElegantOTA.loop();
  }
}