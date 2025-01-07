#include "EPD_JPG_Modifier.hpp"

#include <Arduino.h>
#include "JPEGDEC.h"

bool modifyJPEG(fs::FS &fs, const char *filePath, int brightnessChange)
{
    Serial.printf("Modifying JPG file: %s\n", filePath);
    
    File file = fs.open(filePath, FILE_READ);
    if(!file)
    {
        Serial.println("Failed to open file for reading!");
        return false;
    }

    JPEGDEC jpeg;

}