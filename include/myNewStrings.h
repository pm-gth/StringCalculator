#ifndef MYNEWSTRINGS
#define MYNEWSTRINGS

#include<stdbool.h>

char* readString();
int stringSize(char* str);
bool sameString(char* str1, char* str2);
int stoi(char* str);
void toUpper(char* str);
int stringCopy(char* str1, char* str2);
float stof(char* str);
char* stringSplicer(char* str, int lowerSplitPos, int upperSplitPos);

#endif
