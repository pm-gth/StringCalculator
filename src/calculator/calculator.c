#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "myNewStrings.h"
#include "calculator.h"
#include "privateCalculator.h"

const char OPERATORS[] = {'+', '-', '*', '/', '^', '%', '\0'};

float *stackPointer;
int stackIndex = 0;
int stackSize = 5;

// Struct used to set and catch errors
calculatorErr *newError() {
    calculatorErr *error = malloc(sizeof(calculatorErr));
    error->raised = false;
    error->msg = NULL;

    return error;
}

void clearError(calculatorErr *error) {
    if (error->msg) {
        free(error->msg);
        error->msg = NULL;
    }
    error->raised = false;
}

void setError(calculatorErr *error, const char *message, ...) {
    if (error->msg) {
        free(error->msg);
        error->msg = NULL;
    }

    va_list args;
    va_start(args, message);

    // Get message size
    int len = vsnprintf(NULL, 0, message, args);
    va_end(args);

    if (len < 0)
        return;

    // Allocate message mem
    error->msg = malloc(len + 1);
    if (!error->msg)
        return;

    // Create arguments again
    va_start(args, message);
    
    // Store message with args
    vsnprintf(error->msg, len + 1, message, args);
    va_end(args);

    error->raised = true;
}

void freeError(calculatorErr *error) {
    clearError(error);
    free(error);
}

// Checks if a char is an operator
bool isOperator(char c) {
    bool found = false;
    for (int i = 0; OPERATORS[i] != '\0' && !found; i++) {
        if (OPERATORS[i] == c) {
            found = true;
        }
    }
    return found;
}

// Checks if a char is part of a number
bool isNumber(char c) {
    return ((c >= '0' && c <= '9') || c == '.');
}

// Gets the number inside a string
float getFullNumber(char *str, int pos, calculatorErr *error) {
    int initialBufferSize = 10;
    int bufferIndex = 0;
    char *buffer = malloc(sizeof(char) * (initialBufferSize));

    if (!buffer) {
        setError(error, "getFullNumber: error, could not allocate memory buffer");
        free(buffer);
        return -1;
    }

    while (isNumber(str[pos]) && str[pos] != '\0') {
        buffer[bufferIndex++] = str[pos++];

        if (bufferIndex >= initialBufferSize) {
            initialBufferSize *= 2;
            buffer = realloc(buffer, sizeof(char) * initialBufferSize);

            if (!buffer) {
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
float add(float a, float b, calculatorErr *error) {
    return a + b;
}

float sub(float a, float b, calculatorErr *error) {
    return a - b;
}

float mul(float a, float b, calculatorErr *error) {
    return a * b;
}

float divi(float a, float b, calculatorErr *error) {
    if (b == 0) {
        setError(error, "divi: error, division by zero");
        return -1;
    }

    return a / b;
}

float mod(float a, float b, calculatorErr *error) {
    return (int)a % (int)b;
}

float power(float base, float exponent, calculatorErr *error) {
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
operation_t parseOperator(char operator, calculatorErr * error) {
    operation_t operation;

    switch (operator) {
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
float solveRevPolString(char *str, calculatorErr *error) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (isNumber(str[i])) {
            float number = getFullNumber(str, i, error);
            while (isNumber(str[i]) && str[i] != '\0')
                i++;
            i--;
            pushToStack(number);
        } else if (isOperator(str[i])) {
            if (stackIndex < 2) {
                setError(error,
                         "revPolishCalc: error, not enough operands in stack for %c "
                         "operation",
                         str[i]);
                return -1;
            }

            operation_t operation = parseOperator(str[i], error);

            // printStack();
            float b = popFromStack(error);
            float a = popFromStack(error);
            float res = operation(a, b, error);

            pushToStack(res);
        } else if (str[i] != ' ') {
            setError(error, "revPolishCalc: error, unknown character %c", str[i]);
        }
    }
    return popFromStack(error);
}

// Formats the operation given by the user for printing aesthetics
char *fancyRevPolOperation(char *str, calculatorErr *error) {
    int bufferSize = 10;
    int bufferIndex = 0;
    char *buffer = malloc(sizeof(char) * bufferSize);
    if (!buffer) {
        setError(error, "formatOperation: error, could not allocate initial buffer");
        return NULL;
    }

    bool lastWasBlank = false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ' && lastWasBlank) {
            // Skip this
        } else {
            lastWasBlank = (str[i] == ' ') ? true : false;
            buffer[bufferIndex] = str[i];
            bufferIndex++;

            if (bufferIndex >= bufferSize) {
                bufferSize *= 2;
                buffer = realloc(buffer, sizeof(char) * bufferSize);

                if (!buffer) {
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
void initStack(calculatorErr *error) {
    stackPointer = malloc(sizeof(float) * stackSize);

    if (!stackPointer) {
        setError(error, "initStack: error, could not allocate initial buffer");
    }
}

void pushToStack(float value) {
    stackPointer[stackIndex] = value;
    stackIndex++;

    if (stackIndex >= stackSize) {
        stackSize *= 2;
        stackPointer = realloc(stackPointer, sizeof(float) * stackSize);
    }
}

float popFromStack(calculatorErr *error) {
    if (stackIndex <= 0) {
        setError(error, "popFromStack: error, tried to pop empty stack");
        return 0.0f;
    }

    stackIndex--;
    return stackPointer[stackIndex];
}

void printStack(void) {
    int j = 0;
    for (int i = stackIndex; i >= 0; i--) {
        printf("{[%d] %f}\n", j, stackPointer[i]);
        j++;
    }
}

void resetStack(calculatorErr *error) {
    free(stackPointer);
    stackIndex = 0;
    stackSize = 5;

    initStack(error);
}

// Checks if expression is an atomic value (aka a number)
bool isAtomic(char *str) {
    bool isAtomic = true;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isNumber(str[i])) {
            isAtomic = false;
        }
    }

    return isAtomic;
}

// Unwraps one level of parenthesis
char *removeWrappingParennthesis(char *str) {
    int size = stringSize(str);
    if (size >= 2 && str[0] == '(' && str[size - 1] == ')') {
        // New string without parenthesis
        char *newStr = malloc(size - 1);
        if (!newStr) return NULL;
        
        subStringCopy(str, newStr, 1, size-2);
        
        newStr[size - 2] = '\0';
        return newStr;
    } else {
        // No wrapping parenthesis dound, return a copy
        return stringClone(str);
    }
}

// Parses propperly formatted operation and generates binary tree
// This function should NOT receive operations with blanks in between the chars
// (use removeCharFromString) All operations shall have parenthesis (albeit
// sometimes redundant) so parse user's string through another function that
// marks operator precedence with ()
void buildBinTree(char *str, operationNode **nodeRoot, calculatorErr *error) {
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
                freeTree(*nodeRoot);
                return;
            }
        } else if (isOperator(str[i]) && level == 0) {
            // Found external operator
            foundOperator = true;

            // Look for invalid operator scenarios and leave
            if (i - 1 < 0) {
                setError(error, "buildBinTree: error, invalid expression: %s, started with operator", str);
                freeTree(*nodeRoot);
                return;
            }

            if (i + 1 >= size) {
                setError(error, "buildBinTree: error, invalid expression: %s, ended in operator", str);
                freeTree(*nodeRoot);
                return;
            }

            // Create the node
            *nodeRoot = malloc(sizeof(operationNode));
            if (!*nodeRoot) {
                setError(error, "buildBinTree: error, could not allocate memory for node");
                freeTree(*nodeRoot);
                return;
            }

            // Init node
            (*nodeRoot)->operation = parseOperator(str[i], error);
            (*nodeRoot)->operatorChar = str[i];
            (*nodeRoot)->result = 0;
            (*nodeRoot)->atomicA = 0;
            (*nodeRoot)->atomicB = 0;
            (*nodeRoot)->operandExpressionA = NULL;
            (*nodeRoot)->operandExpressionB = NULL;

            // Split the string in two from the operator
            char *firstHalf = stringSplicer(str, 0, i - 1);
            char *secondHalf = stringSplicer(str, i + 1, size - 1);

            // Check if branch A is an atomic expression, else point it to another
            // node
            if (isAtomic(firstHalf)) {
                (*nodeRoot)->atomicA = getFullNumber(firstHalf, 0, error);
                if (error->raised) {
                    setError(error, "buildBinTree: error, could not determine atomicA: %s", firstHalf);
                    freeTree(*nodeRoot);
                    return;
                }
            } else {
                char *unwrappedFirstHalf = removeWrappingParennthesis(firstHalf);
                printf("%s\n", unwrappedFirstHalf);
                buildBinTree(unwrappedFirstHalf, &((*nodeRoot)->operandExpressionA), error);
 
                // Point the new node to its father
                (*nodeRoot)->operandExpressionA->parentNode = *nodeRoot;

                free(unwrappedFirstHalf);
            }

            // Check if branch B is an atomic expression, else point it to another
            // node
            if (isAtomic(secondHalf)) {
                (*nodeRoot)->atomicB = getFullNumber(secondHalf, 0, error);
                if (error->raised) {
                    setError(error, "buildBinTree: error, could not determine atomicB: %s", secondHalf);
                    freeTree(*nodeRoot);
                    return;
                }
            } else {
                char *unwrappedsecondHalf = removeWrappingParennthesis(secondHalf);
                printf("%s\n", unwrappedsecondHalf);
                buildBinTree(unwrappedsecondHalf, &((*nodeRoot)->operandExpressionB), error);

                // Point the new node to its father
                (*nodeRoot)->operandExpressionB->parentNode = *nodeRoot;

                free(unwrappedsecondHalf);
            }

            free(firstHalf);
            free(secondHalf);
            return;
        }
    }
    // If couldn't find any operator, unwrap and try again
    if (!foundOperator) {
        char *newUnwrapped = removeWrappingParennthesis(str);
        if (sameString(str, newUnwrapped)) {
            // Do nothing, this is a non-valid string
        } else {
            // Try again
            buildBinTree(newUnwrapped, nodeRoot, error);
            free(newUnwrapped);
        }
    }
}

// Prints the binary tree generated by buildBinTree for debugging purposes
void printTree(operationNode *root, int level) {
    if (!root) {
        for (int i = 0; i < level; i++)
            printf("    ");
        printf("EMPTY_BRANCH\n");
        return;
    }

    // Print node and atoms
    for (int i = 0; i < level; i++)
        printf("    ");
    printf("oper: %c, atmA: %.2f, atmB: %.2f, res: %.2f\n", root->operatorChar, root->atomicA, root->atomicB, root->result);

    // Print left (A) branch
    printTree(root->operandExpressionA, level + 1);

    // Print right (B) branch
    printTree(root->operandExpressionB, level + 1);
}

// Frees the memory sapce to which the binary tree was allocated
void freeTree(operationNode *node) {
    if (!node)
        return;

    // Free children
    freeTree(node->operandExpressionA);
    freeTree(node->operandExpressionB);

    // Free parent
    free(node);
}

char *wrapInParenthesis(char *str, int start, int end, calculatorErr *error) {
    int size = stringSize(str);

    if (start < 0 || start > end || end > size) {
        setError(error, "wrapInParenthesis: error, invalid range for start:%d, end:%d in string of size:%d", start, end, size);
        return NULL;
    }

    char *strFirstPar = insertCharInString(str, '(', start);
    if (!strFirstPar) {
        setError(error, "wrapInParenthesis: error replacing left parenthesis");
        return NULL;
    }

    char *wrappedStr = insertCharInString(strFirstPar, ')', end + 1); // End was displaced by (
    if (!wrappedStr) {
        setError(error, "wrapInParenthesis: error replacing right parenthesis");
        return NULL;
    }

    free(strFirstPar);

    return wrappedStr;
}

// Returns operator's precedence
int getOperatorPrecedence(char operator) {
    int res = 0;

    switch (operator) {
    case '+':
        res = 0;
        break;
    case '-':
        res = 0;
        break;
    case '*':
        res = 1;
        break;
    case '/':
        res = 1;
        break;
    case '%':
        res = 1;
        break;
    case '^':
        res = 2;
        break;
    default:
        res = -2;
        break;
    }

    return res;
}

// List of operators, ordered by preference and indicated by their possition
// inside the string, delimited by -1
// MUST receive a string with no blanks
int *generateOperatorPrecedenceList(char *str) {
    int numberOfOperators = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isOperator(str[i])) {
            numberOfOperators++;
        }
    }

    int *list = malloc(sizeof(int) * (numberOfOperators + 1)); // for delimiter
    if (!list)
        return NULL;
    int listIndex = 0;

    // Get operators from all preference levels, already sorted
    for (int precedence = 2; precedence >= 0; precedence--) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (isOperator(str[i]) && getOperatorPrecedence(str[i]) == precedence) {
                list[listIndex] = i;
                listIndex++;
            }
        }
    }

    list[listIndex] = -1;

    return list;
}

// Update operators positions in the string after wrapping with parenthesis
void updatePrecedenceList(int *list, int lowerParenthesis, int upperParenthesis) {
    for (int i = 0; list[i] != -1; i++) {
        // operator after both parentheses
        if (list[i] >= upperParenthesis) {
            list[i] += 2;
            // operator between both parentheses
        } else if (list[i] >= lowerParenthesis) {
            list[i] += 1;
        }
        // does not change
    }
}

// Adds necessary parenthesis and resolves operation precedence for the main
// function MUST receive a string with no blanks
char *evaluateOperatorPrecedence(char *input, int *operatorList, calculatorErr *error) {
    int level = 0; 
    
    // Check if all parenthesis are coupled and no strange chars are found
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '(') {
            level++;
        } else if (input[i] == ')') {
            level--;
            if (level < 0) {
                setError(error, "evaluateOperatorPrecedence: error, found uncoupled parenthesis");
                return NULL;
            }
        // Unknown char found, set error
        } else if(!isNumber(input[i]) && !isOperator(input[i])){
            setError(error, "evaluateOperatorPrecedence: error, strange char '%c' found in string", input[i]);
            return NULL;
        }
    }

    // Make a copy of the original string to modify it
    int size = stringSize(input);
    char* str = malloc(sizeof(char)*(size+1));
    stringCopy(input, str);

    if(stringSize(str) != stringSize(input)){
        setError(error, "evaluateOperatorPrecedence: error, duplicated string differs in size from the original one");
        free(str);
        return NULL;
    }
    
    // Check both operators, two scenarios, they are atomic or they are expressions
    // Needed or not we save the position the parentheses would occupy in case the other side needs them
    int lowerParenthesis;
    int upperParenthesis;
    bool needsLower;
    bool needsUpper;

    for (int j = 0; operatorList[j] != -1; j++) {
        int pos = operatorList[j];
        // Check left operand
        int i = pos - 1;
        lowerParenthesis = -1;
        needsLower = false;
        // Left operand is atomic
        if (isNumber(str[i])) {
            while (i > 0 && isNumber(str[i - 1])) {
                i--;
            }
            if (i > 0 && str[i - 1] == '(') {
                // Already inside parenthesis
                lowerParenthesis = i;
            } else {
                lowerParenthesis = i;
                needsLower = true;
            }
            // Left operand is expression
        } else if (str[i] == ')') {
            level = 1;
            i--;    // Advance to the next character
            while (level != 0 && i >= 0) {
                if (str[i] == ')')
                    level++;
                else if (str[i] == '(')
                    level--;
                i--;
            }
            if (i >= 0 && str[i] == '(') {
                // Already inside parenthesis
                lowerParenthesis = i + 1;
            } else {
                lowerParenthesis = i + 1; // Move away from expression parenthesis
                needsLower = true;
            }
        }

        // Check right operand
        i = pos + 1;
        int size = stringSize(str);
        upperParenthesis = -1;
        needsUpper = false;
        // Right operand is atomic
        if (isNumber(str[i])) {
            while (i < size - 1 && isNumber(str[i + 1])) {
                i++;
            }
            if (i < size - 1 && str[i + 1] == ')') {
                // Already inside parenthesis
                upperParenthesis = i + 1;
            } else {
                upperParenthesis = i + 1;
                needsUpper = true;
            }
            // Right operand is expression
        } else if (str[i] == '(') {
            level = 1;
            i++;    // Advance to the next character
            while (level != 0 && i < size) {
                if (str[i] == '(')
                    level++;
                else if (str[i] == ')')
                    level--;
                i++;
            }
            if (i < size && str[i] == ')') {
                // Already inside parenthesis
                upperParenthesis = i;
            } else {
                upperParenthesis = i;
                needsUpper = true;
            }
        }

        // Wrap if necessary, then update operator positions
        // If at least one of the sides need wrapping, wrap
        if (needsUpper || needsLower) {
            if (upperParenthesis == -1 || lowerParenthesis == -1) {
                setError(error, "formatStringForInfix: error, %s parenthesis for operator in pos %d(%c) could not be placed", ((upperParenthesis == -1)? "right" : "left"), operatorList[j], str[operatorList[j]]);
                free(str);
                return NULL;
            }

            char *newStr = wrapInParenthesis(str, lowerParenthesis, upperParenthesis, error);
            
            // Debug
            printf("Operator in %d to %s -> added par at (%d,%d) -> %s\n", operatorList[j], str, lowerParenthesis, upperParenthesis, newStr);

            
            updatePrecedenceList(operatorList, lowerParenthesis, upperParenthesis);
            
            free(str);
            str = newStr;

            if (error->raised) {
                return NULL;
            }
        }
    }
    return str;
}

void solveInfixBinTree(operationNode* node, calculatorErr* error) {
    if (!node || error->raised) return;

    // Leaf node, return
    if (!node->operandExpressionA && !node->operandExpressionB) {
        node->result = node->operation(node->atomicA, node->atomicB, error);
        return;
    }

    // Solve left son
    if (node->operandExpressionA) {
        solveInfixBinTree(node->operandExpressionA, error);
        if (error->raised) return;
        node->atomicA = node->operandExpressionA->result;
    }

    // Solve right son
    if (node->operandExpressionB) {
        solveInfixBinTree(node->operandExpressionB, error);
        if (error->raised) return;
        node->atomicB = node->operandExpressionB->result;
    }

    // Solve this node
    node->result = node->operation(node->atomicA, node->atomicB, error);
}


float infixCalculator(char* str, calculatorErr* error){
    // Remove blanks
    char* deBlanked = removeCharFromString(str, ' ');
    if(!deBlanked){
        setError(error, "infixCalculator: error, could not remove blanks from the string");
        return -1;
    }

    // Generate operator precedence list
    int* opList = generateOperatorPrecedenceList(deBlanked);
    if(!opList){
        setError(error, "infixCalculator: error, could not generate operator precedence list");
        free(deBlanked);
        return -1;
    }

    // Resolve operator precedence by adding parenthesis
    char* formatted = evaluateOperatorPrecedence(deBlanked, opList, error);
    if(error->raised){
        free(deBlanked);
        free(opList);
        return -1;
    }

    // Build binary tree of the operation
    operationNode *treeRoot;
    buildBinTree(formatted, &treeRoot, error);
    if(error->raised){
        free(deBlanked);
        free(opList);
        free(formatted);
        return -1;
    }

    // Debug
    printTree(treeRoot, 0);
    
    solveInfixBinTree(treeRoot, error);
    
    // Debug
    printf("\nSolved Tree:\n");
    printTree(treeRoot, 0);
    
    float result = treeRoot->result;

    if(error->raised){
        freeTree(treeRoot);
        free(deBlanked);
        free(opList);
        free(formatted);
        return -1;
    }

    // Free
    freeTree(treeRoot);
    free(deBlanked);
    free(opList);
    free(formatted);

    return result;
}

float reversePolishCalculator(char* str, calculatorErr* error){
    initStack(error);
    if(error->raised){
        return -1;
    }

    float res = solveRevPolString(str, error);
    if(error->raised){
        return -1;
    }

    resetStack(error);
    return res;
}