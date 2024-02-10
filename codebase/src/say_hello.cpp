#include "self_arduino.hpp"
#include "say_hello.hpp"
void sayHello(void)
{
    for(int i=0; i<=5; i++) 
    {
        Serial.print(i);
        Serial.println(" Hello");
    }
}
//test
