#ifndef AREA_H
#define AREA_H

#include "utilities.h"

typedef struct area *Area;

Area areaCreate(int id, const char* name);

void areaDestroy(Area area);

int getAreaID(Area area);

char* getAreaName(Area area);

bool isValidAreaID(int id);

bool isValidAreaName(const char* name);

Area stringToArea(char* key, char* value);

Result areaToString(Area area, char** key, char** value);

#endif //AREA_H
