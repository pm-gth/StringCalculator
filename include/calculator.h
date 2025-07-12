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
 
#ifndef CALCULATOR
#define CALCULATOR

#include <stdbool.h>

// Error handling
typedef struct {
  bool raised;
  char *msg;
} calculatorErr;

calculatorErr *newError();
void clearError(calculatorErr *error);
void setError(calculatorErr *error, const char *fmt, ...);
void freeError(calculatorErr *error);

// Reverse-Polish calculator
float reversePolishCalculator(char* str, calculatorErr* error);
char *fancyRevPolOperation(char *str, calculatorErr *error);

// Infix calculator
float infixCalculator(char* str, calculatorErr* error);

#endif
