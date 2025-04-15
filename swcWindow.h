/*
* Naming the Window Program SWC
* Standing for Simple Window Creator
*/
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdint.h>
#include <stdalign.h>
#include <string.h>
#include "GLInterface.h"

#define zLayers 10;
#define divC(div, func) div.func(div.data)

extern uint32_t defConfiguration[];
extern uint64_t defMask;

typedef struct {
    Window mainWin;
    uint32_t programGroupCount;
    uint32_t* groupCounts;
    uint32_t handleGroupCount;

    /*
    * Okay What we have run into is we need mass allocation of dynamic memory
    * the option that im going to take is arena memory allocation, which means
    * that we need to keep track of the memory ourselfs pretty much, and mass allocate
    * an "arena" that is just one giant heap which we than assign our objects specific positions in
    * this means we have to keep track of where they are assigned, and where memory is available
    * to be assigned, pretty much doing what malloc does ourselves, but though it sounds like it
    * doesnt have benefits, it does, its just more effective
    * anyways to tired to implement it tongiht as I just learned about it
    */

}swcWin;

swcWin initWindow(uint32_t* config, uint64_t eventMask, uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy);
// int adjustWinSize();
// int addDiv();
// vertexBI drawWindow();

/*
* Arena Definition
* This arena is specifically designed for divs
* the alignment will be max alignment... just because, why not, whats the worst that could happen, at most we lose 8 bytes 
* per padding, but storing if a div needs more alignment would cost more
* Assigned pointers will be stored at the end with backwards linking, 
* so the amount of space used will equivalent to requestedallocation / minSize * sizeof(swcAssignedPointer)
* and will allow for the freeing of data
*/

typedef struct {
    char* beg;//current next allocation location
    char* end;//current last available memory for normal allocation(also the start of assignedpointers)
    char* origin;//actual pointer to beginning of memory location of arena//  every bit of data stored gains an assignedpointer datapointer
//  which eventually we will program a "garbage collector"
//  which when the arena is full will begin a reallocation process where a new arena is allocated, or several
//  whic are than filled with only assigned pointers, which can be deleted with a dealloc function, dereferncing data
//  so that it will not be saved once the new arena is allocated
    uint32_t size;
    uint32_t minData;
}swcArena;


swcArena creArena(size_t size, size_t minData);
uint32_t desArena(swcArena *arena);
void *alloc(swcArena *a, size_t size);

/**
 * @brief Name Pointer Size contianer
 * 
 */
typedef struct {
    uint32_t name;
    size_t pointer;
    size_t size;
    swcName* lChild;
    uint32_t lSize;
    swcName* rChild;
    uint32_t rSize;
    swcName* par;
}swcName;

/**
 * @brief Memory Manager 
 * 
 */
typedef struct {
    uint32_t count;
    uint32_t size;
    swcArena* arenas;
    uint32_t nameCount;
    uint32_t nameSize;
    swcName* namesTree;
}swcMemMan;

swcMemMan createMan(uint32_t stSize, uint32_t count);
void* allocM(size_t size, swcMemMan* manager);
uint32_t allocNamed(size_t size, swcMemMan *manager);
uint32_t deallocNamed(uint32_t name);
swcArena* addArena(size_t size, size_t minData, swcMemMan *manager);


struct swcDiv;
typedef uint32_t(*funcPointer)(struct swcDiv*);
typedef uint32_t(*resizePointer)(struct swcDiv*, uint32_t x, uint32_t y);
typedef uint32_t(*handlePointer)(struct swcDiv**);

/**
 * @brief hi
 * 
 * @var parent 2
 * 
 */
typedef struct {
    swcWin* win;
    struct swcDiv* parent;
    struct swcDiv* children[200];//making this static for now for an easier memory manager, will change eventually if this project expands
    uint32_t handleGroup;// event func group
    uint32_t posx;
    uint32_t posy;
    uint32_t dimx;
    uint32_t dimy;
    uint32_t programGroup;//these two can probably be the same
    uint32_t programName;
    _Float32* vba;
    funcPointer drawFunc;
    funcPointer onLoad;
    resizePointer resizeFunc;
    handlePointer eventFunc;
    /*
    * so how this works is the window on load of the function will check the eventfunc handle, if its
    * the same as another. if it is it adds it to that other handle group. if not it starts a new handlegroup
    * and than adds the eventfunc to the to a list of funcs that is called in the windows handleevent function
    * (which occurs when it recieves an event from the xserver). it than iterates through these events and throws
    * each event function its event handlegroup, which is like an array pointing to all the divs in that group
    */
    size_t size;
    /*
    * Currently thinking of making an arena allocater, but that necessitates knowing the size of every div
    * because of this the the above variables need to be filled, by the constructor, hmmm
    * at least a default constructor can be provided, that you just need to pass sizeof(yourdiv)
    */
}swcDiv;


