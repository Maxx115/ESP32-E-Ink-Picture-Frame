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
 * Creation date: [2024-01-20]
 * Requirements document: SPI-Requirements.txt
 */

#include "SPI_Module.hpp"

/* REQ 3.1 */
/* parameters: pins for SPI COM, fCLK, endieness, mode -> parameters for the SPI configuration
 * returns: spi_t SPI.bus() -> the SPI object created with the parameters */
spi_t * SPI_Init(int mosi, int miso, int cs, int sck, int fclk, int endieness, int mode)
{
    if(SPI.bus())
    {
        Serial.println("SPI already initialized -> Reinitializing with current arguments...");
        SPI.endTransaction();
        SPI.end();
    }



    digitalWrite(cs, HIGH);
    digitalWrite(mosi, LOW);
    digitalWrite(miso, LOW);
    digitalWrite(sck, LOW);

    pinMode(cs, OUTPUT);
    pinMode(mosi, OUTPUT);
    pinMode(miso, OUTPUT);
    
    SPI.begin(sck, miso, mosi, cs);
    SPI.beginTransaction(SPISettings(fclk, endieness, mode));

    return SPI.bus();
}