#ifndef TEST_TESTER
#define TEST_TESTER

#include <iostream>
#include <typeinfo>

#include "self_arduino.hpp"

enum status
{
    NOT_INIT = -1,
    INIT = 0,
    STARTED,
    ENDED
};

class Tester
{
    private:
    String test_name;
    String test_description;
    status test_status = NOT_INIT;
    unsigned long startTime;
    unsigned long stopTime;
    unsigned int assert_calls = 0;

    void printTime(void)
    {
        Serial.print("Elapsed time: ");
        Serial.print((stopTime-startTime));
        Serial.print(" ms | time per assert: ");
        if(assert_calls > 0)
        {
            Serial.print((stopTime-startTime)/assert_calls);
            Serial.println(" ms");
        }
    }

    public:
    Tester(String name, String description)
    {
        test_name = name;
        test_description = description;
        test_status = INIT;
    }

    void begin()
    {
        Serial.println("\nStarting " + test_name);
        Serial.println("######################");
        test_status = STARTED;
        startTime = millis();
    }
    void end()
    {
        stopTime = millis();
        Serial.println("######################");
        Serial.println("Ending " + test_name);
        printTime();
        test_status = ENDED;
    }

    String retMessage(String statement_1, String statement_2, bool statement_result)
    {
        String result = "";

        if(statement_result)
        {
            result = "SUCCESS ✔";
        }
        else
        {
            result = "FAILURE ❌";
            result += "\nstatement_1 = " + (statement_1);
            result += "\nstatement_2 = " + (statement_2);
        }
        result = test_name + " finished with: " + result;
        Serial.println(result);

        return result;
    }

    template <typename T, typename U, typename Compare = std::function<bool(T, U)>>
    bool tester_assert(T statement_1, U statement_2, Compare compare = [](T statement_1, U statement_2) { return statement_1 == statement_2; })
    {
        assert_calls += 1;
        bool ret = compare(statement_1 ,statement_2);

        retMessage(String(statement_1), String(statement_2), ret);

        return ret;
    }
    template <typename T, typename U>
    bool tester_assert_not(T statement_1, U statement_2)
    {
        return tester_assert(statement_1, statement_2, [](T statement_1, U statement_2) { return statement_1 != statement_2; });
    }

    String getTestName()
    {
        return test_name;
    }
    String getTestDescription()
    {
        return test_description;
    }

};

#endif /* TEST_TESTER */