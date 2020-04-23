#include "tribe.h"
#include <stdlib.h>
#include <string.h>

struct tribe {
    int id;
    char* name;
};

Tribe tribeCreate(int id, const char* name){
    Tribe tribe = malloc(sizeof(*tribe));
    if (tribe == NULL) {
        return NULL;
    }
    tribe->id = id;
    tribe->name = malloc(sizeof(tribe->name)*(strlen(name)+1));
    if (tribe->name == NULL) {
        free(tribe);
        return NULL;
    }
    strcpy(tribe->name, name);
    return tribe;
}

void tribeDestroy(Tribe tribe) {
    if (tribe == NULL) {
        return;
    }
    free(tribe->name);
    free(tribe);
}
int getTribeID(Tribe tribe){
    assert(tribe != NULL);
    return tribe->id;
}
char* getTribeName(Tribe tribe){
    assert(tribe != NULL);
    return tribe->name;
}

bool isValidTribeID(int id) {
    return id >= 0;
}

bool isValidTribeName(const char* name) {
    for (; *name; name++) {
        if (!isLetterOrSpace(*name)) {
            return false;
        }
    }
    return true;
}

Tribe stringToTribe(char* key, char* value) {
    assert(key != NULL);
    assert(value != NULL);
    int id = stringToInt(key);
    return tribeCreate(id, value);
}

Result tribeToString(Tribe tribe, char** key, char** value) {
    assert(tribe != NULL);
    *key = intToString(tribe->id);
    *value = malloc(strlen(tribe->name)+1);
    if (*key == NULL || *value == NULL) {
        free(*key);
        free(*value);
        return OUT_OF_MEMORY;
    }
    strcpy(*value, tribe->name);
    return SUCCESS;
}
