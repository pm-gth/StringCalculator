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
