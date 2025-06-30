#include "calculator.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

#include"privateCalculator.h"
#include"myNewStrings.h"

const char OPERATORS[] = {'+', '-', '*', '/', '^', '%', '\0'};
enum PRECEDENCE {
    ADD = 0,
    SUBTRACT = 0,
    MULTIPLY = 1,
    DIVIDE = 1,
    MODULO = 1,
    POWER = 2
};

float* stackPointer;
int stackIndex = 0;
int stackSize = 5;

// Struct used to set and catch errors
calculatorErr* newError(){
    calculatorErr* error = malloc(sizeof(calculatorErr));
    error->raised = false;
    error->msg = NULL;

    return error;
}

void clearError(calculatorErr* error){
    if(error->msg) {
        free(error->msg);
        error->msg = NULL;
    }
    error->raised = false;
}

void setError(calculatorErr* error, const char* message, ...) {
    if (error->msg) {
        free(error->msg);
        error->msg = NULL;
    }

    va_list args;
    va_start(args, message);

    // Calcular tamaño necesario
    int len = vsnprintf(NULL, 0, message, args);
    va_end(args);

    if (len < 0) return; // Error al formatear

    // Reservar memoria
    error->msg = malloc(len + 1);
    if (!error->msg) return; // Fallo al reservar

    // Escribir mensaje formateado
    // Crea de nuevo los args, ya los gastamos antes
    va_start(args, message);
    // Cada vez que se lee el calor de args este cambia para apuntar al siguiente, así es como se leen todos los args de formato del string
    vsnprintf(error->msg, len + 1, message, args);
    va_end(args);

    error->raised = true;
}

void freeError(calculatorErr* error) {
    clearError(error);
    free(error);
}

// Checks if a char is an operator
bool isOperator(char c){
    bool found = false;
    for(int i = 0; OPERATORS[i] != '\0' && !found; i++){
        if(OPERATORS[i] == c){
            found = true;
        }
    }
    return found;
}

// Checks if a char is part of a number
bool isNumber(char c){
    return ((c >= '0' && c <= '9') || c == '.');
}

// Gets the number inside a string
float getFullNumber(char* str, int pos, calculatorErr* error){
    int initialBufferSize = 10;
    int bufferIndex = 0;
    char* buffer = malloc(sizeof(char)*(initialBufferSize));

    if(!buffer){
        setError(error, "getFullNumber: error, could not allocate memory buffer");
        free(buffer);
        return -1;
        }

    while(isNumber(str[pos]) && str[pos] != '\0'){
        buffer[bufferIndex++] = str[pos++];

        if(bufferIndex >= initialBufferSize){
            initialBufferSize *= 2;
            buffer = realloc(buffer, sizeof(char)*initialBufferSize);

            if(!buffer){
                setError(error, "getFullNumber: error, could not increase memory buffer");
                free(buffer);
                return -1;
            }
        }
    }
    buffer[bufferIndex] = '\0';
    float res = stof(buffer);
    free(buffer);
    return res;
}

// Calculator operations
float add(float a, float b, calculatorErr* error){
    return a+b;
}

float sub(float a, float b, calculatorErr* error){
    return a-b;
}

float mul(float a, float b, calculatorErr* error){
    return a*b;
}

float divi(float a, float b, calculatorErr* error){
    if(b == 0){
        setError(error, "divi: error, division by zero");
        return -1;
    }

    return a/b;
}

float mod(float a, float b, calculatorErr* error){
    return (int)a%(int)b;
}

float power(float base, float exponent, calculatorErr* error){
    float result = 1.0f;
    bool negative = false;

    if (exponent < 0) {
        negative = true;
        exponent = -exponent;
    }

    for (int i = 0; i < exponent; i++) {
        result *= base;
    }

    if (negative) {
        if (base == 0.0f) {
            setError(error, "pow: error, division by zero");
            return -1;
        }
        result = 1.0f / result;
    }
    return result;
}

// Returns a pointer to the respective operation
operation_t parseOperator(char operator, calculatorErr* error){
    operation_t operation;

    switch(operator){
        case '+':
            operation = add;
            break;
        case '-':
            operation = sub;
            break;
        case '*':
            operation = mul;
            break;
        case '/':
            operation = divi;
            break;
        case '%':
            operation = mod;
            break;
        case '^':
            operation = power;
            break;
        default:
            setError(error, "parseOperator: error, %c is a non-valid operator", operator);
            break;
    }
    return operation;
}

// Reverse polish calc, do not forget to call initStack before using it
float revPolishCalc(char* str, calculatorErr* error){
    for(int i = 0; str[i] != '\0'; i++){
        if(isNumber(str[i])){
            float number = getFullNumber(str, i, error);
            while(isNumber(str[i]) && str[i] != '\0') i++;
            i--; //TODO: Change this half-assed fix
            pushToStack(number);
        } else if(isOperator(str[i])){
            if(stackIndex < 2){
                setError(error, "revPolishCalc: error, not enough operands in stack for %c operation", str[i]);
                return -1;
            }
            
            operation_t operation = parseOperator(str[i], error);
            
            //printStack();
            float b = popFromStack(error);
            float a = popFromStack(error);
            float res = operation(a,b, error);

            pushToStack(res);
        } else if(str[i] != ' '){
            setError(error, "revPolishCalc: error, unknown character %c", str[i]);
        }
    }
    return popFromStack(error);
}

// Formats the operation given by the user for printing aesthetics
char* formatOperation(char* str, calculatorErr* error){
    int bufferSize = 10;
    int bufferIndex = 0;
    char* buffer = malloc(sizeof(char)*bufferSize);
    if(!buffer){
        setError(error, "formatOperation: error, could not allocate initial buffer");
        return NULL;
    }

    bool lastWasBlank = false;
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == ' ' && lastWasBlank){
            //Skip this
        } else{
            lastWasBlank = (str[i] == ' ')? true : false;
            buffer[bufferIndex] = str[i];
            bufferIndex++;

            if(bufferIndex >= bufferSize){
                bufferSize *= 2;
                buffer = realloc(buffer, sizeof(char)*bufferSize);

                if(!buffer){
                    setError(error, "formatOperation: error, could not increase buffer");
                    return NULL;
                }
            }
        }
    }
    buffer[bufferIndex] = '\0';
    return buffer;
}

// Starts the stack needed by the polish calculator
void initStack(calculatorErr* error){
    stackPointer = malloc(sizeof(float)*stackSize);

    if(!stackPointer){
        setError(error, "initStack: error, could not allocate initial buffer");
    }
}

void pushToStack(float value){
    stackPointer[stackIndex] = value;
    stackIndex++;

    if(stackIndex >= stackSize){
        stackSize*=2;
        stackPointer = realloc(stackPointer, sizeof(float) * stackSize);
    }
}

float popFromStack(calculatorErr* error){
    if (stackIndex <= 0) {
        setError(error, "popFromStack: error, tried to pop empty stack");
        return 0.0f;
    }

    stackIndex--;
    return stackPointer[stackIndex];
}

void printStack(void){
    int j = 0;
    for(int i = stackIndex; i >= 0; i--){
        printf("{[%d] %f}\n", j, stackPointer[i]);
        j++;
    }
}

void resetStack(calculatorErr* error){
    free(stackPointer);
    stackIndex = 0;
    stackSize = 5;

    initStack(error);
}

// Checks if expression is an atomic value (aka a number) 
bool isAtomic(char* str){
    bool isAtomic = true;

    for(int i = 0; str[i] != '\0'; i++){
        if(!isNumber(str[i])){
            isAtomic = false;
        }
    }

    return isAtomic;
}

#include <string.h>

//Sustituir por cod limpio
char* removeWrappingParennthesis(char* str) {
    int size = strlen(str);
    if (size >= 2 && str[0] == '(' && str[size - 1] == ')') {
        // Crear un nuevo string sin los paréntesis envolventes
        char* newStr = malloc(size - 1);
        if (!newStr) return NULL;  // manejar error
        memcpy(newStr, str + 1, size - 2);
        newStr[size - 2] = '\0';
        return newStr;
    } else {
        // No hay paréntesis envolventes, devolver copia directa
        return strdup(str);
    }
}

// Parses propperly formatted operation and generates binary tree
// This function should NOT receive operations with blanks in between the chars (use removeCharFromString)
// All operations shall have parenthesis (albeit sometimes redundant) so parse user's string through another
// function that marks operator precedence with ()
void buildBinTree(char* str, operationNode** nodeRoot, calculatorErr* error) {
    int level = 0;
    int size = stringSize(str);
    bool foundOperator = false;
   
    // Check parenthesis nesting
    for (int i = 0; i < size; i++) {
        if (str[i] == '(') {
            level++;
        } else if (str[i] == ')') {
            level--;
            if (level < 0) {
                setError(error, "buildBinTree: error, found uncoupled parenthesis");
                return;
            }
        } else if (isOperator(str[i]) && level == 0) {
            // Found external operator
            foundOperator = true;

            // Look for invalid operator scenarios and leave
            if(i-1 < 0){
                setError(error, "buildBinTree: error, invalid expression: %s, started with operator", str);
                return;
            }

            if(i+1 >= size){
                setError(error, "buildBinTree: error, invalid expression: %s, ended in operator", str);
                return;
            }

            // Create the node
            *nodeRoot = malloc(sizeof(operationNode));
            if (!*nodeRoot) {
                setError(error, "buildBinTree: error, could not allocate memory for node");
                return;
            }

             // Init node
            (*nodeRoot)->operation = parseOperator(str[i], error);
            (*nodeRoot)->operatorChar = str[i];
            (*nodeRoot)->atomicA = 0;
            (*nodeRoot)->atomicB = 0;
            (*nodeRoot)->operandExpressionA = NULL;
            (*nodeRoot)->operandExpressionB = NULL;

            // Split the string in two from the operator
            char* firstHalf = stringSplicer(str, 0, i-1);
            char* secondHalf = stringSplicer(str, i+1, size-1);

            // Check if branch A is an atomic expression, else point it to another node
            if (isAtomic(firstHalf)) {
                (*nodeRoot)->atomicA = getFullNumber(firstHalf, 0, error);
                if(error->raised){
                    setError(error, "buildBinTree: error, could not determine atomicA: %s", firstHalf);
                    free(*nodeRoot);
                    free(firstHalf);
                    free(secondHalf);
                    return;
                }
            } else {
                char* unwrappedFirstHalf = removeWrappingParennthesis(firstHalf);
                printf("%s\n", unwrappedFirstHalf);
                buildBinTree(unwrappedFirstHalf, &((*nodeRoot)->operandExpressionA), error);
                free(unwrappedFirstHalf);
            }

            // Check if branch B is an atomic expression, else point it to another node
            if (isAtomic(secondHalf)) {
                (*nodeRoot)->atomicB = getFullNumber(secondHalf, 0, error);
                if(error->raised){
                    setError(error, "buildBinTree: error, could not determine atomicB: %s", secondHalf);
                    free(*nodeRoot);
                    free(firstHalf);
                    free(secondHalf);
                    return;
                }
            } else {
                char* unwrappedsecondHalf = removeWrappingParennthesis(secondHalf);
                printf("%s\n", unwrappedsecondHalf);
                buildBinTree(unwrappedsecondHalf, &((*nodeRoot)->operandExpressionB), error);
                free(unwrappedsecondHalf);
            }

            free(firstHalf);
            free(secondHalf);
            return;
        }
    }
    // If couldn't find any operator, unwrap and try again
    if(!foundOperator){
        char* newUnwrapped = removeWrappingParennthesis(str);
        if(sameString(str, newUnwrapped)){
            // Do nothing, this is a non-valid string
        } else{
            // Try again
            buildBinTree(newUnwrapped, nodeRoot, error);
            free(newUnwrapped);
        }
    }
}

// Prints the binary tree generated by buildBinTree for debugging purposes
void printTree(operationNode* root, int level) {
    if (!root) {
        for (int i = 0; i < level; i++) printf("    ");
        printf("EMPTY_BRANCH\n");
        return;
    }

    // Print node and atoms
    for (int i = 0; i < level; i++) printf("    ");
    printf("oper: %c, atmA: %.2f, atmB: %.2f\n", root->operatorChar, root->atomicA, root->atomicB);

    // Print left (A) branch
    printTree(root->operandExpressionA, level + 1);

    // Print right (B) branch
    printTree(root->operandExpressionB, level + 1);
}

// Frees the memory sapce to which the binary tree was allocated
void freeTree(operationNode* node) {
    if (!node) return;

    // Free children
    freeTree(node->operandExpressionA);
    freeTree(node->operandExpressionB);

    // Free parent
    free(node);
}

char* wrapInParenthesis(char* str, int start, int end, calculatorErr* error){
    int size = stringSize(str);
    
    if(start < 0 || start > end || end > size){
        setError(error, "wrapInParenthesis: error, invalid range for start:%d, end:%d in string of size:%d", start, end, size);
        return NULL;
    }

    char* strFirstPar = insertCharInString(str, '(', start);
    char* wrappedStr = insertCharInString(strFirstPar, ')', end);
    free(strFirstPar);

    return wrappedStr;
}

void formatInfixOperation(char* str, calculatorErr* error){
    int nestingLevel = 0;
    int maxNestingLevel = 0;
    
    char* unBlankedString = removeCharFromString(str, ' ');
    int size = stringSize(unBlankedString);

    // Check for nesting errors and get max nestingLevel
    for (int i = 0; i < size; i++) {
        if (unBlankedString[i] == '(') {
            nestingLevel++;
            if(nestingLevel > maxNestingLevel){
                maxNestingLevel = nestingLevel;
            }
        } else if (unBlankedString[i] == ')') {
            nestingLevel--;
            if (nestingLevel < 0) {
                setError(error, "buildBinTree: error, found uncoupled parenthesis");
                return;
            }
        }
    }
}