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
 * @brief 0 if Failed | Pointer to Added If Success
 * 
 * @param arrayName swcArrayName
 * @param data the data that is being stored... not a pointer the actual data
 * @param sorter sorter func to place the data inside the array
 * @param manager the memorymanager...
 * 
 */
#define swcAddArray(arrayName, data, sorter, manager) \
        addArray(arrayName, sizeof(typeof(data)), &data, sorter, manager)
        
void* addAtArray(swcNameStruct *nameArray, uint32_t dataSize, void* data, uint32_t index, swcMemMan* manager);
/**
 * @brief Adds at index, 0 if Failed | Pointer to Added if Success, Should only be used in conjuction with contains array to preserve
 * the sorting of the array
 * 
 * @param arrayName swcArrayName
 * @param data the data that is being stored, not pointer
 * @param index index to insert at
 * @param manager the memorymanager
 * 
 */
#define swcAddAtArray(arrayName, data, index, manager) \
        addAtArray(retrieveNameL(arrayName, manager), sizeof(typeof(data)), &data, index, manager)

uint32_t removeArray(swcArrayName arrayName, uint32_t dataSize, void* data, sortFunc sorter, swcMemMan* manager);
/**
 * @brief 0 if Failed | 1 if Success
 *
 * @param arrayName swcArrayName
 * @param data the data that is being stored... not a pointer the actual data
 * @param sorter sorter func to place the data inside the array
 * @param manager the memorymanager...
 * 
 */
#define swcRemoveArray(arrayName, data, sorter, manager) \
        removeArray(arrayName, sizeof(typeof(data)), &data, sorter, manager)

uint32_t removeAtArray(swcNameStruct *nameArray, uint32_t dataSize, uint32_t index, swcMemMan* manager);
/**
 * @brief Removes at Index | 0 if Failed | 1 if Success, Only Use in Conjunction with Contains array
 * 
 * @param arrayName swcArrayName
 * @param dataType actual datatype, very cool
 * @param index to remove at
 * @param manager the memorymanager...
 */
#define swcRemoveAtArray(arrayName, dataType, index, manager) \
        removeAtArray(retrieveNameL(arrayName, manager), sizeof(dataType), index, manager)

int32_t containsArray(swcArrayName arrayName, uint32_t dataSize, void* data, sortFunc sorter, swcMemMan* manager);
/**
 * @brief -1 if Failed | Index if Contained | -2 - Index if Not Contained
 * 
 * @param arrayName swcArrayName
 * @param data the data that is being stored... not a pointer the actual data
 * @param sorter sorter func to place the data inside the array
 * @param manager the memorymanager...
 * 
 */
#define swcContainsArray(arrayName, data, sorter, manager) \
        containsArray(arrayName, sizeof(typeof(data)), &data, sorter, manager)

void* retrieveDataArray(swcArrayName arrayName, uint32_t dataSize, void* data, sortFunc sorter, swcMemMan* manager);
/**
 * @brief 0 if Not Found | Pointer if FOund
 * 
 * @param arrayName swcArrayName
 * @param data the "fake" data to be used to sort
 * @param sorter sorter func to find data
 * @param manager the memorymanager
 */
#define swcRetrieveArray(arrayName, data, sorter, manager) \
        retrieveDataArray(arrayName, sizeof(typeof(data)), &data, sorter, manager)

void* retrieveAtArray(swcNameStruct *nameArray, uint32_t dataSize, uint32_t index, swcMemMan* manager);
/**
 * @brief Retrieves data from array at index index, should only be used in conjunction iwth contains array as it is
 * not memory safe
 * 
 * @param arrayName swcArrayName
 * @param type type of data being stored for alignment purposes
 * @param index index tor retrieve from
 * @param manager manager
 * 
 */
#define swcRetrieveAtArray(arrayName, type, index, manager) \
        retrieveAtArray(retrieveNameL(arrayName, manager), sizeof(type), index, manager)

swcArray* retrieveArray(swcArrayName name, swcMemMan* manager);

void* allocSB(size_t size, swcMemMan* manager);
void* allocDB(size_t size, swcMemMan* manager);
void* retrieveName(uint32_t name, swcMemMan *manager);
swcNameStruct* retrieveNameL(uint32_t name, swcMemMan* manager);
swcArena* addArena(size_t size, size_t minData, swcMemMan *manager);
uint32_t frameChange(swcMemMan* manager);
uint32_t freeMemMan(swcMemMan* manager);

/*
*       SORTERS
*/
uint32_t uint64_tSorter(void* left, void* right);
uint32_t uint32_tSorter(void* left, void* right);
uint32_t programNameSorter(void* left, void* right);

#endif