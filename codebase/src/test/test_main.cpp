#include "test_main.hpp"
#include "test/Tester.hpp"


#include "self_arduino.hpp"

#include "test_tester/test_tester.hpp"
#include "SPI_tester/SPI_tester.hpp"
#include "EPD_7IN3F_tester/EPD_7IN3F_tester.hpp"

void test_main(void)
{
    Serial.begin(9600);
    Serial.println("Starting Unit Tests...");

    SPI_tester();
    EPD_7IN3F_tester();
    
    for(;;){delay(1000);}
}

