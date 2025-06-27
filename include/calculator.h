#ifndef CALCULATOR
#define CALCULATOR

#include<stdbool.h>

typedef struct{
    bool raised;
    char* msg;
} calculatorErr;

calculatorErr* newError();
void clearError(calculatorErr* error);
void setError(calculatorErr* error, const char* fmt, ...);
bool isOperator(char c);
bool isNumber(char c);
float getFullNumber(char* str, int pos, calculatorErr* error);
float add(float a, float b, calculatorErr* error);
float sub(float a, float b, calculatorErr* error);
float mul(float a, float b, calculatorErr* error);
float divi(float a, float b, calculatorErr* error);
float mod(float a, float b, calculatorErr* error);
float power(float base, float exponent, calculatorErr* error);
float parseString(char* str, calculatorErr* error);
char* formatOperation(char* str, calculatorErr* error);
void initStack(calculatorErr*);
void pushToStack(float value);
float popFromStack(calculatorErr* error);
void printStack(void);

#endif
