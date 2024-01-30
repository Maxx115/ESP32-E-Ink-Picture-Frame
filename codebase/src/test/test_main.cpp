#include "test_main.hpp"
#include "test/Tester.hpp"


#include "self_arduino.hpp"

#include "test_tester/test_tester.hpp"

void test_main(void)
{
    Serial.begin(9600);
    Serial.println("Starting Unit Tests...");

    test_tester();
    
    for(;;){delay(1000);}
}

