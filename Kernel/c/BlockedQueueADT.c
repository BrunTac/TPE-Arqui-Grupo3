#include "BlockedQueueADT.h"
#include "newmm.h"
#include "processManager.h"

typedef struct Node {
    uint64_t pid;
    struct Node * next;
} Node;

struct BlockedQueue {
    Node * head;
    Node * tail;
};

BlockedQueueADT newQueue(void) {
    BlockedQueueADT queue = malloc_mm(getHeap(), sizeof(struct BlockedQueue));
    if (queue == NULL) return NULL;
    queue->head = queue->tail = NULL;
    return queue;
}

void enqueue(BlockedQueueADT queue, uint64_t pid) {
    if (!queue) return;

    Node* newNode = malloc_mm(getHeap(), sizeof(Node));
    if (!newNode) return;

    newNode->pid = pid;
    newNode->next = NULL;

    if (queue->tail) {
        queue->tail->next = newNode;
    } else {
        queue->head = newNode;
    }

    queue->tail = newNode;
}

void dequeue(BlockedQueueADT queue) {
    if (!queue || !queue->head) return ;

    Node * temp = queue->head;
    uint64_t pid = temp->pid;
    queue->head = temp->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    unblockProcess(pid);
    free_mm(getHeap(), temp);
}

void freeQueue(BlockedQueueADT queue) {
    if (!queue) return;

    Node* current = queue->head;
    while (current) {
        Node* next = current->next;
        free_mm(getHeap(), current);
        current = next;
    }

    free_mm(getHeap(), queue);
}

uint8_t isQueued(BlockedQueueADT queue, uint64_t pid){
    if (!queue){
        return 0;
    }

    Node * current = queue->head;
    while (current) {
        if (current->pid == pid) {
            return 1;
        }
        current = current->next;
    }

    return 0; 
}

void emptyQueue(BlockedQueueADT queue){
    if(queue != NULL){
        Node * current = queue->head;
        while(current != NULL){
            unblockProcess(current->pid);
            current = current->next;
        }
    }
}

