#include <memoryManager.h>
#include <structs.h>
#include <processManager.h>
#include <stddef.h>
#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>

#define BLOCK_HEIGHT 3
#define BLOCKS_START_Y 5
#define SCREEN_MARGIN 2

memory_manager_t *createMemoryManager_mm(void * manager, void *memoryRegion, size_t regionSize) {
    if(!memoryRegion || !manager || regionSize < sizeof(header_t)) {
        return NULL; // params
    }

	memory_manager_t *memoryManager = (memory_manager_t *) manager;
    header_t *firstBlock = (header_t *) memoryRegion;
    
    firstBlock->next = NULL;
    firstBlock->size = regionSize - sizeof(header_t);
    firstBlock->isFree= 1;

    memoryManager->firstBlock = firstBlock;
    memoryManager->memStart = (uintptr_t) memoryRegion;
    memoryManager->memEnd = memoryManager->memStart + regionSize;

    return memoryManager;
}

void *malloc_mm(memory_manager_t *mgr, size_t size) {
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

void free_mm(memory_manager_t *mgr, void *memToFree) {
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

void visualizeMemory(mm_t *mgr) {
    if (!mgr || !mgr->firstBlock)
        return;

    clear();

    // obtengo la cantidad de memoria a cargo del manager, la cantidad de memoria usada
    // y los "fragmentos" (basicamente cantidad de cosas usando mem)
    size_t totalMemory = mgr->memEnd - mgr->memStart;
    size_t usedMemory = 0;
    size_t fragmentCount = 0;
    header_t *current = mgr->firstBlock;

    // calculo tanto la cantidad de memoria usada como los fragmentos
    while (current) {
        if (!current->isFree) {
            usedMemory += current->size + sizeof(header_t);
        }
        fragmentCount++;
        current = current->next;
    }

    // imprimo lo anterior (mem total, mem usada y cant fragmentos)
    char buffer[20];
    uintToBase(totalMemory, buffer, 10);
    prints("Memoria total: ", WHITE, BLACK); prints(buffer, WHITE, BLACK); print_newline();
    uintToBase(totalMemory - usedMemory, buffer, 10); 
    prints("Memoria libre: ", WHITE, BLACK); prints(buffer, WHITE, BLACK); print_newline();
    uintToBase(usedMemory, buffer, 10);
    prints("Memoria usada: ", WHITE, BLACK); prints(buffer, WHITE, BLACK); print_newline();
    uintToBase(fragmentCount, buffer, 10);
    prints("Fragment count: ", WHITE, BLACK); prints(buffer, WHITE, BLACK); print_newline();
}

void viewmem() {
    visualizeMemory(heapManager);
}