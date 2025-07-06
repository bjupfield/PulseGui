#ifndef SWCDEF_H
#define SWCDEF_H

#include <X11/Xlib.h>
#include <stdint.h>
#include <string.h>
#include <stdalign.h>
#include "../glInterface/GLInterface.h"
#include "GLfuncs/glPointers.h"

#define InitialHandleToDivCount 40

#define InitialProgramSize 40
#define InitialProgramToDivSize 80

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
    swcArena singleBuffer; /* Arena for single frame storage */
    swcArena doubleBuffer1; /* Don't Know if I'll use double buffers */
    swcArena doubleBuffer2;
    swcArena* curDB;
    //TODO:
    //I was thinkin gof how we want two different group iterators over the divs, one over event funcs and one for draw funcs
    //the name tree needs to be made specifically for this, or something does
}swcMemMan;

/**
 * @brief This is an array, classic java style, data is stored in various
 * locations because of the less array style of a name, refer to swcArray for
 * how data is stored
 */
typedef uint32_t swcArrayName;
//im kind of sick and twisted, bald man

/**
 * @brief Return 0 if Left is Less Than Right, 1 if Equal, 2 if More Than
 */
typedef uint32_t(*sortFunc)(void*, void*);

/**
 * @brief Assign Left Value Into Right Value...
 */
typedef void(*assigner)(void*, void*);

/**
 * @brief currently completely theoretical, I wont actually be using this, but
 * all this information will be contained in an array, just it will be accessed, actually
 * what am i talking about, I will use this structure to access the data
 * 
 * No Duplicates are ALLOWED
 */
typedef struct
{
    uint32_t curSize;
    char data[];
}swcArray;

/**
 * @brief This is a container that holds the association between X window events and swcWindow event handlers. an X window event can have
 * handleToEventCount handles associated with it. The Handle container is of size ((4 + handletoevent * 4) * eventGroup count). The first
 * 4 bytes of each region is the event/mask, the rest of the bytes are the name of the event that are associated with said event/mask,
 * with each name pointing to a eventHandle struct 
 * 
 * THIS IS NOW A THEORETICAL STRUCT
 * 
 * Because function pointers are what we are using to assign to handles we are going to include them in our handlecontainer, the first sizeof(uint32_t)
 * bytes being the event/mask of the handle container the second
 * sizeof(uint32_t) bytes being the function pointer and the the next sizeof(uint32_t) bytes of handlecontainer being the name... 
 * because of possible alignment problems the data will now not
 * be handled in a struct
 */
struct funcHandleArrays{
    uintptr_t func;
    uint32_t divsName;
    //TODO: add func that deletes div names in this container
    //TODO: make divsName a swcNameArray
};
/**
 * @brief Holds events, honestly i don't really know what this is doing to be honest
 * 
 */
typedef struct {//TODO: change evnt group to use swcArray
    uint32_t eventGroupCount;
    uint32_t handleToEventCount;
    uint32_t events[24];//max of 24 eventtypes
    struct funcHandleArrays funcHandles[];
}evntGroup;


/**
 * @brief Container defining the programs linked and the divs linked to them, is used, element pathname is used to not redeclare the same program, and to assign 
 * 
 */

typedef struct {
    char pathName[256];
    uint32_t programName;
}programNames;

typedef struct {
    uint32_t programName;
    swcArrayName divs;
}nameToDiv;

/**
 * @brief This is the Window...
 * 
 */
typedef struct {
    Display* dis;
    Window mainWin;
    uint32_t event_mask;
    uint32_t eventGroups;//name for eventGroup struct
    swcMemMan* manager;
    glFuncPointers glPointers;
    GLXContext glContext;
    GLXWindow glWindow;
    swcArrayName glProgramNames;//array of program names
    swcArrayName glNamesToDivs;//association of program names to divs
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
typedef uint32_t(*handlePointer)(struct swcDiv**, XEvent* event);

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
    //TODO: change this
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