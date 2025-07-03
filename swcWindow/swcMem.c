#include "swcMem.h"



/**
 * @brief Returns the name of from manager, if ret is null does not exist
 * if size is 0 name is deallocated
 * 
 * @param name 
 * @param manager 
 * @return swcName* 
 */
static inline swcName* binSearch(uint32_t name, swcMemMan* manager)
{
    swcName* root = manager->namesTree;
    while(root != NULL && root->name != name)
    {
        if(name > root->name)
        {
            root = root->rChild;
            continue;
        }
        root = root->lChild;
    }
    return root;
}

/**
 * @brief Create and return an arena of size 
 * 
 * @param size The amount of memory to allocate
 * @param minData Maximum data size will fail upon minData < size
 * @return swcArena 
 */
swcArena creArena(size_t size, size_t minData)
{
    //TODO: couple with garbage collector, create failure output
    if(minData > size)
    {
        //fail
    }
    swcArena arena = {0};
    arena.origin = calloc(1, size);//need initialization to zero for assigned pointers
    arena.beg = arena.origin;
    arena.end = arena.beg + size;
    arena.size = size;
    arena.minData = minData;

    return arena;
}
/**
 * @brief Destroys arena
 * 
 * @param arena 
 * @return uint32_t 
 */

uint32_t freeArena(swcArena *arena)
{
    free(arena->origin);
    memset(arena, 0, sizeof(swcArena));
}

/**
 * @brief return pointer with allocated size
 * 
 * @param size 
 * @param count 
 * @return void* || Returns allocated data if success, if fail nullptr if failed(no space left in arena) 
 */
void *alloc(swcArena *a, size_t size)
{
    //everythings aligned on 16 bytes
    //dereference pointer at the end of all data so on dealloc calls can say data is gone
    //first find padding for original data
    //second alignment with dereference pointer
    size_t padding = -(size_t)a->beg & (uint32_t)15;
    if(a->end - a->beg - padding < size)
    {
        return 0;
    }
    void *pointer;
    a->beg += padding;
    pointer = a->beg;
    a->beg += size;
    return pointer;
}

//above func heavily inspired by:
//https://nullprogram.com/blog/2023/09/27/
//srry for not a proper reference :P


/**
 * @brief Create a memory manager with initial size of arCount
 * 
 * @param arCount Desired Arena Count
 * @param naCount Names Initialization
 * @param singleBufSize Starting size for the single buffer
 * @param doubleBufSize Starting size for the double buffers
 * @return swcMemMan 
 */
swcMemMan createMan(uint32_t arCount, uint32_t naCount, uint32_t singleBufSize, uint32_t doubleBufSize)
{
    swcMemMan ret = 
    {
        .count = 0,
        .size = arCount + 1,
        .arenas = (swcArena*)calloc(arCount + 1, sizeof(swcArena)),
        .nameSize = naCount,
        .nameCount = 1,//tree start
        .singleBuffer = creArena(singleBufSize, (size_t)1),
        .doubleBuffer1 = creArena(doubleBufSize, (size_t)1),
        .doubleBuffer2 = creArena(doubleBufSize, (size_t)1),
        .curDB = &ret.doubleBuffer1,

    };
    swcArena *a = addArena((sizeof(swcName) + (-sizeof(swcName) & alignof(swcName))) * naCount, sizeof(swcName), &ret);
    //TODO: change maxsize to something dynamic

    ret.namesTree = (swcName*)alloc(a, sizeof(swcName));
    ret.namesTree->name = 1 << 15;//for binary tree start, max divs i guess is sizeof(uint32_t) - 1
    ret.namesTree->pointer = NULL;
    ret.namesTree->size = 0;
    ret.namesTree->lSize = 0;
    ret.namesTree->rSize = 0;

    return ret;
}

/**
 * @brief Add an arena to the manager returns null if would exceed arena count
 * 
 * @param size 
 * @param avgData 
 * @param manager 
 * @return swcArena* 
 */
swcArena* addArena(size_t size, size_t avgData, swcMemMan* manager)
{
    if(manager->count < manager->size)
    {
        manager->arenas[manager->count] = creArena(size, avgData);
        manager->count++; 
        return &(manager->arenas[manager->count - 1]);
    }
    return NULL;
}

/**
 * @brief returns pointer of data block of size size attached to manager
 * 
 * @param size 
 * @param manager 
 * @return void* || Returns allocated if success if fail returns NULL
 */
void *allocM(size_t size, swcMemMan *manager)
{

    swcArena* a = &manager->arenas[manager->count - 1];

    void* data = alloc(a, size);

    return data;

}

/**
 * @brief Allocates a named memory space, to be used for long term storage (Ideally for divs only).
 * Uses a binary tree algo to generate names, for speed binary nodes store node sizes
 * 
 * @param size 
 * @return Returns name
 */
uint32_t allocNamed(size_t size, swcMemMan* manager)
{
    void* data = allocM(size, manager);

    //TODO: account for null return...
    


    //gen name

    swcName* root = manager->namesTree;
    uint32_t genName = root->name >> 1;
    while(root->lChild != NULL && root->rChild != NULL)
    {
        genName >>= 1;
        if(root->lSize > root->rSize)
        {
            root->rSize++;
            root = root->rChild;
            
            continue;
        }
        root->lSize++;
        root = root->lChild;
    }

    if(genName == 0)
    {
        //reached end of generatable tree
        return 0;
    }
    if(root->lSize > root->rSize)
    {
        genName += root->name;
    }
    else
    {
        genName = root->name - genName;
    }

    swcName* name = (swcName*)alloc(manager->arenas, sizeof(swcName));

    if(root->lSize > root->rSize)
    {
        root->rChild = name;
        root->rSize++;
    }
    else
    {
        root->lChild = name;
        root->lSize++;
    }

    name->name = genName;
    name->pointer = data;
    name->par = root;
    name->size = size;

    manager->nameCount++;

    return genName;
}

/**
 * @brief Sets a names pointer to null, 
 * cannot set a names name to zero as to maintain a binary tree a name cannot be set to 0
 * otherwise its unsortable
 * 
 * @param name 
 * @return uint32_t dealloced name if succes, 0 if failure
 */
uint32_t deallocNamed(uint32_t name, swcMemMan* manager)
{
    swcName* root = binSearch(name, manager);
    if(root == NULL)
    {
        return 0;
    }
    root->pointer = 0;
    return name;
}

/**
 * @brief reallocates a name to the size newsize, if size is less than current size the only change is the size parameter on the name struct, if greater than checks
 * to see if the name is at the end of allocated data currently, if so expands the data allocated to name if possible, if not allocates a new memory location to name
 * and copies the data over, and expands the size
 * 
 * @param name 
 * @param newSize New Size for Name
 * @param manager 
 * @param swcN I don't see a situation you call this func without having this too
 * @return 1 Upon Success | 0 Upon Failure 
 */
uint32_t reallocNamed(uint32_t name, uint32_t newSize, swcMemMan* manager, swcName* swcN)
{
    swcName* swcName = retrieveNameL(name, manager);
    if(swcName == NULL)
    {
        return 0;
    }
    swcN = swcName;
    if(swcName->size >= newSize)
    {
        swcName->size = newSize;
        return 1;
    }
    swcArena* a = &manager->arenas[manager->count - 1];
    if(a->beg == (swcName->pointer + (size_t)(swcName->size)))
    {
        //this was the last name "added" to the arena, attempt to expand name
        if((uint32_t)(a->end - a->beg) >= newSize - swcName->size)
        {
            a->beg += (sizeof(char) * (newSize - swcName->size));
            swcName->size = newSize;
            return 1;
        }
    }
    
    void* newPointer = allocM(newSize, manager);
    if(newPointer == NULL)
    {
        return 0;
    }
    swcName->pointer = newPointer;
    swcName->size = newSize;
    return 1;
    

}

/**
 * @brief Creates an Array, memory size = (size) * dataSize + sizeof(uint32_t) + sizeof(size_t) + sizeof(uintptr_t)
 * 
 * @param size Elements needed
 * @param dataSize Size of each element
 * @param sorter Function to sort each element with
 * @param assigner Function to assign to an element too
 * @param manager 
 * @return swcNameArray 
 */
swcArrayName allocArray(uint32_t size, size_t dataSize, sortFunc sorter, assigner assigner, swcMemMan* manager)
{
    swcArrayName ret = allocNamed(size * dataSize + sizeof(uint32_t) + sizeof(size_t) + sizeof(uintptr_t), manager);
    swcName *arrayName = retrieveNameL(ret, manager);
    swcArray *array = (swcArray*)(arrayName->pointer);
    array->curSize = 0;
    array->dataSize = dataSize;
    array->sorter = sorter;
    array->assigner = assigner;
    return ret; //hhmmmmm
}
/**
 * @brief Adds a value to name array checks to see if value has been added before... only single instances in this array
 * uses basic sorting algo to find if name has been added
 * 
 * @param arrayName Array name
 * @param name Name to add to Name array
 * @param manager 
 * @return 0 if Failed | 1 if Success (Failure can Occur for Name Array not Existing, Memory Overload, Name Already Existing)
 */
uint32_t addArray(swcArrayName arrayName, void* data, swcMemMan* manager)
{

    swcName* nameArray = retrieveNameL(arrayName, manager);
    if(nameArray == NULL)
    {
        return 0;
    }
    swcArray *array = (swcArray*)nameArray->pointer;
    
    int32_t sizeAr = (int32_t)array->curSize;

    if(array->curSize == 0)
    {
        array->assigner(data, (void*)array->data);
        array->curSize++;
        return 1;
    }
    uint32_t dataSize = array->dataSize;
    sortFunc sorter = array->sorter;
    char* arrayData = array->data; 
    
    uint32_t i = sizeAr >> 1;
    uint32_t c = i != 0 ? i : 1;
    uint32_t check = 0;
    uint32_t breakUpon = 0;
    while(1)
    {
        breakUpon++;
        if(c != 1)
        {
            c >>= 1;
        }
        if(i != 0)
        {
            if(!sorter(data, (void*)(arrayData + (dataSize * (i - 1)))))
            {
                i -= c;
                continue;
            }
            if(sorter(data, (void*)(arrayData + (dataSize * (i - 1)))) == 1)
            {
                return 1;
            }
        }
        if(i <= sizeAr - 1)
        {
            if(!sorter((void*)(arrayData + dataSize * i), data))
            {
                i += c;
                continue;
            }
            if(sorter((void*)(arrayData + dataSize * i),data) == 1)
            {
                return 1;
            }
        }
        break;
    }


    if(sizeAr == (((uint32_t)(nameArray->size)) / (uint32_t)sizeof(uint32_t)) - 1)
    {
        //need to expand namearray
        if(!reallocNamed(nameArray->name, (uint32_t)((_Float32)nameArray->size * (_Float32)nameArrayReallocSize), manager, nameArray))
        {
            //TODO: handle memory overload.... explode for now
            return 0;
        }
    }
    array->curSize++;
    while(sizeAr > (int64_t)i)
    {
        array->assigner((void*)(arrayData + ((sizeAr - 1) * dataSize)), (void*)(arrayData + (sizeAr * dataSize)));
        sizeAr--;
    }
    array->assigner(data,(void*)(arrayData + i * dataSize));
    return 1;
}

// /**
//  * @brief 
//  * 
//  * @param array NameArray
//  * @param name Name to Remove from NameArray
//  * @param manager 
//  * @return 0 if Failed | 1 if Success (Failure Can Occur due to NameArray Not Existing and Name Not Existing Within Name Array)
//  */
// uint32_t removeNameArray(swcNameArray array, uint32_t name, swcMemMan* manager)
// {
//     swcName* nameArray = retrieveName(array, manager);
//     if(nameArray == NULL)
//     {
//         return 0;
//     }
//     int32_t sizeAr = *(uint32_t*)nameArray->pointer;
    
//     uint32_t *names = (uint32_t*)((uintptr_t)nameArray->pointer + sizeof(uint32_t));
//     uint32_t i = sizeAr >> 1;
//     uint32_t c = i;
//     while(1)
//     {
//         if(c != 1)
//         {
//             c >>= 1;
//         }
//         if(i != 0)
//         {
//             if(names[i - 1] > name)
//             {
//                 i -= c;
//                 continue;
//             }
//             if(names[i - 1] == name)
//             {
//                 i--;
//                 break;
//             }
//         }
//         if(i <= sizeAr - 1)
//         {
//             if(names[i] < name)
//             {
//                 i += c;
//                 continue;
//             }
//             if(names[i] == name)
//             {
//                 break;
//             }
//         }
//         return 0;
//     }
//     sizeAr--;
//     while(i < sizeAr)
//     {
//         names[i] = names[i + 1];
//         i++;
//     }
//     names[sizeAr] = 0;
//     *(--names) = sizeAr;
//     return 1;
// }


/**
 * @brief Retrieves named pointer, if name is deallocated or does not exist
 * returns nullptr
 * 
 * @param name 
 * @return void* 
 */
void* retrieveName(uint32_t name, swcMemMan* manager)
{
    swcName *root = binSearch(name, manager);
    if(root == NULL)
    {
        return NULL;
    }
    return root->pointer;
}

swcName* retrieveNameL(uint32_t name, swcMemMan* manager)
{
    return binSearch(name, manager);
}

/**
 * @brief frees a manager passed as a pointer
 * 
 * @param manager 
 * @return uint32_t 
 */
uint32_t freeMemMan(swcMemMan* manager)
{
    for(int i = manager->count - 1; i >= 0; i--)
    {
        freeArena(&manager->arenas[i]);
    }
    freeArena(&manager->doubleBuffer1);
    freeArena(&manager->doubleBuffer2);
    freeArena(&manager->singleBuffer);
    free(manager->arenas);
    return 1;
}

/**
 * @brief Reorganizes Arenas, returns new "manager"
 * only Named data blocks are copied over to the new manager, all else are scrapped... 
 * passed manager will be made null
 * 
 * @param manager
 * @return uint32_t 
 */
swcMemMan reconfigureArenas(swcMemMan* manager)
{
    uint32_t avgSize = 2000;//=>for initial pointer array fo rbinary tree copying
    uint32_t minSize = 0;
    uint32_t i;
    for(i = 0; i < manager->count; i++)
    {
        avgSize += manager->arenas[i].size; 
        minSize = minSize < manager->arenas[i].minData ? manager->arenas[i].minData : minSize;
    }
    avgSize /= i;
    avgSize = minSize < avgSize ? avgSize : minSize;

    /*
    * This might be the completely incorrect approach, it might be better to just configure all arena
    * data into one massive arena, but thats dependent upon how much deletion of data occurs,
    * or how long term the data is for now ill pretend that this is the faster method
    */

    swcMemMan ret = createMan(manager->size * 1.5, manager->nameCount * 1.5, manager->singleBuffer.size, manager->doubleBuffer1.size);//sure 1.5, hope there is enough discarded data to justify the small increase, hopefully there is a decrease of total data

    swcArena *curArena = addArena(avgSize, minSize, &ret);//this line is a problem i dont know how to fix it
    //TODO: FIX ABOVE LINE(MINSIZE and AVGSIZE conflict, or not really, but it creates a memory size problem where every arena is made for the largest arena)
    //i guess its not really a problem now that I think of it... but whatever

    swcName* root = manager->namesTree;
    swcName* parent = ret.namesTree;
    swcName* lChilds = allocM(sizeof(lChilds) * 24, &ret);//only need 16 lchilds to fully copy 2^32
    uint32_t lChildsCount = 0;
    if(root->lChild != NULL)
    {
        *lChilds = *root->lChild;
        lChildsCount++;
    }
    root = root->rChild;
    while(1)
    {
        //actually change storage here
        if(root->size != 0)
        {
            swcName* curName = (swcName*)alloc(manager->arenas, sizeof(swcName));
            curName->name = root->name;
            curName->size = root->size;
            curName->par = parent;

            curName->pointer = allocM(curName->size, &ret);
            if(curName->pointer == NULL)
            {
                if(addArena(avgSize, minSize, &ret) == NULL)
                {
                    //TODO: explode, just probably reiterate reorganize request with some sort of scalar to make sure the arenas will cover the size
                }
                curName->pointer = allocM(curName->size, &ret);
                if(curName->pointer == NULL)
                {
                    //TODO: this shouldnt occur, if it does ive mismanaged the memory space
                }

            }
            memcpy((char*)curName->pointer, root->pointer, curName->size);

            if(parent->name > curName->name)
            {
                parent->lChild = curName;
            }
            else
            {
                parent->rChild = curName;
            }
            parent = curName;
            ret.nameCount++;
        }
        if(root->rChild != NULL)
        {
            lChildsCount++;
            *(lChilds + lChildsCount) = *root->lChild;//check in lchildsloop to see if its null

            root = root->rChild;
            continue;
        }
        if(lChildsCount >= 0)
        {
            if((lChilds + lChildsCount) != NULL)
            {
                root = (lChilds + lChildsCount);
                lChildsCount--;
            }
            continue;
        }
        break;
    }
    freeMemMan(manager);
    return ret;
}

/**
 * @brief Allocates memory of size under managers single buffer, returns NULL if no space
 * 
 * @param size 
 * @param manager 
 * @return uint32_t 
 */
void* allocSB(size_t size, swcMemMan* manager)
{

    return alloc(&manager->singleBuffer, size);
}

/**
 * @brief Allocates memory of size under managers double buffer, returns NULL if no space
 * 
 * @param size 
 * @param manager 
 * @return uint32_t 
 */
void* allocDB(size_t size, swcMemMan* manager)
{
    return alloc(manager->curDB, size);
}

/**
 * @brief Resets the Single Buffer and Moves the Double Buffer, All Data Stored in the Single Buffer Is Corrupt
 * All Data stroed in the Double Buffer From Two Frames Ago Is Corrupt
 * @param swcMemMan* Manager
 * 
 */
uint32_t frameChange(swcMemMan* manager)
{
    //TODO: Maybe add a way to allocate more memory if needed, I have no clue how to detect something like that
    manager->singleBuffer.beg = manager->singleBuffer.origin;
    if(manager->curDB == &manager->doubleBuffer1)
    {
        manager->curDB = &manager->doubleBuffer2;
    }
    else
    {
        manager->curDB = &manager->doubleBuffer1;
    }
    manager->curDB->beg = manager->curDB->origin;
}