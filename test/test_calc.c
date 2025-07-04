#include "calculator.h"
#include "fff.h"
#include "myNewStrings.h"
#include "privateCalculator.h"
#include "unity.h"
#include "unity_internals.h"

#include <stdlib.h>

void setUp(void) {
    ;
}

void tearDown(void) {
    ;
}

void isOperator_works(void) {
    TEST_ASSERT_EQUAL(true, isOperator('+'));
    TEST_ASSERT_EQUAL(false, isOperator('\0'));
}

void isNumber_works(void) {
    TEST_ASSERT_EQUAL(true, isNumber('0'));
    TEST_ASSERT_EQUAL(true, isNumber('6'));
    TEST_ASSERT_EQUAL(true, isNumber('1'));
    TEST_ASSERT_EQUAL(true, isNumber('9'));
    TEST_ASSERT_EQUAL(false, isNumber('\0'));
    TEST_ASSERT_EQUAL(false, isNumber('+'));
}

void getFullNumber_works(void) {
    calculatorErr *error = newError();
    char *str1 = "45600.77";
    char *str2 = "0";

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 45600.77, getFullNumber(str1, 0, error));
    TEST_ASSERT_FALSE(error->raised);

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0, getFullNumber(str2, 0, error));
    TEST_ASSERT_FALSE(error->raised);

    clearError(error);
    free(error);
}

void stack_works(void) {
    calculatorErr *error = newError();
    initStack(error);
    float list[] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++) {
        pushToStack(list[i]);
    }

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 4, popFromStack(error));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3, popFromStack(error));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2, popFromStack(error));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1, popFromStack(error));
    TEST_ASSERT_FALSE(error->raised);
}

void reverse_polish_calc_works(void) {
    calculatorErr *error = newError();
    initStack(error);

    char *str1 = "1 2 +";
    char *str2 = "4.2    2 /";
    char *str3 = "1.07               6        + ";
    char *str4 = "3    +       ";
    char *str5 = "1 + 23 ";
    char *str6 = "10 5 - 1 1 + * 1 1 + /";

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3, solveRevPolString(str1, error));
    TEST_ASSERT_FALSE(error->raised);
    resetStack(error);

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.1, solveRevPolString(str2, error));
    TEST_ASSERT_FALSE(error->raised);
    resetStack(error);

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 7.07, solveRevPolString(str3, error));
    TEST_ASSERT_FALSE(error->raised);
    resetStack(error);

    clearError(error);
    solveRevPolString(str4, error);
    TEST_ASSERT_TRUE(error->raised);
    resetStack(error);

    clearError(error);
    solveRevPolString(str5, error);
    TEST_ASSERT_TRUE(error->raised);
    resetStack(error);

    clearError(error);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 5, solveRevPolString(str6, error));
    TEST_ASSERT_FALSE(error->raised);
    resetStack(error);

    clearError(error);
    free(error);
}

void generateOperatorPrecedenceList_works(void) {
    char *str = "(2+3)*89/78 + 9 ^ 2 + 7";
    int solution[] = {16, 5, 8, 2, 12, 20};
    int *generated = generateOperatorPrecedenceList(str);

    for (int i = 0; generated[i] != -1; i++) {
        TEST_ASSERT_EQUAL(solution[i], generated[i]);
    }
}

void operator_precedence_is_correctly_evaluated(void) {
    calculatorErr *error = newError();

    char *str = "2^3 * 3 + 2.5 / 1";
    char *str1 = "2 * (3+4)^2 - 5 / 2.5";

    char *deblanked = removeCharFromString(str, ' ');
    int *list = generateOperatorPrecedenceList(deblanked);
    char *evaluated = evaluateOperatorPrecedence(deblanked, list, error);
    TEST_ASSERT_FALSE(error->raised);
    TEST_ASSERT_EQUAL_STRING("(((2^3)*3)+(2.5/1))", evaluated);
    free(deblanked);
    free(evaluated);
    free(list);

    clearError(error);
    char *deblanked1 = removeCharFromString(str1, ' ');
    int *list1 = generateOperatorPrecedenceList(deblanked1);
    char *evaluated1 = evaluateOperatorPrecedence(deblanked1, list1, error);
    TEST_ASSERT_FALSE(error->raised);
    TEST_ASSERT_EQUAL_STRING("((2*((3+4)^2))-(5/2.5))", evaluated1);
    free(deblanked1);
    free(evaluated1);
    free(list1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(isOperator_works);
    RUN_TEST(isNumber_works);
    RUN_TEST(getFullNumber_works);
    RUN_TEST(stack_works);
    RUN_TEST(reverse_polish_calc_works);
    RUN_TEST(generateOperatorPrecedenceList_works);
    RUN_TEST(operator_precedence_is_correctly_evaluated);

    return UNITY_END();
}
