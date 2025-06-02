#ifndef NEWMM_H
#define NEWMM_H

#include <stddef.h>
#include <stdint.h>

#define SIZEOF_HEADER sizeof(header_t)

typedef char ALIGN[SIZEOF_HEADER];

typedef union header {
    struct {
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

mm_t *createMemoryManager(void * manager, void *memoryRegion, size_t regionSize);

void *malloc_mm(mm_t *mgr, size_t size);

void free_mm(mm_t *mgr, void *memToFree);

#endif