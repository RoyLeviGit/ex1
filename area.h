#ifndef AREA_H
#define AREA_H

#include "utilities.h"

#define MAX_AREA_KEY_LENGTH 20

typedef struct area {
    int id;
    char* name;
} *Area;

void areaDestroy(Area area);

bool isValidAreaID(int id);

bool isValidAreaName(char* name);

Area stringToArea(char* key, char* value);

Result areaToString(Area area, char* key, char* value);

#endif //AREA_H
