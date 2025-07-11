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

 #ifndef MYNEWSTRINGS
#define MYNEWSTRINGS

#include <stdbool.h>

char *readString();
int stringSize(char *str);
bool sameString(char *str1, char *str2);
int stoi(char *str);
void toUpper(char *str);
void stringCopy(char *str1, char *str2);
int subStringCopy(char *str1, char *str2, int start, int end);
float stof(char *str);
char *stringSplicer(char *str, int lowerSplitPos, int upperSplitPos);
char *removeCharFromString(char *str, char c);
bool isNullTerminated(char *str);
char *insertCharInString(char *str, char c, int pos);
char *stringClone(char *str);

#endif
