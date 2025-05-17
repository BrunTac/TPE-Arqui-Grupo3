#include <circularListStatic.h>

void initList(List* list) {
    list->current = -1;
    list->size = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        list->nodes[i].used = 0;
        list->nodes[i].next = -1;
        list->nodes[i].data = NULL;
    }
}

static int allocateNode(List* list) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (!list->nodes[i].used) {
            list->nodes[i].used = 1;
            return i;
        }
    }
    return -1;
}

int addNode(List* list, void* data) {
    int newIndex = allocateNode(list);
    if (newIndex == -1) return 0;

    list->nodes[newIndex].data = data;

    if (list->current == -1) {
        // First node
        list->nodes[newIndex].next = newIndex;
        list->current = newIndex;
    } else {
        int next = list->nodes[list->current].next;
        list->nodes[list->current].next = newIndex;
        list->nodes[newIndex].next = next;
    }

    list->size++;
    return 1;
}

void removeCurrentNode(List* list) {
    if (list->current == -1) return;

    if (list->nodes[list->current].next == list->current) {
        list->nodes[list->current].used = 0;
        list->current = -1;
    } else {
        int prev = list->current;
        while (list->nodes[prev].next != list->current) {
            prev = list->nodes[prev].next;
        }

        int toDelete = list->current;
        list->nodes[prev].next = list->nodes[toDelete].next;
        list->current = list->nodes[toDelete].next;
        list->nodes[toDelete].used = 0;
    }

    list->size--;
    if (list->size == 0)
        list->current = -1;
}

void nextNode(List* list) {
    if (list->current != -1)
        list->current = list->nodes[list->current].next;
}

void * getCurrentData(List* list) {
    if (list->current == -1)
        return NULL;
    return list->nodes[list->current].data;
}

int isEmpty(List* list) {
    return list->size == 0;
}
