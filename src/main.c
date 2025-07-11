/*
 * Copyright (C) 2025 pablo.marq04
 *
 * This file is part of StringCalculator.
 *
 * StringCalculator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StringCalculator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with StringCalculator. If not, see <https://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"
#include "myNewStrings.h"

#ifdef RELEASE
int main(void) {
  calculatorErr *error = newError();
  
  while(true){
    printf("Input Operation or 'q'\n");
    char *input = readString();
    
    if(sameString(input, "q")){
      freeError(error);
      free(input);
      break;
    }
  
    float res = infixCalculator(input, error);

    if(error->raised){
      printf("%s\n", error->msg);
      freeError(error);
      free(input);
      return -1;
    } else{
      printf("%f\n", res);
    }
    free(input);
  }
}
#endif