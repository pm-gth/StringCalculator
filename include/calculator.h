#ifndef CALCULATOR
#define CALCULATOR

#include <stdbool.h>

typedef struct {
  bool raised;
  char *msg;
} calculatorErr;

calculatorErr *newError();
void clearError(calculatorErr *error);
void setError(calculatorErr *error, const char *fmt, ...);

float revPolishCalc(char *str, calculatorErr *error);
char *formatOperation(char *str, calculatorErr *error);
void initStack(calculatorErr *);
void resetStack(calculatorErr *error);
void infixCalculator(char* str, calculatorErr* error);

#endif
