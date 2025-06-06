#ifndef NEWMM_H
#define NEWMM_H


mm_t *createMemoryManager_mm(void * manager, void *memoryRegion, size_t regionSize);

void *malloc_mm(mm_t *mgr, size_t size);

void free_mm(mm_t *mgr, void *memToFree);

void *malloc(size_t size);

void free(void *memToFree);

#endif