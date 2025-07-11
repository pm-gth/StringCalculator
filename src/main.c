#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"
#include "myNewStrings.h"

#ifdef RELEASE
int main(void) {
  while(true){
    printf("Input Operation: ");
    calculatorErr *error = newError();
    char *input = readString();
  
    float res = infixCalculator(input, error);
    printf("Res: %f\n", res);

    if(error->raised){
      printf("%s\n", error->msg);
      freeError(error);
      free(input);
      return -1;
    }

    freeError(error);
    free(input);
  }
}
#endif

//Fix (1+)2