#ifndef TRIBE_H_
#define TRIBE_H_

#include "utilities.h"

typedef struct tribe *Tribe;

Tribe tribeCreate(int id, const char* name);

void tribeDestroy(Tribe tribe);

int getTribeID(Tribe tribe);

char* getTribeName(Tribe tribe);

bool isValidTribeID(int id);

bool isValidTribeName(const char* name);

Tribe stringToTribe(char* key, char* value);

Result tribeToString(Tribe tribe, char** key, char** value);

#endif //TRIBE_H_
