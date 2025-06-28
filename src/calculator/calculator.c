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

bool isAtomic(char* str){
    bool isAtomic = true;

    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] != ' ' && !isNumber(str[i])){
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

void buildBinTree(char* str, operationNode** nodeRoot, calculatorErr* error) {
    int level = 0;
    int size = stringSize(str);
   
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
            *nodeRoot = malloc(sizeof(operationNode));
            if (!*nodeRoot) {
                setError(error, "buildBinTree: error, could not allocate memory for node");
                return;
            }
            (*nodeRoot)->operation = parseOperator(str[i], error);
            (*nodeRoot)->operatorChar = str[i];
            (*nodeRoot)->atomicA = 0;
            (*nodeRoot)->atomicB = 0;
            (*nodeRoot)->operandA = NULL;
            (*nodeRoot)->operandB = NULL;

            if(i-1 < 0){
                setError(error, "buildBinTree: error, invalid expression: %s, started with operator", str);
                return;
            }

            if(i+1 >= size){
                setError(error, "buildBinTree: error, invalid expression: %s, ended in operator", str);
                return;
            }

            char* firstHalf = stringSplicer(str, 0, i-1);
            char* secondHalf = stringSplicer(str, i+1, size-1);

            if (isAtomic(firstHalf)) {
                (*nodeRoot)->atomicA = getFullNumber(firstHalf, 0, error);
            } else {
                firstHalf = removeWrappingParennthesis(firstHalf);
                printf("%s\n", firstHalf);
                buildBinTree(firstHalf, &((*nodeRoot)->operandA), error);
            }

            if (isAtomic(secondHalf)) {
                (*nodeRoot)->atomicB = getFullNumber(secondHalf, 0, error);
            } else {
                secondHalf = removeWrappingParennthesis(secondHalf);
                printf("%s\n", secondHalf);
                buildBinTree(secondHalf, &((*nodeRoot)->operandB), error);
            }

            free(firstHalf);
            free(secondHalf);
            return;
        }
    }
}

//Sustituir por cod limpio
void printTree(operationNode* root, int level) {
    if (!root) {
        for (int i = 0; i < level; i++) printf("    ");
        printf("EMPTY_BRANCH\n");
        return;
    }

    // Imprimir nodo con indentación
    for (int i = 0; i < level; i++) printf("    ");
    printf("oper: %c, atmA: %.2f, atmB: %.2f\n", root->operatorChar, root->atomicA, root->atomicB);

    // Imprimir rama izquierda (operandA)
    printTree(root->operandA, level + 1);

    // Imprimir rama derecha (operandB)
    printTree(root->operandB, level + 1);
}




