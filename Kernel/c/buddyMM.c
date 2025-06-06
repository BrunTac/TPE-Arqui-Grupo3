#include <buddyMM.h>
#include <processManager.h>
#include <structs.h>
#include <stddef.h>
#include <stdint.h>

bmm_t *createMemoryManager_mm(void * manager, void *memoryRegion, size_t regionSize) {
    if(!memoryRegion || !manager || regionSize < sizeof()) {
        return NULL; // params
    }

	bmm_t *memoryManager = (mm_t *) manager;
    buddy_t *megaBuddy = (buddy_t *) memoryRegion;
    
    megaBuddy->buddy = NULL;
    megaBuddy->left = NULL;
    megaBuddy->right = NULL;
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
            } else if(size <= node->size/2) { // 
                buddy_t *leftBuddy = (buddy_t *)((uintptr_t)node + sizeof(buddy_t));
                buddy_t *rightBuddy = (buddy_t *)((uintptr_t)leftBuddy + sizeof(buddy_t) + (node->size - 2*sizeof(buddy_t))/2);
                
                leftBuddy->size = (node->size - 2*sizeof(buddy_t))/2;
                leftBuddy->isFree = 1;
                leftBuddy->left = NULL;
                leftBuddy->right = NULL;
                leftBuddy->buddy = rightBuddy;
                
                rightBuddy->size = (node->size - 2*sizeof(buddy_t))/2;
                rightBuddy->isFree = 1;
                rightBuddy->left = NULL;
                rightBuddy->right = NULL;
                rightBuddy->buddy = leftBuddy;
                
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

void *malloc_mm(bmm_t *mgr, size_t size) {
    if(!mgr || !size) {
        return NULL; // param check
    }    
    
    return find_free_block(mgr->megaBuddy, size);
}

