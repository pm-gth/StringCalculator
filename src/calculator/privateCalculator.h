#ifndef PRIVATECALCULATOR
#define PRIVATECALCULATOR

#include "calculator.h"
#include <stdbool.h>
/*
    Práctica estándar en C, dos headers, uno con las variables y métodos
   públicos, el otro con los privados. Para evitar errores de redefinición de
   símbolos, el header privado incluye al público, el archivo fuente (el .c)
   solo incluye al header privado, el main.c al público
*/

// Constants and global vars
extern const char OPERATORS[];
extern float *stackPointer;
extern int stackIndex;
extern int stackSize;

typedef float (*operation_t)(float, float, calculatorErr *);

// Node for the infix binary tree (written operationNode twice for self-referencing)
typedef struct operationNode {
  float result;
  struct operationNode* parentNode;
  operation_t operation;
  char operatorChar; // For print and debug purposes
  struct operationNode *operandExpressionA;
  struct operationNode *operandExpressionB;
  float atomicA;
  float atomicB;
} operationNode;

bool isOperator(char c);
bool isNumber(char c);
float getFullNumber(char *str, int pos, calculatorErr *error);

float add(float a, float b, calculatorErr *error);
float sub(float a, float b, calculatorErr *error);
float mul(float a, float b, calculatorErr *error);
float divi(float a, float b, calculatorErr *error);
float mod(float a, float b, calculatorErr *error);
float power(float base, float exponent, calculatorErr *error);
operation_t parseOperator(char operator, calculatorErr * error);

// Stack control for the RPolish calc
void pushToStack(float value);
float popFromStack(calculatorErr *error);
void initStack(calculatorErr *);
void resetStack(calculatorErr *error);
void printStack(void);

float solveRevPolString(char *str, calculatorErr *error); // Internal operation of RP-calc

// Used in infix to resolve operator precedence
char *wrapInParenthesis(char *str, int start, int end, calculatorErr *error);
int *generateOperatorPrecedenceList(char *str);
void updatePrecedenceList(int *list, int lowerParenthesis, int upperParenthesis);
char *evaluateOperatorPrecedence(char *str, int *operatorList, calculatorErr* error);

// Infix binary tree
void buildBinTree(char *str, operationNode **nodeRoot, calculatorErr *error);
void printTree(operationNode *root, int lvl);
void freeTree(operationNode *node);

#endif