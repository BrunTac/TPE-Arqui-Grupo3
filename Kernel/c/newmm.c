#include "newmm.h"
#include <stddef.h>
#include <stdint.h>

mm_t *createMemoryManager(void * manager, void *memoryRegion, size_t regionSize) {
    if(!memoryRegion || !manager || regionSize < sizeof(header_t)) {
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

void *malloc_mm(mm_t *mgr, size_t size) {
    if(!mgr || !size) {
        return NULL; // param check
    }

    header_t *current = mgr->firstBlock;
    while(current) {
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
        current = current->s.next;
    }
    return NULL; // llego a null y no encontro espacio
}

void free_mm(mm_t *mgr, void *memToFree) {
    if(!memToFree || !mgr)
        return; // params check

    header_t *current = mgr->firstBlock;
    while(current) {
        if((uintptr_t) current == (uintptr_t) memToFree - sizeof(header_t)) {
            current->s.isFree = 1;
            for(header_t *next = current->s.next ; next && next->s.isFree ; next = next->s.next) {
                current->s.next = next->s.next;
                current->s.size += next->s.size + sizeof(header_t);
            }
            return; // success, y se asegura de tomar todos los bloques liberados contiguos en frente suyo
        }
        current = current->s.next;
    }
    return; // no encontro el puntero al bloque pedido para liberar
}