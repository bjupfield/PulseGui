#ifndef SWCDEF_H
#define SWCDEF_H

#include <X11/Xlib.h>
#include <stdint.h>
#include <string.h>
#include <stdalign.h>
#include "../glInterface/GLInterface.h"

static uint32_t defConfiguration[] =
{
    GLX_DOUBLEBUFFER, True,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_BUFFER_SIZE, 32,
    GLX_DEPTH_SIZE, 16,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_X_RENDERABLE, True,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_CONFIG_CAVEAT, GLX_NONE,
    GLX_ACCUM_RED_SIZE, 1,
    GLX_ACCUM_GREEN_SIZE, 1,
    GLX_ACCUM_BLUE_SIZE, 1,

    None    
};

static uint64_t defMask = ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|
    EnterWindowMask|LeaveWindowMask|ExposureMask|PointerMotionMask|
    VisibilityChangeMask|StructureNotifyMask;
/*
* Arena Definition
* This arena is specifically designed for divs
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

/**
 * @brief Name Pointer Size contianer
 * 
 */
typedef struct swcName {
    uint32_t name;
    void* pointer;
    size_t size;
    struct swcName* lChild;
    uint32_t lSize;
    struct swcName* rChild;
    uint32_t rSize;
    struct swcName* par;
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
    //TODO:
    //I was thinkin gof how we want two different group iterators over the divs, one over event funcs and one for draw funcs
    //the name tree needs to be made specifically for this, or something does
}swcMemMan;

//TODO:
// Add single frame and double frame arenas for smaller data dynamic data within the divs


typedef struct {
    Window mainWin;
    uint32_t programGroupCount;
    uint32_t* groupCounts;
    uint32_t eventGroups;//name
    /*
    * Data Structere  Event Group void *: (doesnt really work in a struct)
    * groupsCount = uint32_t (4 Bytes) : the amount of groups there are (event types, amount handled by this window)
    * groupsSize = uint32_t (8ytes) : the size of each name container
    * groupCurCounts c = array of uint64_t count is value of a (groupsCount * 8 Bytes): first 4 bytes assigned to event type second 4 bytes amount of divs currently assigned to each group... includes deallocated windows untill event group sorted
    * eventGroups = array of arrays of uint32_t (groupsCount * groupsSize * 4 * Bytes): grouped names of divs, each group begins at its sizeof(groupsCount) + sizeof(groupsSize) + sizeof(groupsCurCount) + groupPos * groupsSize
    */
    swcMemMan manager;
    Display* dis;

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
typedef struct swcDiv{
    swcWin* win;
    uint32_t name;
    uint32_t parent;
    uint32_t children[512];//making this static for now for an easier memory manager, will change eventually if this project expands
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

#endif