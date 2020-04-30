#include "node.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct node {
    char* key;
    char* value;
    struct node* next;
};

Node nodeCreate(const char* key, const char* value, Node next) {
    Node new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->key = malloc(strlen(key)+1);
    new_node->value = malloc(strlen(value)+1);
    if (new_node->key == NULL || new_node->value == NULL) {
        nodeDestroy(new_node);
        return NULL;
    }
    strcpy(new_node->key, key);
    strcpy(new_node->value, value);

    new_node->next = next;
    return new_node;
}

void nodeDestroy(Node node) {
    if (node == NULL) {
        return;
    }
    free(node->key);
    free(node->value);
    free(node);
}

void nodeListDestroy(Node head) {
    while (head != NULL){
        free(head->key);
        free(head->value);
        Node tmp = head->next;
        free(head);
        head = tmp;
    }
}

char* nodeGetKey(Node node) {
    assert(node != NULL);
    return node->key;
}

void nodeSetKey(Node node, char* key) {
    assert(node != NULL);
    node->key = key;
}

char* nodeGetValue(Node node) {
    assert(node != NULL);
    return node->value;
}

void nodeSetValue(Node node, char* value) {
    assert(node != NULL);
    node->value = value;
}

Node nodeGetNext(Node node) {
    assert(node != NULL);
    return node->next;
}

void nodeSetNext(Node node, Node next) {
    assert(node != NULL);
    node->next = next;
}

Node nodeCopyData(Node node) {
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
        free(copy);
        return NULL;
    }

    strcpy(copy->key,node->key);
    strcpy(copy->value,node->value);
    copy->next = NULL;
    return copy;
}

void nodePrint(Node node) {
    assert(node != NULL);
    assert(node->key != NULL);
    assert(node->value != NULL);
    printf("\"%s\" : \"%s\"\n", node->key, node->value);
}
