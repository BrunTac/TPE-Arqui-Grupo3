#ifndef CIRCULAR_LIST_STATIC_H
#define CIRCULAR_LIST_STATIC_H

#include <stddef.h>

#define MAX_NODES 100

typedef struct StaticNode {
    void * data;
    int next;
    int used;
} StaticNode;

typedef struct {
    StaticNode nodes[MAX_NODES];
    int current;
    int size;
} List;

void initList(List * list);

int addNode(List * list, void * data);

void removeCurrentNode(List * list);

void nextNode(List * list);

void * getCurrentData(List * list);

int isEmpty(List * list);

#endif
