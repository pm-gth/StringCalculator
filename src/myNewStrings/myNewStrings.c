#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include"myNewStrings.h"

//Creates String from stdin
char* readString(){
    int size = 64; //default char capacity
    int index = 0;
    char c;

    char* buffer = malloc(sizeof(char)*(size));

    if(buffer == NULL){
        fprintf(stderr, "Input: error, could not allocate buffer\n");
        return NULL;
    }

    while((c = getchar()) != EOF && c != '\n'){
        buffer[index++] = c;

        if(index >= size){
            size *= 2;
            buffer = realloc(buffer, sizeof(char)*size);

            if(buffer == NULL){
                fprintf(stderr, "Input: error, could not extend buffer\n");
                return NULL;
            }
        }
    }

    buffer[index] = '\0';

    //DEBUG
    //printf("Input: written up to index %d, buffer size is %d\n", index, size);

    return buffer;
}

//Gets the size of a string (not including \0)
int stringSize(char* str){
    int i;
    for(i = 0; str[i] != '\0'; i++);
    return i;
}

//Compares two strings
bool sameString(char* str1, char* str2){
    bool equal = true;

    int i;
    for(i = 0; str1[i] != '\0'; i++){
        if(str1[i] != str2[i]){
            equal = false;
            break;
        }
    }

    if(equal && str2[i] != '\0'){ //Check if str2 has same length as str1
        equal = false;
    }

    return equal;
}

//String to integer
int stoi(char* str) {
    int accumulator = 0;
    int i = 0;

    while (str[i] != '\0') {
        accumulator = accumulator * 10 + (str[i] - '0');
        i++;
    }

    return accumulator;
}

//String to float
float stof(char* str) {
    float integerPart = 0;
    float decimalPart = 0;
    float divisor = 10;
    bool arrivedToDecimal = false;
    int i = 0;

    while (str[i] != '\0') {
        if(str[i] == '.'){
            arrivedToDecimal = true;
        } else if(!arrivedToDecimal){
            integerPart = integerPart * 10 + (str[i] - '0');
        } else if(arrivedToDecimal){
            decimalPart = decimalPart + (str[i] - '0')/divisor;
            divisor *= 10;
        }
        i++;
    }

    return integerPart+decimalPart;
}

//Capitalizes the string
void toUpper(char* str){
    int i = 0;
    for(; str[i] != '\0'; i++);

    for (int j = 0; j < i; j++){
        if (str[j] >= 'a' && str[j] <= 'z'){
            str[j] -= ('a' - 'A');
        }
    }
}

//Copies string
int stringCopy(char* str1, char* str2){
    int j = 0;
    for(int i = 0; str1[i] != '\0'; i++){
        str2[j] = str1[i];
        j++;
    }
    str2[j] = '\0';

    return 0;
}
