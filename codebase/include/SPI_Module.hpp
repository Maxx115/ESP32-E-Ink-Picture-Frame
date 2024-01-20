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
// parameters: pins for SPI COM, fCLK, endieness, mode
spi_t * SPI_Init(int mosi = MOSI_DEFAULT, int miso = MISO_DEFAULT, int cs = CS_DEFAULT, int sck = SCK_DEFAULT, int fclk = FCLK_DEFAULT, int endieness = ENDIANESS_DEFAULT, int mode = MODE_DEFAULT);

#endif //SPIINIT