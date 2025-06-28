#include<stdio.h>
#include<stdlib.h>

#include"calculator.h"
#include"privateCalculator.h"
#include"myNewStrings.h"

/*
 No se pueden tener varios main en la misma unidad de compilación, como en los test
 hago gcc src/'*.c cubro este main() con el flag RELEASE para que solo se compile cuando
 hay un release de verdad y no un test
*/
 
#ifdef NOTUSING
int main(void){
    calculatorErr* error = newError();
    char* input;
    char* formattedInput;
    float res;
    initStack(error);

    while(true){
        printf("Imput an operation:\n");
        input = readString();
        res = revPolishCalc(input, error);
        formattedInput = formatOperation(input, error);

        if(error->raised){
            printf("Exit: (%s)\n", error->msg);
            return 1;
        }

        printf("%s = %f\n", formattedInput, res);
        resetStack(error);
        free(input);
        free(formattedInput);
    }

    return 0;
}
#endif

#ifdef RELEASE
int main(void){
    calculatorErr* error = newError();
    char* input = "(2+(5-4))*(3+(6+(8-(2*1))))";
    printf("%s\n", input);
    operationNode* treeRoot;
    buildBinTree(input, &treeRoot, error);
    printTree(treeRoot, 0);

    if(error->raised){
        printf("%s\n", error->msg);
    }

    return 0;
}
#endif

