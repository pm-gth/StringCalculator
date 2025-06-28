#include "unity.h"
#include "fff.h"
#include "calculator.h"

#include<stdlib.h>

void setUp(void) {
    ;
}

void tearDown(void) {
    ;
}

void isOperator_detects_chars_correctly(void){
    TEST_ASSERT_EQUAL(true, isOperator('+'));
    TEST_ASSERT_EQUAL(false, isOperator('\0'));
}

void isNumber_detects_chars_correctly(void){
    TEST_ASSERT_EQUAL(true, isNumber('0'));
    TEST_ASSERT_EQUAL(true, isNumber('6'));
    TEST_ASSERT_EQUAL(true, isNumber('1'));
    TEST_ASSERT_EQUAL(true, isNumber('9'));
    TEST_ASSERT_EQUAL(false, isNumber('\0'));
    TEST_ASSERT_EQUAL(false, isNumber('+'));
}

void getFullNumber_expands_numbers_correctly(void) {
    calculatorErr* error = newError();
    char* str1 = "45600.77";
    char* str2 = "0";

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 45600.77, getFullNumber(str1,0, error));
    TEST_ASSERT_FALSE(error->raised);

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0, getFullNumber(str2,0, error));
    TEST_ASSERT_FALSE(error->raised);
    
    clearError(error);
    free(error);
}

void parseString_does_operation_correctly(void){
    calculatorErr* error = newError();
    initStack(error);

    char* str1 = "1 2 +";
    char* str2 = "4.2    2 /";
    char* str3 = "1.07               6        + ";
    char* str4 = "3    +       ";
    char* str5 = "1 + 23 ";

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3, revPolishCalc(str1, error));
    TEST_ASSERT_FALSE(error->raised);
    resetStack(error);

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.1, revPolishCalc(str2, error));
    TEST_ASSERT_FALSE(error->raised);
    resetStack(error);

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 7.07, revPolishCalc(str3, error));
    TEST_ASSERT_FALSE(error->raised);
    resetStack(error);

    clearError(error);
    revPolishCalc(str4, error);
    TEST_ASSERT_TRUE(error->raised);
    resetStack(error);

    clearError(error);
    revPolishCalc(str5, error);
    TEST_ASSERT_TRUE(error->raised);
    resetStack(error);

    clearError(error);
    free(error);
}

void formatOperation_formats_strings_correctly(void){
    calculatorErr* error = newError();
    char* str1 = "23         +6";
    char* str2 = "5+5";

    clearError(error);
    TEST_ASSERT_EQUAL_STRING("23+6",formatOperation(str1, error));
    TEST_ASSERT_FALSE(error->raised);

    clearError(error);
    TEST_ASSERT_EQUAL_STRING("5+5",formatOperation(str2, error));
    TEST_ASSERT_FALSE(error->raised);

    clearError(error);
    free(error);
}

void stack_works_propperly(void){
    calculatorErr* error = newError();
    initStack(error);
    float list[] = {1,2,3,4};

    for(int i = 0; i < 4; i++){
        pushToStack(list[i]);
    }

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 4, popFromStack(error));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3, popFromStack(error));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2, popFromStack(error));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1, popFromStack(error));
    TEST_ASSERT_FALSE(error->raised);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(isOperator_detects_chars_correctly);
    RUN_TEST(isNumber_detects_chars_correctly);
    RUN_TEST(getFullNumber_expands_numbers_correctly);
    RUN_TEST(parseString_does_operation_correctly);
    //RUN_TEST(formatOperation_formats_strings_correctly);
    RUN_TEST(stack_works_propperly);
    return UNITY_END();
}
