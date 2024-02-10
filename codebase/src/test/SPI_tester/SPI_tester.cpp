#include "test/SPI_tester/SPI_tester.hpp"
#include "test/Tester.hpp"

#include "self_arduino.hpp"
#include "SPI_Module.hpp"

void SPI_tester(void)
{
    Tester init_test("Init_Test", "This test shall test the REQ 3.1");
    init_test.begin();
    init_test.tester_assert(SPI_Init(), SPI.bus());
    init_test.end();
}