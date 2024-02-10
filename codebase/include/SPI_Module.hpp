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

#ifndef SPIINIT
#define SPIINIT

#include <SPI.h>
#include "self_arduino.hpp"

#define MOSI_DEFAULT 12
#define MISO_DEFAULT 14
#define CS_DEFAULT 26
#define SCK_DEFAULT 27
#define FCLK_DEFAULT 2000000
#define ENDIANESS_DEFAULT MSBFIRST
#define MODE_DEFAULT SPI_MODE0

/* REQ 3.1 */
/* parameters: pins for SPI COM, fCLK, endieness, mode -> parameters for the SPI configuration
 * returns: spi_t SPI.bus() -> the SPI object created with the parameters */
spi_t * SPI_Init(int mosi = MOSI_DEFAULT, int miso = MISO_DEFAULT, int cs = CS_DEFAULT, int sck = SCK_DEFAULT, int fclk = FCLK_DEFAULT, int endieness = ENDIANESS_DEFAULT, int mode = MODE_DEFAULT);

#endif //SPIINIT