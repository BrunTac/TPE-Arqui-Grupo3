#include <memoryManager.h>
#include <processManager.h>
#include <structs.h>
#include <stddef.h>
#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>

#define BLOCK_HEIGHT 3
#define BLOCKS_START_Y 5
#define SCREEN_MARGIN 2

memory_manager_t *createMemoryManager_mm(void * manager, void *memoryRegion, size_t regionSize) {
    if(!memoryRegion || !manager || regionSize < sizeof(buddy_t)) {
        return NULL; // params
    }

	memory_manager_t *memoryManager = (memory_manager_t *) manager;
    buddy_t *megaBuddy = (buddy_t *) memoryRegion;
    
    megaBuddy->buddy = NULL;
    megaBuddy->left = NULL;
    megaBuddy->right = NULL;
    megaBuddy->parent = NULL;
    megaBuddy->size = regionSize - sizeof(buddy_t);
    megaBuddy->isFree= 1;

    memoryManager->megaBuddy = megaBuddy;
    memoryManager->memStart = (uintptr_t) memoryRegion;
    memoryManager->memEnd = memoryManager->memStart + regionSize;

    return memoryManager;
}

static void *find_free_block(buddy_t *node, size_t size) {
    if(!node) {
        return NULL;
    }

    // es una hoja => capaz podemos guardar info dentro de este nodo
    if(!node->left && !node->right) {
        // nos fijamos si corresponde guardar o dividir
        if(node->isFree) { 
            if(node->size/2 < size && size <= node->size) {
                node->isFree = 0;
                return (void *) ((uintptr_t) node + sizeof(buddy_t)); // ocupa la hoja encontrada   
            } else if(size <= node->size/2) { // es demasiado chico para ocupar la mem entera => se divide
                node->isFree = 0;
                
                buddy_t *leftBuddy = (buddy_t *)((uintptr_t)node + sizeof(buddy_t));
                buddy_t *rightBuddy = (buddy_t *)((uintptr_t)leftBuddy + sizeof(buddy_t) + (node->size - 2*sizeof(buddy_t))/2);
                
                leftBuddy->size = (node->size - 2*sizeof(buddy_t))/2;
                leftBuddy->isFree = 1;
                leftBuddy->left = NULL;
                leftBuddy->right = NULL;
                leftBuddy->buddy = rightBuddy;
                leftBuddy->parent = node;
                
                rightBuddy->size = (node->size - 2*sizeof(buddy_t))/2;
                rightBuddy->isFree = 1;
                rightBuddy->left = NULL;
                rightBuddy->right = NULL;
                rightBuddy->buddy = leftBuddy;
                rightBuddy->parent = node;
                
                node->left = leftBuddy;
                node->right = rightBuddy;
                
                return find_free_block(leftBuddy, size); // divido la hoja encontrada y me fijo uno de los hijos (deberia bastar con esto)
            }
        }
        return NULL; // la hoja esta ocupada o la memoria a guardar es demasiado grande
    }

    // no es una hoja => me fijo en los hijos izquierdos y derechos si encuentro una hoja valida
    if(node->left) {
        void *result = find_free_block(node->left, size);
        if(result) {
            return result;
        }
        return find_free_block(node->right, size);
    }
    return NULL;
}

void *malloc_mm(memory_manager_t *mgr, size_t size) {
    if(!mgr || !size) {
        return NULL; // param check 
    }    
    
    return find_free_block(mgr->megaBuddy, size);
}

static buddy_t *find_node(buddy_t *node, void *memToFree) {
    if(!node) {
        return NULL;
    }

    // se fija si memToFree es igual a la memoria asignada a node
    void *nodeData = (void *)((uintptr_t)node + sizeof(buddy_t));
    if(nodeData == memToFree && !node->left && !node->right) { // se asegura de que sea una hoja
        return node;
    }

    // no encontro memToFree o no era hoja => sigo buscando
    if(node->left) {
        buddy_t *result = find_node(node->left, memToFree);
        if(result) {
            return result;
        }
        return find_node(node->right, memToFree);
    }

    return NULL; // no encontro memToFree
}

static buddy_t *try_coalesce(buddy_t *node) {
    if(!node || !node->buddy || !node->parent) {
        return node;
    }

    // el buddy esta libre => los junto haciendo que el padre sea hoja
    if(node->isFree && node->buddy->isFree) {
        buddy_t *parent = node->parent;
        size_t sizeToSet = node->size + node->buddy->size + 2*sizeof(buddy_t);
        
        parent->left = NULL;
        parent->right = NULL;
        parent->isFree = 1;
        parent->size = sizeToSet;
        
        return parent;
    }

    return node; // el buddy no estaba free
} 

void free_mm(memory_manager_t *mgr, void *memToFree) {
    if(!mgr || !memToFree) {
        return;
    }

    // busco el nodo a liberar
    buddy_t *node = find_node(mgr->megaBuddy, memToFree);
    if(!node) {
        return; // me pasaste cualquier cosa loko
    }

    node->isFree = 1;

    // intento de juntar los buddies, corta cuando llego a megaBuddy (no tiene buddy) o simplemente no pude juntar alguno
    while(node != mgr->megaBuddy && node->parent) {
        buddy_t *coalesced = try_coalesce(node);
        if(coalesced == node) {
            break;
        }
        node = coalesced;
    }
}

void *malloc(size_t size) {
    return malloc_mm(heapManager, size);
}

void free(void *memToFree) {
    free_mm(heapManager, memToFree);
}

void visualizeMemory(bmm_t *mgr) {
    if (!mgr || !mgr->megaBuddy)
        return;

    // Clear screen for visualization
    clear();

    // Calculate total memory size and usage
    size_t totalMemory = mgr->memEnd - mgr->memStart;
    size_t usedMemory = 0;
    size_t fragmentCount = 0;
    buddy_t *current = mgr->megaBuddy;

    // First pass: calculate statistics
    while (current) {
        if (!current->isFree) {
            usedMemory += current->size + sizeof(buddy_t);
        }
        fragmentCount++;
        current = current->left;
    }

    // Print memory statistics
    char buffer[20];
    printsInPos("Memory Usage Statistics:", SCREEN_MARGIN, 1, WHITE, BLACK);
    
    uintToBase(usedMemory, buffer, 10);
    printsInPos("Used Memory: ", SCREEN_MARGIN, 2, WHITE, BLACK);
    printsInPos(buffer, 15, 2, YELLOW, BLACK);
    
    uintToBase(totalMemory, buffer, 10);
    printsInPos("Total Memory: ", SCREEN_MARGIN, 3, WHITE, BLACK);
    printsInPos(buffer, 15, 3, YELLOW, BLACK);
    
    uintToBase(fragmentCount, buffer, 10);
    printsInPos("Fragments: ", SCREEN_MARGIN, 4, WHITE, BLACK);
    printsInPos(buffer, 15, 4, YELLOW, BLACK);

    // Visualize memory blocks
    current = mgr->megaBuddy;
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
        current = current->left;
    }

    // Print legend
    printsInPos("Legend:", SCREEN_MARGIN, BLOCKS_START_Y + BLOCK_HEIGHT + 2, WHITE, BLACK);
    printsInPos("[ ]", SCREEN_MARGIN, BLOCKS_START_Y + BLOCK_HEIGHT + 3, WHITE, GREEN);
    printsInPos(" - Free Memory", SCREEN_MARGIN + 4, BLOCKS_START_Y + BLOCK_HEIGHT + 3, WHITE, BLACK);
    printsInPos("[ ]", SCREEN_MARGIN, BLOCKS_START_Y + BLOCK_HEIGHT + 4, WHITE, RED);
    printsInPos(" - Used Memory", SCREEN_MARGIN + 4, BLOCKS_START_Y + BLOCK_HEIGHT + 4, WHITE, BLACK);
}

void viewmem() {
    visualizeMemory(heapManager);
}
