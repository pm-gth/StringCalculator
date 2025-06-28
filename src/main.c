#include<stdio.h>
#include<stdlib.h>

#include"calculator.h"
#include"myNewStrings.h"

/*
 No se pueden tener varios main en la misma unidad de compilación, como en los test
 hago gcc src/'*.c cubro este main() con el flag RELEASE para que solo se compile cuando
 hay un release de verdad y no un test
 */
 
#ifdef RELEASE
int main(void){
    calculatorErr* error = newError();
    char* input;
    char* formattedInput;
    float res;
    initStack(error);

    while(true){
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
