#include "fff.h"
#include "privateCalculator.h"
#include "unity.h"
#include "unity_internals.h"

#include <stdlib.h>

void setUp(void) { ; }

void tearDown(void) { ; }

void isOperator_detects_chars_correctly(void) {
  TEST_ASSERT_EQUAL(true, isOperator('+'));
  TEST_ASSERT_EQUAL(false, isOperator('\0'));
}

void isNumber_detects_chars_correctly(void) {
  TEST_ASSERT_EQUAL(true, isNumber('0'));
  TEST_ASSERT_EQUAL(true, isNumber('6'));
  TEST_ASSERT_EQUAL(true, isNumber('1'));
  TEST_ASSERT_EQUAL(true, isNumber('9'));
  TEST_ASSERT_EQUAL(false, isNumber('\0'));
  TEST_ASSERT_EQUAL(false, isNumber('+'));
}

void getFullNumber_expands_numbers_correctly(void) {
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

void parseString_does_operation_correctly(void) {
  calculatorErr *error = newError();
  initStack(error);

  char *str1 = "1 2 +";
  char *str2 = "4.2    2 /";
  char *str3 = "1.07               6        + ";
  char *str4 = "3    +       ";
  char *str5 = "1 + 23 ";
  char *str6 = "10 5 - 1 1 + * 1 1 + /";

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
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 5, revPolishCalc(str6, error));
  TEST_ASSERT_FALSE(error->raised);
  resetStack(error);

  clearError(error);
  free(error);
}

void stack_works_propperly(void) {
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

void generateOperatorPrecedenceList_works(void){
  char* str = "(2+3)*89/78 + 9 ^ 2 + 7";
  int solution[] = {16,5,8,2,12,20};
  int* generated = generateOperatorPrecedenceList(str);

  for(int i = 0; i < 6; i++){
    TEST_ASSERT_EQUAL(solution[i], generated[i]);
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(isOperator_detects_chars_correctly);
  RUN_TEST(isNumber_detects_chars_correctly);
  RUN_TEST(getFullNumber_expands_numbers_correctly);
  RUN_TEST(parseString_does_operation_correctly);
  RUN_TEST(stack_works_propperly);
  RUN_TEST(generateOperatorPrecedenceList_works);

  return UNITY_END();
}
