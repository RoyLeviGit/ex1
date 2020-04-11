#include "map.h"
#include <string.h>
#include <assert.h>

#define NO_SIZE -1

struct Node {
    char* key;
    char* value;
    struct Node* next;
};

typedef struct Node* Node;

struct Map_t {
   Node head; 
};

Map mapCreate() {
    Map result = malloc(sizeof(*result));
    if (result == NULL) {
        return NULL;
    }
    result->head == NULL;
    return result;
}

void mapDestroy(Map map) {
    if (map==NULL) {
        return;
    }
    Node current_origin = map->head;
    while (current_origin!=NULL){
        free(current_origin->key);
        free(current_origin->value);
        Node tmp = current_origin->next;
        free(current_origin);
        current_origin = tmp;
    }
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

    copy->key = malloc(sizeof(*copy->key)*strlen(node->key));
    copy->value = malloc(sizeof(*copy->value)*strlen(node->value));
    if (copy->key == NULL || copy->value == NULL) {
        free(copy->key);
        free(copy->value);
        return NULL;
    }

    strcpy(copy->key,node->key);
    strcpy(copy->value,node->value);
    copy->next = NULL;
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
    return copy;
}

int mapGetSize(Map map) {
    if (map == NULL) {
        return NO_SIZE;
    }
    int counter = 0;
    for (Node current = map->head; current !=NULL; current = current->next, ++counter);
    return counter;
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
        value = realloc(sizeof(*value)*strlen(data));
        if (value == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        strcpy(value, data);
    } else {
        Node new_node = malloc(sizeof(*new_node));
        if (new_node == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        new_node->key = malloc(sizeof(*new_node->key)*strlen(key));
        new_node->value = malloc(sizeof(*new_node->value)*strlen(data));
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
   }
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
    Node previous, current = map->head;

    // take care if head case
    if (current !=NULL && strcmp(current->key, key)) {
        free(current->key);
        free(current->value);
        map->head = current->next;
        free(current);
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
    return MAP_SUCCESS;
}

char* mapGetFirst(Map map) {
    if (map == NULL || map->head != NULL) {
        return NULL;
    }

}