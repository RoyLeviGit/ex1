#ifndef UTILITIES_H
#define UTILITIES_H

typedef enum Result {
    OUT_OF_MEMORY,
    SUCCESS
} Result;

int stringToInt(char* str);

char* intToString(int number);

#endif // "UTILITIES_H"