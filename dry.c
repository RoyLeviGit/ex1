#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;

typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    EMPTY_LIST,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node head){
    int count = 0;
    for (; head != NULL; head = head->next) {
        count++;
    }
    return count;
}

bool isListSorted(Node head){
    while (head != NULL) {
        if (head->next != NULL) {
            if (head->x > head->next->x) {
                return false;
            }
        }
        head = head->next;
    }
    return true;
}

//realase the memory allocated for the list
void destroyList(Node head) {
    while(getListLength(head) > 1) {
        Node current = head->next;
        if (current != NULL) {
            head->next = current->next;
            free(current);
        }
    }
    free(head);
}

// allocates memory for given size of list, and returns head of list
Node listAllocateMemory(int size){
    if (size <= 0) {
        return NULL;
    }

    Node head = malloc(sizeof(*head));
    if (head == NULL) {
        return NULL;
    }
    head->next = NULL;

    Node current = head;
    for (int i = 1; i < size; i++, current = current->next) {
        Node new_node = malloc(sizeof(*new_node));
        if(new_node == NULL) {
            destroyList(head);
            return NULL;
        }
        new_node->next = NULL;
        current->next = new_node;
    }
    return head;
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out){
    if(list1 == NULL || list2 == NULL) {
        return EMPTY_LIST;
    }
    if (!isListSorted(list1) || !isListSorted(list2)) {
        return UNSORTED_LIST;
    }

    *merged_out = NULL;
    int list_length = getListLength(list1) + getListLength(list2);

    Node head = listAllocateMemory(list_length);
    if (head == NULL) {
        return MEMORY_ERROR;
    }
    
    Node current = head;
    for (; list1 != NULL && list2 != NULL; current = current->next) {
        if(list1->x < list2->x) {
            current->x = list1->x;
            list1 = list1->next;
        }
        else {
            current->x = list2->x;
            list2 = list2->next;
        }
    }

    // add what's left of the lists to the end
    for(; list1 != NULL; current = current->next, list1 = list1->next) {
        current->x = list1->x;
    }
    for(; list2 != NULL; current = current->next, list2 = list2->next) {
        current->x = list2->x;
    }
    
    *merged_out = head;
    return SUCCESS;
}

char *stringDuplicate(char *str, int times) {
    assert(str && times > 0);
    int len = strlen(str);
    char *out = malloc((len * times) + 1);
    if (out == NULL) {
        return NULL;
    }
    for (int i = 0; i < times; i++) {
        strcpy(out + (i * len), str);
    }
    return out;
}

void printList(Node list){
    while (list != NULL)
    {
        if (list->next == NULL)
            printf("%d", list->x);
        else
            printf("%d->", list->x);
        list = list->next;
    }
    printf("\n");
}

int main()
{
    Node list1 = (Node)malloc(sizeof(*list1));
    list1->x = 1;
    list1->next = (Node)malloc(sizeof(*list1));
    list1->next->x = 4;
    list1->next->next = (Node)malloc(sizeof(*list1));
    list1->next->next->x = 9;
    list1->next->next->next = NULL;
    
    Node list2 = (Node)malloc(sizeof(*list2));
    list2->x = 2;
    list2->next = (Node)malloc(sizeof(*list2));
    list2->next->x = 4;
    list2->next->next = (Node)malloc(sizeof(*list2));
    list2->next->next->x = 8;
    list2->next->next->next = NULL;
    
    printList(list1);
    printList(list2);

    Node merged_out;
    mergeSortedLists(list1, list2, &merged_out);
    if (merged_out != NULL)
        printList(merged_out);
    destroyList(merged_out);
    destroyList(list1);
    destroyList(list2);

    char* name = "nadav_gonev_refrensim";
    char* new_name = stringDuplicate(name, 5);
    printf("%s\n", new_name);
    free(new_name);
    return 0;
}
