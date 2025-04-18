#ifndef SWCMEM_H
#define SWCMEM_H

#include "swcDef.h"

swcArena creArena(size_t size, size_t minData);
uint32_t freeArena(swcArena *arena);
void *alloc(swcArena *a, size_t size);

swcMemMan createMan(uint32_t stSize, uint32_t count);
void* allocM(size_t size, swcMemMan* manager);
uint32_t allocNamed(size_t size, swcMemMan *manager);
uint32_t deallocNamed(uint32_t name, swcMemMan *manager);
void* retrieveName(uint32_t name, swcMemMan *manager);
swcArena* addArena(size_t size, size_t minData, swcMemMan *manager);

#endif