#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <assert.h>
#include <stdbool.h>

#define CONVERT_ERROR -1

typedef enum Result {
    OUT_OF_MEMORY,
    SUCCESS
} Result;

int stringToInt(char* str);

char* intToString(int number);

void stringToTwoNumbers(char* str, int* num1, int* num2);

Result twoNumbersToString(int num1, int num2, char** str);

bool isLetterOrSpace(char character);

#endif // "UTILITIES_H_"
