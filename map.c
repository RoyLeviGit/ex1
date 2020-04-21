#include "map.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define NO_SIZE -1

struct Node {
    char* key;
    char* value;
    struct Node* next;
};

typedef struct Node* Node;

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

static void nodeListDestroy(Node head) {
    while (head!=NULL){
        free(head->key);
        free(head->value);
        Node tmp = head->next;
        free(head);
        head = tmp;
    }
}

void mapDestroy(Map map) {
    if (map==NULL) {
        return;
    }
    nodeListDestroy(map->head);
    free(map);
}

static Node nodeCopyData(Node node) {
    assert(node != NULL);
    assert(node->key != NULL);
    assert(node->value != NULL);

    Node copy = malloc(sizeof(*copy));
    if (copy == NULL) {
        return NULL;
    }

    copy->key = malloc(sizeof(*copy->key)*(strlen(node->key)+1));
    copy->value = malloc(sizeof(*copy->value)*(strlen(node->value)+1));
    if (copy->key == NULL || copy->value == NULL) {
        free(copy->key);
        free(copy->value);
        return NULL;
    }

    strcpy(copy->key,node->key);
    strcpy(copy->value,node->value);
    copy->next = NULL;
    return copy;
}

Map mapCopy(Map map) {
    Map copy = mapCreate();
    if (map == NULL || map->head==NULL || copy == NULL) {
        mapDestroy(copy);
        return NULL;
    }

    Node current_origin = map->head;
    Node current_copy = copy->head;
    // take care of head
    current_copy = nodeCopyData(current_origin);
    if (current_copy == NULL) {
        mapDestroy(copy);
        return NULL;
    }
    
    while (current_origin->next != NULL) {
        current_copy->next = nodeCopyData(current_origin->next);
        if (current_copy->next == NULL) {
            mapDestroy(copy);
            return NULL;
        }
        current_copy = current_copy->next;
        current_origin = current_origin->next;  
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

MapResult mapPut (Map map, const char* key, const char* data) {
    if (map == NULL || key == NULL || data == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    
    char* value = mapGet(map, key);
    // if map contains key
    if (value != NULL) {
        value = realloc(value, sizeof(*value)*strlen(data));
        if (value == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        strcpy(value, data);
    } else {
        Node new_node = malloc(sizeof(*new_node));
        if (new_node == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        new_node->key = malloc(sizeof(*new_node->key)*(strlen(key)+1));
        new_node->value = malloc(sizeof(*new_node->value)*(strlen(data)+1));
        if (new_node->key == NULL || new_node->value == NULL) {
            free(new_node->key);
            free(new_node->value);
            free(new_node);
            return MAP_OUT_OF_MEMORY;
        }
        strcpy(new_node->key, key);
        strcpy(new_node->value, data);

        new_node->next = map->head;
        map->head = new_node;
        map->size++;
   }
   return MAP_SUCCESS;
}

char* mapGet(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }
    
    for (Node current = map->head; current !=NULL; current = current->next) {
        if (!strcmp(current->key, key)) {
            return current->value;
        }
    }
    return NULL;
}

MapResult mapRemove(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    // reset iterator
    mapGetFirst(map);

    Node previous, current = map->head;

    // take care if head case
    if (current !=NULL && !strcmp(current->key, key)) {
        free(current->key);
        free(current->value);
        map->head = current->next;
        free(current);
        map->size--;
        return MAP_SUCCESS;
    }

    while (current !=NULL && strcmp(current->key, key)) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    previous->next = current->next;
    free(current->key);
    free(current->value);
    free(current);
    map->size--;
    return MAP_SUCCESS;
}

char* mapGetFirst(Map map) {
    if (map == NULL || map->head == NULL) {
        return NULL;
    }
    assert(map->head->key != NULL);
    map->iterator = map->head;
    return map->iterator->key;
}

char* mapGetNext(Map map){
    if (map==NULL || map->iterator==NULL || map->iterator->next==NULL) {
        return NULL;
    }
    assert(map->iterator->next->key!=NULL);
    map->iterator = map->iterator->next;
    return map->iterator->key;
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

static void nodePrint(Node node) {
    assert(node != NULL);
    assert(node->key != NULL);
    assert(node->value != NULL);
    printf("\"%s\" : \"%s\"\n", node->key, node->value);
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
        current = current->next;
    }
    return;
}
