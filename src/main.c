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
  char *input = "2^3 * 3 + 2.5 / 1";
  infixCalculator(input, error);

  if(error->raised){
    printf("%s\n", error->msg);
  }

  free(error);
  return 0;
}
#endif