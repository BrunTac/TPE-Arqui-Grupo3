#include <linkedListStatic.h>

static Node nodePool[MAX_NODES];
static Node *freeList = NULL;

void initNodePool() {
    for (int i = 0; i < MAX_NODES - 1; i++)
        nodePool[i].next = &nodePool[i + 1];
    nodePool[MAX_NODES - 1].next = NULL;
    freeList = &nodePool[0];
}

static Node *allocateNode() {
    if (freeList == NULL)
        return NULL;
    Node *node = freeList;
    freeList = freeList->next;
    return node;
}

static void freeNode(Node *node) {
    node->next = freeList;
    freeList = node;
}

void initList(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int addLast(LinkedList *list, void *data) {
    Node *newNode = allocateNode();
    if (newNode == NULL)
        return -1;

    newNode->data = data;
    newNode->next = NULL;

    if (list->tail)
        list->tail->next = newNode;
    else
        list->head = newNode;

    list->tail = newNode;
    list->size++;
    return 0;
}

void *removeFirst(LinkedList *list) {
    if (list->head == NULL)
        return NULL;

    Node *temp = list->head;
    void *data = temp->data;

    list->head = temp->next;
    if (list->head == NULL)
        list->tail = NULL;

    freeNode(temp);
    list->size--;
    return data;
}

int isEmpty(const LinkedList *list) {
    return list->head == NULL;
}