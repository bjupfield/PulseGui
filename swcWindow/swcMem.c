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
    //TODO: add decoupling from garbage collector
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
        //TODO: handle this
        //needs like an outside func that catches a null pointer and than allocates a new arena to hold more data
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
 * @brief Create a memory manager with initial size of stSize
 * 
 * @param stSize Desired Arena Count
 * @param count Names Initialization
 * @return swcMemMan 
 */
swcMemMan createMan(uint32_t stSize, uint32_t count)
{
    swcMemMan ret = 
    {
        .count = 0,
        .size = stSize + 1,
        .arenas = (swcArena*)calloc(stSize + 1, sizeof(swcArena)),
        .nameSize = count,
        .nameCount = 1,//tree start
    };
    swcArena *a = addArena((sizeof(swcName) + (-sizeof(swcName) & alignof(swcName))) * count, sizeof(swcName), &ret);
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
        //TODO:
        //this either means a fake name is given or it has failed
        return 0;
    }
    root->pointer = 0;
    return name;
}

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

/**
 * @brief frees a manager passed as a pointer
 * 
 * @param manager 
 * @return uint32_t 
 */
uint32_t freeMemMan(swcMemMan* manager)
{
    for(int i = 0; i < manager->count; i++)
    {
        freeArena(&manager->arenas[i]);
    }
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

    swcMemMan ret = createMan(manager->size * 1.5, manager->nameCount * 1.5);//sure 1.5, hope there is enough discarded data to justify the small increase, hopefully there is a decrease of total data

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