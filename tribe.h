#ifndef TRIBE_H
#define TRIBE_H

#include "utilities.h"

#define MAX_TRIBE_KEY_LENGTH 20
typedef struct tribe {
    int id;
    char* name;
} *Tribe;

// return - NULL is out of memory
Tribe tribeCreate(int id, char* name);

bool isValidTribeID(int id);

bool isValidTribeName(char* name);

Tribe stringToTribe(char* key, char* value);

Result tribeToString(Tribe tribe, char* key, char* value);

#endif //TRIBE_H