#include "SPI_Module.hpp"

/* REQ 3.1 */
void SPI_Init(int mosi, int miso, int cs, int sck, int fclk, int endieness, int mode)
{
    digitalWrite(cs, HIGH);
    pinMode(cs, OUTPUT);
    pinMode(mosi, OUTPUT);
    pinMode(miso, OUTPUT);
    SPI.begin(sck, miso, mosi, cs);
    SPI.beginTransaction(SPISettings(fclk, endieness, mode));
}