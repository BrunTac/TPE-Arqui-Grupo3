#include <newmm.h>
#include <processManager.h>
#include <stddef.h>
#include <stdint.h>

mm_t *createMemoryManager_mm(void * manager, void *memoryRegion, size_t regionSize) {
    if(!memoryRegion || !manager || regionSize < sizeof(header_t)) {
        return NULL; // params
    }

	mm_t *memoryManager = (mm_t *) manager;
    header_t *firstBlock = (header_t *) memoryRegion;
    
    firstBlock->next = NULL;
    firstBlock->size = regionSize - sizeof(header_t);
    firstBlock->isFree= 1;

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
        uintptr_t boundary = (current->next ? (uintptr_t) current->next : mgr->memEnd);
        if(current->isFree) {
            if(newAllocEnd < boundary - sizeof(header_t)) {
                header_t *next = (header_t *) newAllocEnd;

                next->next = current->next;
                next->size = current->size - (size + sizeof(header_t));
                next->isFree = 1;
                
                current->isFree = 0;
                current->size = size;
                current->next = next;

                return (void *) ((uintptr_t) current + sizeof(header_t)); // success y crea un nuevo header
            } else if(newAllocEnd <= boundary) {
                    current->isFree = 0;
                    return (void *) ((uintptr_t) current + sizeof(header_t)); // success, pero no crea un nuevo header 'next' ya que no hay lugar
            }
        }   
        current = current->next;
    }
    return NULL; // llego a null y no encontro espacio
}

void free_mm(mm_t *mgr, void *memToFree) {
    if(!memToFree || !mgr)
        return; // params check

    header_t *current = mgr->firstBlock;
    while(current) {
        if((uintptr_t) current == (uintptr_t) memToFree - sizeof(header_t)) {
            current->isFree = 1;
            for(header_t *next = current->next ; next && next->isFree ; next = next->next) {
                current->next = next->next;
                current->size += next->size + sizeof(header_t);
            }
            return; // success, y se asegura de tomar todos los bloques liberados contiguos en frente suyo
        }
        current = current->next;
    }
    return; // no encontro el puntero al bloque pedido para liberar
}

void *malloc(size_t size) {
    return malloc_mm(heapManager, size);
}

void free(void *memToFree) {
    free_mm(heapManager, memToFree);
}