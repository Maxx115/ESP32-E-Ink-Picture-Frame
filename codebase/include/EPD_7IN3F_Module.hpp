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

#ifndef EPD_7IN3_MODULE_HPP
#define EPD_7IN3_MODULE_HPP

#include "self_arduino.hpp"

typedef unsigned char ubyte_t;
typedef unsigned int uword_t;
typedef unsigned long udouble_t;

#define EPD_WIDTH       800
#define EPD_HEIGHT      480

#define EPD_7IN3F_BLACK   0x0	/// 000
#define EPD_7IN3F_WHITE   0x1	///	001
#define EPD_7IN3F_GREEN   0x2	///	010
#define EPD_7IN3F_BLUE    0x3	///	011
#define EPD_7IN3F_RED     0x4	///	100
#define EPD_7IN3F_YELLOW  0x5	///	101
#define EPD_7IN3F_ORANGE  0x6	///	110
#define EPD_7IN3F_CLEAN   0x7	///	111
#define EPD_7IN3F_STEPPER 0xF

#define RST_PIN_DEFAULT 33
#define DC_PIN_DEFAULT 25
#define BUSY_PIN_DEFAULT 32
#define CS_DEFAULT 26

#define CMDH_1 0xAA
#define CMDH_2 0x01
#define CMDH_3 0x00
#define CMDH_4 0x03
#define CMDH_5 0x05
#define CMDH_6 0x06
#define CMDH_7 0x08
#define IPC_1 0x13
#define IPC_2 0x30
#define TSE_1 0x41
#define TSE_2 0x50
#define TSE_3 0x60
#define TSE_4 0x61
#define TSE_5 0x82
#define TSE_6 0x84
#define AGID_1 0x86
#define AGID_2 0xE3
#define CCSET 0xE0
#define TSSET 0xE6
#define SLP 0x07
#define POWER_ON 0x04
#define DISPLAY_REFRESH 0x12
#define POWER_OFF 0x02
#define DATA_TRANSMISSION 0x10

#define DATA_PIN_ON HIGH
#define COMMAND_PIN_ON LOW

/* REQ 3.1 */
/* parameters: image data, xstart, ystart, image_width, image_height */
/* returns: success-flag */
bool epd_showImage(const ubyte_t *image_data, int refresh_cnt, uword_t xstart = 0, uword_t ystart = 0, uword_t image_width = EPD_WIDTH, uword_t image_height = EPD_HEIGHT, bool color = false);

/* REQ 3.2 */
/* parameters: epd color code */
/* returns: success-flag */
bool epd_showColor(ubyte_t color, int refresh_cnt, uword_t xstart = 0, uword_t ystart = 0, uword_t image_width = EPD_WIDTH, uword_t image_height = EPD_HEIGHT);

/* REQ 3.3 */
/* parameters: - */
/* returns: success-flag */
bool epd_reset(void);

/* REQ 3.4 */
/* parameters: - */
/* returns: success-flag */
bool epd_deepSleep(void);

/* REQ 3.5 */
/* parameters: - */
/* returns: success-flag */
bool epd_init(int rst_pin_param = RST_PIN_DEFAULT, int dc_pin_param = DC_PIN_DEFAULT, int busy_pin_param = BUSY_PIN_DEFAULT);

#endif /* EPD_7IN3_MODULE_HPP */