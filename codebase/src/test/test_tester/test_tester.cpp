#include "test_tester.hpp"
#include "test/Tester.hpp"

#include "self_arduino.hpp"

void test_tester(void)
{
    Tester test_case_0("test_case_0", "Creating and testing the Tester class.");
    test_case_0.begin();
    test_case_0.tester_assert(test_case_0.tester_assert(2,1), false);
    test_case_0.end();
}