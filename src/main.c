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
  calculatorErr *error = newError();
  char *input = "45 / (63*93 -5)";
  
  infixCalculator(input, error);

  if(error->raised){
    printf("%s\n", error->msg);
  }

  freeError(error);
  // free(input);
  return 0;
}
#endif

/*
45 / (65*93 -5)
Given: 45/(65*93-5)
formatStringForInfix: error, one parenthesis for operator in pos 7 was not recognised
*/