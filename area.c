#include "area.h"
#include <stdlib.h>
#include <string.h>

Area areaCreate(int id, const char* name){
    Area area= malloc(sizeof(*area));
    if (area == NULL) {
        return NULL;
    }
    area->id = id;
    area->name = malloc(sizeof(*area->name)*strlen(name));
    if (area->name == NULL) {
        free(area);
        return NULL;
    }
    strcpy(area->name, name);
    return area;
}

void areaDestroy(Area area) {
    if (area == NULL) {
        return;
    }
    free(area->name);
    free(area);
}

bool isValidAreaID(int id) {
    return id >= 0;
}

bool isValidAreaName(const char* name) {
    for (; *name; name++) {
        if (!isLetterOrSpace(*name)) {
            return false;
        }
    }
    return true;
}

Area stringToArea(char* key, char* value) {
    assert(key != NULL);
    int id = stringToInt(key);
    return areaCreate(id, value);
}

Result areaToString(Area area, char** key, char** value) {
    assert(area != NULL);
    *key = intToString(area->id);
    *value = malloc(sizeof(*value)*strlen(area->name));
    if (*key == NULL || *value == NULL) {
        free(*key);
        free(*value);
        return OUT_OF_MEMORY;
    }
    strcpy(*value, area->name);
    return SUCCESS;
}
