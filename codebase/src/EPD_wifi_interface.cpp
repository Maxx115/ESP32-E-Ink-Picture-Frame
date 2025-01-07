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

#include "EPD_wifi_interface.hpp"

#include "WiFi.h"
#include <ArduinoHttpClient.h>

#include "EPD_nvm_man.hpp"

uint8_t EPD_IP_DEVICE[] = {192, 168, 0, 201};
String EPD_IP_DEVICE_STR = "192.168.0.201";
uint8_t EPD_IP_GATEWAY[] = {192, 168, 0, 1};
uint8_t EPD_IP_SUBNET[] = {255, 255, 255, 0};
uint8_t EPD_IP_DNS[] = {8, 8, 8, 8};
String HOSTNAME = "ESP32_E-Ink_Picture_Frame";

WiFiUDP UDP;
ScanResult ssidResult;

ScanResult scanSSID()
{
  Serial.println("Scan start");
  int numNetworks = WiFi.scanNetworks();
  Serial.println("Scan done");

  ssidResult.numNetworks = numNetworks;

  if (numNetworks == 0) 
  {
      Serial.println("No networks found");
      ssidResult.ssids = nullptr;
  } else 
  {
      Serial.print(numNetworks);
      Serial.println(" networks found");
      Serial.println("Nr | SSID | RSSI | CH | Encryption");

      // Create a string array to hold the SSIDs
      ssidResult.ssids = new String[numNetworks];
      ssidResult.rssis = new int[numNetworks];

      for (int i = 0; i < ssidResult.numNetworks; ++i) 
      {
          ssidResult.ssids[i] = WiFi.SSID(i);
          ssidResult.rssis[i] = WiFi.RSSI(i);

          Serial.printf("%2d | %-32.32s | %4ld | %2ld | ", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i));
          switch (WiFi.encryptionType(i)) 
          {
              case WIFI_AUTH_OPEN: Serial.print("open"); break;
              case WIFI_AUTH_WEP: Serial.print("WEP"); break;
              case WIFI_AUTH_WPA_PSK: Serial.print("WPA"); break;
              case WIFI_AUTH_WPA2_PSK: Serial.print("WPA2"); break;
              case WIFI_AUTH_WPA_WPA2_PSK: Serial.print("WPA+WPA2"); break;
              case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
              case WIFI_AUTH_WPA3_PSK: Serial.print("WPA3"); break;
              case WIFI_AUTH_WPA2_WPA3_PSK: Serial.print("WPA2+WPA3"); break;
              case WIFI_AUTH_WAPI_PSK: Serial.print("WAPI"); break;
              default: Serial.print("unknown");
          }
          Serial.println();
          if(i == 10)
          {
            break;
          }
      }
      Serial.println();
      WiFi.scanDelete();
      return ssidResult;
    }
}

void APstartupMode(void)
{
  WiFi.softAP(nvm_read_string(WIFI_NVM, AP_SSID_NVM).c_str(), nvm_read_string(WIFI_NVM, AP_PASSWORD_NVM).c_str());
  
  WiFi.printDiag(Serial);
  Serial.print("Home Controller IP: ");
  Serial.println(WiFi.localIP());
}

wl_status_t wifiInit_default(void)
{
  return wifiInit(EPD_IP_DEVICE, EPD_IP_GATEWAY, EPD_IP_SUBNET, EPD_IP_DNS, HOSTNAME, nvm_read_string(WIFI_NVM, SSID_NVM), nvm_read_string(WIFI_NVM, PASSWORD_NVM));
}

wl_status_t wifiInit(uint8_t ip_device[], uint8_t ip_gateway[], uint8_t ip_subnet[], uint8_t ip_dns[], String hostname, 
                     String ssid, String password)
{
  IPAddress ipAddress_device(ip_device);
  IPAddress ipAddress_gateway(ip_gateway);
  IPAddress ipAddress_subnet(ip_subnet);
  IPAddress ipAddress_dns(ip_dns);

  wl_status_t wifiReturn = WL_DISCONNECTED;

  if(!WiFi.config(ipAddress_device, ipAddress_gateway, ipAddress_subnet, ipAddress_dns))
  {
    Serial.println("STA failed to configure");
    return WL_CONNECT_FAILED;
  }
  else
  {
    WiFi.setHostname(hostname.c_str());
    WiFi.enableSTA(true);
    WiFi.begin(ssid.c_str(), password.c_str());
    
    for(int i = 0; (i < 5) && (WiFi.status() != WL_CONNECTED); i++)
    {
      delay(1000);
    }

    wifiReturn = WiFi.status();
    if(wifiReturn != WL_CONNECTED)
    {
      Serial.println("STA failed to connect");
      //Serial.println(ssid);
      //Serial.println(password);
      WiFi.enableSTA(false);
      WiFi.disconnect(false, true);
      return wifiReturn;
    }

    WiFi.printDiag(Serial);
    Serial.print("Home Controller IP: ");
    Serial.println(WiFi.localIP());
  }

  return wifiReturn;
}

String sendClientRequest(String ipAdress, String clientGet, boolean getBody)
{
    String body = "";
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, ipAdress);

    client.get(clientGet);
    if(getBody)
    {
      client.responseStatusCode();
      body = client.responseBody();
    }
    client.stop();

    return body;
}