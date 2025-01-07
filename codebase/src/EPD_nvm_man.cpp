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

#include "self_arduino.hpp"
#include <nvs_flash.h>
#include <nvs.h>

void initNVM(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
        Serial.println("Initializing NVS failed!");
    }
    ESP_ERROR_CHECK(err);
}

void nvm_write_string(String storageSpace, String storageKey, String toStoreStr)
{
    // Open a handle
    nvs_handle nvmHandle;
    esp_err_t err = nvs_open(storageSpace.c_str(), NVS_READWRITE, &nvmHandle);
    if (err != ESP_OK) 
    {
        Serial.println("NVS could not be opened!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }

    // Store a String
    err = nvs_set_str(nvmHandle, storageKey.c_str(), toStoreStr.c_str());
    if (err != ESP_OK) 
    {
        Serial.println("Storing String failed!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }
    
    err = nvs_commit(nvmHandle);
    if (err != ESP_OK) 
    {
        Serial.println("NVS could not be committed!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }

    // Close the handle
    nvs_close(nvmHandle);

    if(err == ESP_OK)
    {
        Serial.print("Stored value ");
        Serial.print(toStoreStr);
        Serial.print(" in key ");
        Serial.println(storageKey);   
    }
}

String nvm_read_string(String storageSpace, String storageKey)
{
    // Open a handle
    nvs_handle nvmHandle;
    esp_err_t err = nvs_open(storageSpace.c_str(), NVS_READWRITE, &nvmHandle);
    if (err != ESP_OK) 
    {
        Serial.println("NVS could not be opened!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }

    size_t size_string;
    err = nvs_get_str(nvmHandle, storageKey.c_str(), NULL, &size_string);
    if (err != ESP_OK) 
    {
        Serial.println("Reading String failed!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }
    
    char* read_string = (char*)malloc(size_string);
    if(read_string == NULL)
    {
        Serial.println("Allocating memory failed!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }
    err = nvs_get_str(nvmHandle, storageKey.c_str(), read_string, &size_string);
    if (err != ESP_OK) 
    {
        Serial.println("Reading String failed!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }

    String str(read_string);

    // Close the handle
    free(read_string);
    nvs_close(nvmHandle);

    return str;
}

bool nvm_erase_namespace(String storageSpace)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle my_handle;
    err = nvs_open(storageSpace.c_str(), NVS_READWRITE, &my_handle); // "storage" is an example namespace
    if (err != ESP_OK) 
    {
        nvs_close(my_handle);
        return false;
    }  

    err = nvs_erase_all(my_handle);
    if (err != ESP_OK) 
    {
        nvs_close(my_handle);
        return false;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) 
    {
        nvs_close(my_handle);
        return false;
    }

    nvs_close(my_handle);
    return true;
}

void listKeysInNamespace(const char* namespace_name) 
{
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        Serial.printf("Error initializing NVS: %s\n", esp_err_to_name(err));
        return;
    }

    nvs_iterator_t it = nvs_entry_find(NVS_DEFAULT_PART_NAME, namespace_name, NVS_TYPE_ANY);
    if (it == NULL) {
        Serial.println("No entries found");
        return;
    }

    Serial.println("Keys in namespace:");
    while (it != NULL) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        Serial.printf("Key: %s, Type: 0x%02x\n", info.key, info.type);

        it = nvs_entry_next(it);
    }

    nvs_release_iterator(it);
}

bool nvm_contains(String storageSpace, String storageKey) 
{
    // Open a handle
    nvs_handle nvmHandle;
    esp_err_t err = nvs_open(storageSpace.c_str(), NVS_READWRITE, &nvmHandle);
    if (err != ESP_OK) 
    {
        Serial.println("NVS could not be opened!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }

    size_t size_string;
    err = nvs_get_str(nvmHandle, storageKey.c_str(), NULL, &size_string);

    nvs_close(nvmHandle);

    if (err == ESP_OK) 
    {
        return true;
    }
    else if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        Serial.print(storageKey);
        Serial.println(" not found.");
    }
    else
    {
        Serial.println("Reading String failed!");
        Serial.print("0x");
        Serial.println(err, HEX);
    }

    return false;
}