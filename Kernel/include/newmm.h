#ifndef NEWMM_H
#define NEWMM_H

#include <stddef.h>
#include <stdint.h>

typedef struct header {
    struct header *next;
    size_t size;
    int isFree;
} header_t;

typedef struct {
    header_t *firstBlock;
    uintptr_t memStart;
    uintptr_t memEnd;
} mm_t ;

mm_t *createMemoryManager_mm(void * manager, void *memoryRegion, size_t regionSize);

void *malloc_mm(mm_t *mgr, size_t size);

void free_mm(mm_t *mgr, void *memToFree);

#endif