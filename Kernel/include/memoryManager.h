#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdint.h>
#include <structs.h>

#ifdef USE_BUDDY_MM
    typedef bmm_t memory_manager_t;
#elif USE_NORMAL_MM
    typedef mm_t memory_manager_t;
#else
    #error "Must define either USE_BUDDY_MM or USE_NORMAL_MM"
#endif

memory_manager_t *createMemoryManager_mm(void * manager, void *memoryRegion, size_t regionSize);

void *malloc_mm(memory_manager_t *mgr, size_t size);

void free_mm(memory_manager_t *mgr, void *memToFree);

void *malloc(size_t size);

void free(void *memToFree);

#endif