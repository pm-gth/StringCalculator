#ifndef CALCULATOR
#define CALCULATOR

#include <stdbool.h>

// Error handling
typedef struct {
  bool raised;
  char *msg;
} calculatorErr;

calculatorErr *newError();
void clearError(calculatorErr *error);
void setError(calculatorErr *error, const char *fmt, ...);
void clearError(calculatorErr *error);
void freeError(calculatorErr *error);

// Reverse-Polish calculator
float reversePolishCalculator(char* str, calculatorErr* error);
char *fancyRevPolOperation(char *str, calculatorErr *error);

// Infix calculator
float infixCalculator(char* str, calculatorErr* error);

#endif
