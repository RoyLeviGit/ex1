#include "utilities.h"
#include <stdio.h>

#define NUMBER_BASE 10
#define ZERO_ASCII '0'
#define NINE_ASCII '9'
#define CONVERT_ERROR -1

static lengthOfNum(int num) {
    int sum =0;
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
    char* str =(char*)malloc(length*sizeof(char));
    if (str == NULL) {
        return NULL;  
    }
    for (int i=length-1; i >= 0; i--, num /= NUMBER_BASE) {
        str[i] = (num%NUMBER_BASE)+ZERO_ASCII;
    }
    return str;
}