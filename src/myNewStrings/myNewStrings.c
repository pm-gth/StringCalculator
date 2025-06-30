#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "myNewStrings.h"

// Creates String from stdin
char *readString() {
  int size = 64; // default char capacity
  int index = 0;
  char c;

  char *buffer = malloc(sizeof(char) * (size));

  if (buffer == NULL) {
    fprintf(stderr, "Input: error, could not allocate buffer\n");
    return NULL;
  }

  while ((c = getchar()) != EOF && c != '\n') {
    buffer[index++] = c;

    if (index >= size) {
      size *= 2;
      buffer = realloc(buffer, sizeof(char) * size);

      if (buffer == NULL) {
        fprintf(stderr, "Input: error, could not extend buffer\n");
        return NULL;
      }
    }
  }

  buffer[index] = '\0';

  // DEBUG
  // printf("Input: written up to index %d, buffer size is %d\n", index, size);

  return buffer;
}

// Gets the size of a string (not including \0)
int stringSize(char *str) {
  int i;
  for (i = 0; str[i] != '\0'; i++)
    ;
  return i;
}

// Compares two strings
bool sameString(char *str1, char *str2) {
  bool equal = true;

  int i;
  for (i = 0; str1[i] != '\0'; i++) {
    if (str1[i] != str2[i]) {
      equal = false;
      break;
    }
  }

  if (equal && str2[i] != '\0') { // Check if str2 has same length as str1
    equal = false;
  }

  return equal;
}

// String to integer
int stoi(char *str) {
  int accumulator = 0;
  int i = 0;

  while (str[i] != '\0') {
    accumulator = accumulator * 10 + (str[i] - '0');
    i++;
  }

  return accumulator;
}

// String to float
float stof(char *str) {
  float integerPart = 0;
  float decimalPart = 0;
  float divisor = 10;
  bool arrivedToDecimal = false;
  int i = 0;

  while (str[i] != '\0') {
    if (str[i] == '.') {
      arrivedToDecimal = true;
    } else if (!arrivedToDecimal) {
      integerPart = integerPart * 10 + (str[i] - '0');
    } else if (arrivedToDecimal) {
      decimalPart = decimalPart + (str[i] - '0') / divisor;
      divisor *= 10;
    }
    i++;
  }

  return integerPart + decimalPart;
}

// Capitalizes the string
void toUpper(char *str) {
  int i = 0;
  for (; str[i] != '\0'; i++)
    ;

  for (int j = 0; j < i; j++) {
    if (str[j] >= 'a' && str[j] <= 'z') {
      str[j] -= ('a' - 'A');
    }
  }
}

// Copies string
int stringCopy(char *str1, char *str2) {
  int j = 0;
  for (int i = 0; str1[i] != '\0'; i++) {
    str2[j] = str1[i];
    j++;
  }
  str2[j] = '\0';

  return 0;
}

// Splits string (both upper and lower positions are included)
char *stringSplicer(char *str, int lowerSplitPos, int upperSplitPos) {
  if (lowerSplitPos > upperSplitPos) {
    return NULL;
  }

  int size = stringSize(str);
  if (lowerSplitPos >= size || upperSplitPos >= size) {
    return NULL;
  }

  // First pass to get subtring's size
  int noOfChars = 0;
  for (int i = lowerSplitPos; i <= upperSplitPos && str[i] != '\0'; i++) {
    noOfChars++;
  }

  char *subString = malloc(sizeof(char) * (noOfChars + 1)); // Plus one for '\0'
  if (!subString) {
    return NULL;
  }

  int subStringIndex = 0;
  for (int i = lowerSplitPos; i <= upperSplitPos && str[i] != '\0'; i++) {
    subString[subStringIndex] = str[i];
    subStringIndex++;
  }
  subString[subStringIndex] = '\0';

  return subString;
}

bool isNullTerminated(char *str) {
  int threshold = 9999;

  for (int i = 0; i < threshold; i++) {
    if (str[i] == '\0') {
      return true;
    }
  }
  return false;
}

char *removeCharFromString(char *str, char c) {
  int deletions = 0;

  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == c) {
      deletions++;
    }
  }

  int newSize = stringSize(str) - deletions + 1; // +1 for '\0'
  char *newStr = malloc(sizeof(char) * newSize);

  if (!newStr) {
    return NULL;
  }

  int newStrIndex = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != c) {
      newStr[newStrIndex] = str[i];
      newStrIndex++;
    }
  }

  newStr[newStrIndex] = '\0';

  return newStr;
}

// Puts char c inside the string, the current char at position pos will be
// displaced to the right (pos+1)
char *insertCharInString(char *str, char c, int pos) {
  int old_len = stringSize(str);
  if (pos < 0 || pos > old_len) {
    return NULL;
  }

  char *newStr = malloc(old_len + 2); // 1 for new char 1 for \0
  if (!newStr)
    return NULL;

  int newStrIndex = 0;

  for (int i = 0; i <= old_len; i++) {
    if (i == pos) {
      newStr[newStrIndex++] = c;
    }
    newStr[newStrIndex++] = str[i];
  }

  return newStr;
}
