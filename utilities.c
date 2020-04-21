#include "utilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define NUMBER_BASE 10
#define ZERO_ASCII '0'
#define NINE_ASCII '9'
#define TOKEN ":"
#define TOKEN_LENGTH 1
#define SMALL_A 'a'
#define SMALL_Z 'z'
#define SPACE ' '

static int lengthOfNum(int num) {
    if (num == 0) {
        return 1;
    }
    int sum = 0;
    while(num != 0)
    {
        num /= NUMBER_BASE;
        sum++;
    }
    return sum;
}

int stringToInt(char* str) {
    assert(str != NULL);
    int id = 0;
    int index = 0;
    while (str[index]) {
        if (str[index] >= ZERO_ASCII && str[index] <= NINE_ASCII) {
            id *= NUMBER_BASE;
            id += str[index]-ZERO_ASCII;
        } else {
            return CONVERT_ERROR;
        }
        index++;
    }
    return id;
}

char* intToString(int num) {
    int length = lengthOfNum(num);
    char* str =(char*)malloc((length+1)*sizeof(*str));
    if (str == NULL) {
        return NULL;  
    }
    str[length] = 0;
    for (int i=length-1; i >= 0; i--, num /= NUMBER_BASE) {
        str[i] = (num%NUMBER_BASE)+ZERO_ASCII;
    }
    return str;
}

Result stringToTwoNumbers(const char* str, int* num1, int* num2) {
    assert(str != NULL);
    assert(num1 != NULL);
    assert(num2 != NULL);

    // create copy of str for token to destroy :)
    char* copy = malloc(sizeof(*copy)*(strlen(str)+1));
    if (copy == NULL) {
        return OUT_OF_MEMORY;
    }
    strcpy(copy, str);

    char* token = strtok(copy, TOKEN);
    *num1 = stringToInt(token);
    token = strtok(NULL, TOKEN);
    *num2 = stringToInt(token);
    free(copy);
    return SUCCESS;
}

Result twoNumbersToString(int num1, int num2, char** str) {
    char* num1_string = intToString(num1);
    char* num2_string = intToString(num2);
    *str = malloc(sizeof(**str)*(strlen(num1_string)+strlen(num2_string)+TOKEN_LENGTH+1));
    if (num1_string == NULL || num2_string == NULL || *str == NULL) {
        free(num1_string);
        free(num2_string);
        free(*str);
        return OUT_OF_MEMORY;
    }

    sprintf(*str, "%s%s%s", num1_string, TOKEN, num2_string);
    return SUCCESS;
}

bool isLetterOrSpace(char character){
    if((character < SMALL_A || character > SMALL_Z) && character != SPACE){
        return false ;
    }
    return true;
}
