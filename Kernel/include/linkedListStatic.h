#ifndef LINKED_LIST_STATIC_H
#define LINKED_LIST_STATIC_H

#include <stddef.h>

#define MAX_NODES 128

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

void initList(LinkedList *list);
void initNodePool();

int addLast(LinkedList *list, void *data);
void *removeFirst(LinkedList *list);
int isEmpty(const LinkedList *list);

#endif
