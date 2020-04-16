#ifndef TRIBE_H
#define TRIBE_H

#include "utilities.h"

typedef struct tribe {
    int id;
    char* name;
} *Tribe;

// return - NULL is out of memory
Tribe tribeCreate(int tribe);

bool isValidTribeID(int tribe_id);

bool isValidTribeName(char* name);

Tribe stringToTribe(char* key, char* value);

Result tribeToString(Tribe tribe, char* key, char* value);

#endif //TRIBE_H