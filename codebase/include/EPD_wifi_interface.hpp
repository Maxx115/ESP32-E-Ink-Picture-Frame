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

#ifndef EPD_WIFI_INTERFACE
#define EPD_WIFI_INTERFACE

#include "self_arduino.hpp"
#include "WiFi.h"

extern uint8_t EPD_IP_DEVICE[];
extern String EPD_IP_DEVICE_STR;
extern uint8_t EPD_IP_GATEWAY[];
extern uint8_t EPD_IP_SUBNET[];
extern uint8_t EPD_IP_DNS[];
extern String HOSTNAME;

#define SETUP_AP_SSID "ESP_E-Ink_Picture_Frame"
#define SETUP_AP_PASSWORD "123456789"

struct ScanResult 
{
     int numNetworks;
     String* ssids;
     int* rssis;
};

extern ScanResult ssidResult;

ScanResult scanSSID(void);
void APstartupMode(void);

wl_status_t wifiInit(uint8_t ip_device[] = EPD_IP_DEVICE, uint8_t ip_gateway[] = EPD_IP_GATEWAY, uint8_t ip_subnet[] = EPD_IP_SUBNET, uint8_t ip_dns[] = EPD_IP_DNS, String hostname = HOSTNAME, 
                     String ssid = "", String password = "");
wl_status_t wifiInit_default(void);

#endif /* EPD_WIFI_INTERFACE */