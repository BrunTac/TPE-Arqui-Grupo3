#include "newmm.h"
#include <stddef.h>
#include <stdint.h>

#define SIZEOF_HEADER sizeof(header_t)

typedef char ALIGN[SIZEOF_HEADER];

typedef union header {
    typedef struct {
        union header *next;
        size_t size;
        int isFree;
    } s;
    ALIGN align; 
} header_t ;

typedef struct {
    header_t *firstBlock;
    uintptr_t memStart;
    uintptr_t memEnd;
} mm_t ;

mm_t *createMemoryManager(void * manager, void *memoryRegion, size_t regionSize) {
    if(!memoryRegion || !memoryManager || regionSize < sizeof(header_t)) {
        return NULL; // params
    }

	mm_t *memoryManager = (mm_t *) manager;
    header_t *firstBlock = (header_t *) memoryRegion;
    
    firstBlock->s.next = NULL;
    firstBlock->s.size = regionSize - sizeof(header_t);
    firstBlock->s.isFree= 1;

    memoryManager->firstBlock = firstBlock;
    memoryManager->memStart = (uintptr_t) memoryRegion;
    memoryManager->memEnd = memoryManager->memStart + regionSize;

    return memoryManager;
}

void *malloc(mm_t *mgr, size_t size) {
    if(!mgr || !size) {
        return NULL; // params
    }

    header_t *current = mgr->firstBlock;
    while(1) {
        uintptr_t realSize = size + sizeof(header_t);
        uintptr_t newAllocEnd = (uintptr_t) current + realSize;
        uintptr_t boundary = (current->s.next ? (uintptr_t) current->s.next : mgr->memEnd);
        if(current->s.isFree) {
            if(newAllocEnd < boundary - sizeof(header_t)) {
                header_t *next = (header_t *) newAllocEnd;

                next->s.next = current->s.next;
                next->s.size = current->s.size - (size + sizeof(header_t));
                next->s.isFree = 1;
                
                current->s.isFree = 0;
                current->s.size = size;
                current->s.next = next;

                return (void *) ((uintptr_t) current + sizeof(header_t)); // success y crea un nuevo header
            } else if(newAllocEnd <= boundary) {
                    current->s.isFree = 0;
                    return (void *) ((uintptr_t) current + sizeof(header_t)); // success, pero no crea un nuevo header 'next' ya que no hay lugar
            }
        }   
        if(!current->s.next) {
            return NULL; // no encontro suficiente espacio
        }
        current = current->s.next;
    }
}