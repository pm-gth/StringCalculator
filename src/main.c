#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"
#include "myNewStrings.h"

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

// Añadir func que detecte anidamiento incorrecto, si no hay operandos para todo operador, chars raros, etc

/*
CAUSA: los pars del 89 (cualquier atomico solo en unos ())
TIPO: segfault

Introduzca operacion: 76 / 23 * 6 * (89)^2
Operator in 12 to 76/23*6*(89)^2 -> added par at (8,14) -> 76/23*6*((89)^2)
Operator in 2 to 76/23*6*((89)^2) -> added par at (0,5) -> (76/23)*6*((89)^2)
Operator in 7 to (76/23)*6*((89)^2) -> added par at (0,9) -> ((76/23)*6)*((89)^2)
Operator in 11 to ((76/23)*6)*((89)^2) -> added par at (0,20) -> (((76/23)*6)*((89)^2))
(76/23)*6
76/23
(89)^2
89
AddressSanitizer:DEADLYSIGNAL
=================================================================
==9457==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x56e52e8a92db bp 0x7ffce78d2f20 sp 0x7ffce78d2eb0 T0)
==9457==The signal is caused by a WRITE memory access.
==9457==Hint: address points to the zero page.
    #0 0x56e52e8a92db in buildBinTree src/calculator/calculator.c:413
    #1 0x56e52e8a9467 in buildBinTree src/calculator/calculator.c:430
    #2 0x56e52e8a9560 in buildBinTree src/calculator/calculator.c:450
    #3 0x56e52e8ab187 in infixCalculator src/calculator/calculator.c:788
    #4 0x56e52e8a7417 in main src/main.c:21
    #5 0x7e3d42e2a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #6 0x7e3d42e2a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #7 0x56e52e8a7304 in _start (/home/noroot/programacion/c/programas/stringCalculator/build/strCalc+0x2304) (BuildId: c51a1b9301a26b90c5434ebe1019804c6e05ca28)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV src/calculator/calculator.c:413 in buildBinTree
==9457==ABORTING

*/