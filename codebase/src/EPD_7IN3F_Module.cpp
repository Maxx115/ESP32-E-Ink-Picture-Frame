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

#include "EPD_7IN3F_Module.hpp"

#include "self_arduino.hpp"
#include "freertos/FreeRTOS.h"
#include "SPI_Module.hpp"

bool initialized = false;

ubyte_t color_step = 0x00;

int rst_pin = 0;
int dc_pin = 0;
int busy_pin = 0;
int power_pin = 0;

/* REQ 3.3 */
/* parameters: - */
/* returns: success-flag */
bool epd_reset(void)
{
    bool retValue = false;

    if(rst_pin != 0)
    {
        digitalWrite(rst_pin, HIGH);
        vTaskDelay(20);
        digitalWrite(rst_pin, LOW);
        vTaskDelay(2);
        digitalWrite(rst_pin, HIGH);
        vTaskDelay(20);

        retValue = true;
    }

    return retValue;
}

void edp_BusyHigh(void)
{
    while(!digitalRead(busy_pin))
    {
        vTaskDelay(1);
    }
}

void sendSPI(ubyte_t spi_data)
{
    if(SPI.bus())
    {
        digitalWrite(SPI.pinSS(), LOW);
        SPI.transfer(spi_data);
        digitalWrite(SPI.pinSS(), HIGH);
    }
}

void sendCmd(ubyte_t cmd)
{
    digitalWrite(dc_pin, COMMAND_PIN_ON);
    sendSPI(cmd);
}

void sendData(ubyte_t data)
{
    digitalWrite(dc_pin, DATA_PIN_ON);
    sendSPI(data);
}

void epd_OnRefreshOff(int refresh_cnt)
{
    Serial.println("<Turning on EDP>");
    sendCmd(POWER_ON);  // POWER_ON
    edp_BusyHigh();
    
    for(int i = 0; i < refresh_cnt; i++)
    {
        sendCmd(DISPLAY_REFRESH);  // DISPLAY_REFRESH
        sendData(0x00);
        edp_BusyHigh();
    }
    
    sendCmd(POWER_OFF);  // POWER_OFF
    sendData(0x00);
    edp_BusyHigh();
}

/* REQ 3.5 */
/* parameters: - */
/* returns: success-flag */
bool epd_init(int rst_pin_param, int dc_pin_param, int busy_pin_param)
{
    bool retValue = false;

    rst_pin = rst_pin_param;
    dc_pin = dc_pin_param;
    busy_pin = busy_pin_param;

    Serial.println("<Initializing SPI>");
    if(SPI_Init())
    {
        Serial.println("<Initializing EDP>");

        digitalWrite(rst_pin, LOW);

        pinMode(rst_pin, OUTPUT);
        pinMode(dc_pin, OUTPUT);
        #if UNIT_TEST==1
        pinMode(busy_pin, INPUT_PULLUP); 
        #else
        pinMode(busy_pin, INPUT_PULLDOWN);
        #endif

        epd_reset();
        edp_BusyHigh();
        
        sendCmd(CMDH_1);    // CMDH
        sendData(0x49);
        sendData(0x55);
        sendData(0x20);
        sendData(0x08);
        sendData(0x09);
        sendData(0x18);

        sendCmd(CMDH_2);
        sendData(0x3F);
        sendData(0x00);
        sendData(0x32);
        sendData(0x2A);
        sendData(0x0E);
        sendData(0x2A);

        sendCmd(CMDH_3);
        sendData(0x5F);
        sendData(0x69);

        sendCmd(CMDH_4);
        sendData(0x00);
        sendData(0x54);
        sendData(0x00);
        sendData(0x44); 

        sendCmd(CMDH_5);
        sendData(0x40);
        sendData(0x1F);
        sendData(0x1F);
        sendData(0x2C);

        sendCmd(CMDH_6);
        sendData(0x6F);
        sendData(0x1F);
        sendData(0x1F);
        sendData(0x22);

        sendCmd(CMDH_7);
        sendData(0x6F);
        sendData(0x1F);
        sendData(0x1F);
        sendData(0x22);

        sendCmd(IPC_1);    // IPC
        sendData(0x00);
        sendData(0x04);

        sendCmd(IPC_2);
        sendData(0x3C);

        sendCmd(TSE_1);     // TSE
        sendData(0x00);

        sendCmd(TSE_2);
        sendData(0x3F);

        sendCmd(TSE_3);
        sendData(0x02);
        sendData(0x00);

        sendCmd(TSE_4);
        sendData(0x03);
        sendData(0x20);
        sendData(0x01); 
        sendData(0xE0);

        sendCmd(TSE_5);
        sendData(0x1E); 

        sendCmd(TSE_6);
        sendData(0x00);

        sendCmd(AGID_1);    // AGID
        sendData(0x00);

        sendCmd(AGID_2);
        sendData(0x2F);

        sendCmd(CCSET);   // CCSET
        sendData(0x00); 

        sendCmd(TSSET);   // TSSET
        sendData(0x00);

        retValue = true;
    }
    
    initialized = retValue;
    epd_showColor(EPD_7IN3F_STEPPER, 1);
    epd_showColor(EPD_7IN3F_WHITE, 1);

    return retValue;
}

ubyte_t getData(const ubyte_t *image_data, uword_t image_width, uword_t xstart, uword_t ystart, uword_t x_pos, uword_t y_pos, bool color)
{
    ubyte_t retValue = 0x00;

    if(color)
    {
        if(*image_data == EPD_7IN3F_STEPPER)
        {
            color_step = ((color_step + random(7)) % EPD_7IN3F_CLEAN);
            retValue = (color_step << 4) | color_step;
        }
        else
        {
            retValue = (*image_data << 4) | *image_data;
        }
    }
    else
    {
        retValue = pgm_read_byte(&image_data[(x_pos - xstart/2) + (image_width/2 * (y_pos - ystart))]);
    }

    return retValue;
}

/* REQ 3.1 */
/* parameters: image data, xstart, ystart, image_width, image_height */
/* returns:  */
bool epd_showImage(const ubyte_t *image_data, int refresh_cnt , uword_t xstart, uword_t ystart, uword_t image_width, uword_t image_height, bool color)
{
    bool retValue = false;

    Serial.println("<Sending image data to EDP>");

    if(initialized != true)
    {
        retValue = epd_init();
    }

    if(initialized == true)
    {
        unsigned long x,y;

        sendCmd(DATA_TRANSMISSION);

        for(y = 0; y < EPD_HEIGHT; y++) 
        {
            for(x = 0; x < EPD_WIDTH/2; x++) 
            {
                if(y < image_height + ystart && y >= ystart && x < (image_width + xstart)/2 && x >= xstart/2) 
                {
                    sendData(getData(image_data, image_width, xstart, ystart, x, y, color));
                }
                else
                {
                    sendData(0x11);
                }
            }
        }
        epd_OnRefreshOff(refresh_cnt);

        retValue = true;
    }

    return retValue;
}

/* REQ 3.2 */
/* parameters: epd color code */
/* returns: success-flag */
bool epd_showColor(ubyte_t color, int refresh_cnt, uword_t xstart, uword_t ystart, uword_t image_width, uword_t image_height)
{
    bool retValue;

    if((color > 0x7))
    {
        retValue = false;
    }
    else
    {
        retValue = epd_showImage(&color, refresh_cnt, xstart, ystart, image_width, image_height, true);
    }

    return retValue;
}

/* REQ 3.4 */
/* parameters: - */
/* returns: success-flag */
bool epd_deepSleep(void)
{
    bool retValue = false;

    Serial.println("<Deep sleep EDP>");

    edp_BusyHigh();

    sendCmd(SLP);
    sendData(0xA5);
    vTaskDelay(10);
	digitalWrite(rst_pin, LOW); // Reset

    retValue = true;

    return retValue;
}

