#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"
#include "myNewStrings.h"
#include "privateCalculator.h"

/*
 No se pueden tener varios main en la misma unidad de compilación, como en los
 test hago gcc src/'*.c cubro este main() con el flag RELEASE para que solo se
 compile cuando hay un release de verdad y no un test
*/

#ifdef RELEASE
int main(void) {
  while(true){
    printf("Introduzca operacion: ");
    calculatorErr *error = newError();
    char *input = readString();
  
    float res = infixCalculator(input, error);
    printf("Sol: %f\n", res);

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