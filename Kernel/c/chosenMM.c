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
    uintToBase(usedMemory, buffer, 10);
    prints("Memoria usada: ", WHITE, BLACK); prints(buffer, WHITE, BLACK); print_newline();
    uintToBase(fragmentCount, buffer, 10);
    prints("Fragment count: ", WHITE, BLACK); prints(buffer, WHITE, BLACK); print_newline();

    // Visualize memory blocks
    /* current = mgr->firstBlock;
    int blockX = SCREEN_MARGIN;
    int maxWidth = getWidth() - (2 * SCREEN_MARGIN);

    while (current) {
        // Calculate block width proportional to its size
        int blockWidth = (current->size * maxWidth) / totalMemory;
        if (blockWidth < 4) blockWidth = 4; // Minimum visible width

        // Draw block
        Color blockColor = current->isFree ? GREEN : RED;
        
        // Draw block border
        for (int x = blockX; x < blockX + blockWidth; x++) {
            printsInPos("-", x, BLOCKS_START_Y, WHITE, BLACK);
            printsInPos("-", x, BLOCKS_START_Y + BLOCK_HEIGHT, WHITE, BLACK);
        }
        for (int y = BLOCKS_START_Y; y <= BLOCKS_START_Y + BLOCK_HEIGHT; y++) {
            printsInPos("|", blockX, y, WHITE, BLACK);
            printsInPos("|", blockX + blockWidth - 1, y, WHITE, BLACK);
        }

        // Fill block
        for (int y = BLOCKS_START_Y + 1; y < BLOCKS_START_Y + BLOCK_HEIGHT; y++) {
            for (int x = blockX + 1; x < blockX + blockWidth - 1; x++) {
                printsInPos(" ", x, y, WHITE, blockColor);
            }
        }

        // Print block size
        uintToBase(current->size, buffer, 10);
        int textX = blockX + (blockWidth / 2) - (strlen(buffer) / 2);
        printsInPos(buffer, textX, BLOCKS_START_Y + 1, WHITE, blockColor);

        blockX += blockWidth;
        current = current->next;
    }

    // Print legend
    printsInPos("Legend:", SCREEN_MARGIN, BLOCKS_START_Y + BLOCK_HEIGHT + 2, WHITE, BLACK);
    printsInPos("[ ]", SCREEN_MARGIN, BLOCKS_START_Y + BLOCK_HEIGHT + 3, WHITE, GREEN);
    printsInPos(" - Free Memory", SCREEN_MARGIN + 4, BLOCKS_START_Y + BLOCK_HEIGHT + 3, WHITE, BLACK);
    printsInPos("[ ]", SCREEN_MARGIN, BLOCKS_START_Y + BLOCK_HEIGHT + 4, WHITE, RED);
    printsInPos(" - Used Memory", SCREEN_MARGIN + 4, BLOCKS_START_Y + BLOCK_HEIGHT + 4, WHITE, BLACK); */
}

void viewmem() {
    visualizeMemory(heapManager);
}