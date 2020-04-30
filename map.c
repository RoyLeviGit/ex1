#include "map.h"
#include "node.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define NO_SIZE -1

struct Map_t {
   Node head;
   Node iterator;
   int size;
};

Map mapCreate() {
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
        return NULL;
    }
    map->head = NULL;
    map->iterator = NULL;
    map->size = 0;
    return map;
}

void mapDestroy(Map map) {
    if (map==NULL) {
        return;
    }
    nodeListDestroy(map->head);
    free(map);
}

Map mapCopy(Map map) {
    Map copy = mapCreate();
    if (map == NULL || copy == NULL) {
        mapDestroy(copy);
        return NULL;
    }
    if (map->head == NULL) {
        return copy;
    }

    Node current_origin = map->head;
    // take care of head
    copy->head = nodeCopyData(current_origin);
    if (copy->head == NULL) {
        mapDestroy(copy);
        return NULL;
    }
    Node current_copy = copy->head;
    while (nodeGetNext(current_origin) != NULL) {
        nodeSetNext(current_copy, nodeCopyData(nodeGetNext(current_origin)));
        if (nodeGetNext(current_copy) == NULL) {
            mapDestroy(copy);
            return NULL;
        }
        current_copy = nodeGetNext(current_copy);
        current_origin = nodeGetNext(current_origin);  
    }
    copy->size = map->size;
    return copy;
}

int mapGetSize(Map map) {
    if (map == NULL) {
        return NO_SIZE;
    }
    return map->size;
}

bool mapContains (Map map, const char* key) {
    return (mapGet(map, key) != NULL);
}

// find node in map by given key
static Node getNodeByKey(Map map, const char* key) {
    assert(map != NULL);
    assert(key != NULL);

    for (Node current = map->head; current !=NULL; current = nodeGetNext(current)) {
        if (!strcmp(nodeGetKey(current), key)) {
            return current;
        }
    }
    return NULL;
}

MapResult mapPut (Map map, const char* key, const char* data) {
    if (map == NULL || key == NULL || data == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    
    Node node = getNodeByKey(map, key);
    // if map contains key
    if (node != NULL) {
        free(nodeGetValue(node));
        nodeSetValue(node, malloc(strlen(data)+1));
        if (nodeGetValue(node) == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        strcpy(nodeGetValue(node), data);
    } else {
        Node new_node = nodeCreate(key, data, map->head);
        if (new_node == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        map->head = new_node;
        map->size++;
   }
   return MAP_SUCCESS;
}

char* mapGet(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }
    Node node = getNodeByKey(map, key);
    return node == NULL ? NULL : nodeGetValue(node);
}

MapResult mapRemove(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    // reset iterator
    mapGetFirst(map);

    Node previous, current = map->head;

    // take care if head case
    if (current != NULL && !strcmp(nodeGetKey(current), key)) {
        map->head = nodeGetNext(current);
        nodeDestroy(current);
        map->size--;
        return MAP_SUCCESS;
    }

    while (current != NULL && strcmp(nodeGetKey(current), key)) {
        previous = current;
        current = nodeGetNext(current);
    }

    if (current == NULL) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    nodeSetNext(previous, nodeGetNext(current));
    nodeDestroy(current);
    map->size--;
    return MAP_SUCCESS;
}

char* mapGetFirst(Map map) {
    if (map == NULL || map->head == NULL) {
        return NULL;
    }
    assert(nodeGetKey(map->head) != NULL);
    map->iterator = map->head;
    return nodeGetKey(map->iterator);
}

char* mapGetNext(Map map){
    if (map == NULL || map->iterator == NULL || nodeGetNext(map->iterator) == NULL) {
        return NULL;
    }
    assert(nodeGetKey(nodeGetNext(map->iterator)) != NULL);
    map->iterator = nodeGetNext(map->iterator);
    return nodeGetKey(map->iterator);
}

MapResult mapClear(Map map) {
    if (map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    nodeListDestroy(map->head);
    map->head = NULL;
    map->iterator = NULL;
    map->size = 0;
    return MAP_SUCCESS;
}

void mapPrint(Map map){
    if (map==NULL) {
        printf("Map is NULL\n");
        return;
    }
    printf("Map size is: %d\n", map->size);
    Node current = map->head;
    while (current!=NULL) {
        nodePrint(current);
        current = nodeGetNext(current);
    }
    return;
}
