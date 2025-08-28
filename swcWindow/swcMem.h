#ifndef SWCMEM_H
#define SWCMEM_H

#include "swcDef.h"

#define nameArrayReallocSize 1.20f

swcArena creArena(size_t size, size_t minData);
uint32_t freeArena(swcArena *arena);
void *alloc(swcArena *a, size_t size);

swcMemMan createMan(uint32_t arCount, uint32_t naCount, uint32_t singleBufSize, uint32_t doubleBufSize);
void* allocM(size_t size, swcMemMan* manager);

swcName allocNamed(size_t size, swcMemMan* manager);
uint32_t deallocNamed(swcName name, swcMemMan* manager);
uint32_t reallocNamed(swcName name, uint32_t newSize, swcMemMan* manager, swcNameStruct* swcN);

swcArrayName allocArray(uint32_t size, size_t dataSize, swcMemMan* manager);
#define swcAllocArray(size, data, manager) \
        allocArray(size, sizeof(data), manager); \

void* addArray(swcArrayName arrayName, uint32_t dataSize, void* data, sortFunc sorter, swcMemMan* manager);
/**
 * @brief 0 if Failed | Pointer to Added/Retrieved If Success
 * 
 */
#define swcAddArray(arrayName, data, sorter, manager) \
        addArray(arrayName, sizeof(typeof(data)), &data, sorter, manager)

uint32_t removeArray(swcArrayName arrayName, uint32_t dataSize, void* data, sortFunc sorter, swcMemMan* manager);
/**
 * @brief 0 if Failed | 1 if Success
 * 
 */
#define swcRemoveArray(arrayName, data, sorter, manager) \
        removeArray(arrayName, sizeof(typeof(data)), &data, sorter, manager)
int32_t containsArray(swcArrayName arrayName, uint32_t dataSize, void* data, sortFunc sorter, swcMemMan* manager);
/**
 * @brief -1 if Not Contained | Index if Contained
 * 
 */
#define swcContainsArray(arrayName, data, sorter, manager) \
        containsArray(arrayName, sizeof(typeof(data)), &data, sorter, manager)
swcArray* retrieveArray(swcArrayName name, swcMemMan* manager);

void* allocSB(size_t size, swcMemMan* manager);
void* allocDB(size_t size, swcMemMan* manager);
void* retrieveName(uint32_t name, swcMemMan *manager);
swcNameStruct* retrieveNameL(uint32_t name, swcMemMan* manager);
swcArena* addArena(size_t size, size_t minData, swcMemMan *manager);
uint32_t frameChange(swcMemMan* manager);
uint32_t freeMemMan(swcMemMan* manager);

#endif