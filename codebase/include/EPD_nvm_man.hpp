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

#ifndef EPD_NVM_MAN
#define EPD_NVM_MAN

#include "self_arduino.hpp"
#include <nvs_flash.h>
#include <nvs.h>

#define WIFI_NVM "SSID-Password"

#define AP_SSID_NVM "AP_SSID"
#define AP_PASSWORD_NVM "AP_Password"

#define SSID_NVM "SSID"
#define PASSWORD_NVM "Password"

void initNVM(void);

void nvm_write_string(String storageSpace, String storageKey, String toStoreStr);
String nvm_read_string(String storageSpace, String storageKey);

bool nvm_erase_namespace(String storageSpace);
void listKeysInNamespace(const char* namespace_name);
bool nvm_contains(String storageSpace, String storageKey);

#endif /* EPD_NVM_MAN */