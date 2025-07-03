#ifndef SWCMEM_H
#define SWCMEM_H

#include "swcDef.h"

#define nameArrayReallocSize 1.20f

swcArena creArena(size_t size, size_t minData);
uint32_t freeArena(swcArena *arena);
void *alloc(swcArena *a, size_t size);

swcMemMan createMan(uint32_t arCount, uint32_t naCount, uint32_t singleBufSize, uint32_t doubleBufSize);
void* allocM(size_t size, swcMemMan* manager);
uint32_t allocNamed(size_t size, swcMemMan* manager);
uint32_t deallocNamed(uint32_t name, swcMemMan* manager);
//TODO:
//uint32_t reallocNamed
uint32_t reallocNamed(uint32_t name, uint32_t newSize, swcMemMan* manager, swcName* swcN);
swcNameArray allocNameArray(uint32_t size, swcMemMan* manager);
void* allocSB(size_t size, swcMemMan* manager);
void* allocDB(size_t size, swcMemMan* manager);
void* retrieveName(uint32_t name, swcMemMan *manager);
swcName* retrieveNameL(uint32_t name, swcMemMan* manager);
swcArena* addArena(size_t size, size_t minData, swcMemMan *manager);
uint32_t frameChange(swcMemMan* manager);


#endif