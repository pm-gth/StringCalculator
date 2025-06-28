#include "calculator.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

#include"privateCalculator.h"
#include"myNewStrings.h"

const char OPERATORS[] = {'+', '-', '*', '/', '^', '%', '\0'};
float* stackPointer;
int stackIndex = 0;
int stackSize = 5;

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

bool isOperator(char c){
    bool found = false;
    for(int i = 0; OPERATORS[i] != '\0' && !found; i++){
        if(OPERATORS[i] == c){
            found = true;
        }
    }
    return found;
}

bool isNumber(char c){
    return ((c >= '0' && c <= '9') || c == '.');
}

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

void infixCalc(char* str, calculatorErr* error){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == '('){
            //Entered inside parenthesis, skip it
            while(str[i] != ')' && str[i] != '\0') i++;
            if(str[i] == '\0'){
                setError(error, "infixCalc: error, found uncoupled parenthesis");
                return;
            }
        } else if(isOperator(str[i])){
            //Found External operator, create a tree from it, then split the string
            operationNode* parentNode = malloc(sizeof(operationNode));
            if(!parentNode){
                setError(error, "infixCalc: error, could not allocate memory");
                return;
            }
        }
    }
}