#ifndef SWCDEF_H
#define SWCDEF_H

#include <X11/Xlib.h>
#include <stdint.h>
#include <string.h>
#include <stdalign.h>
#include "../glInterface/GLInterface.h"
#include "GLfuncs/glPointers.h"

#define InitialEventToHandleSize 20
#define InitialHandleToDivSize 80

#define InitialProgramSize 40
#define InitialProgramToDivSize 80

#define InitialDivCount 512

#define DefaultLayerCount 32

#define MinBufferDataChangedSize 16

//GpuStorageDefinitions
#define AdditionalGpuMem 0.4f
#define MaxAdditionalGpuMem 20000
#define InitialGpuMem 10000

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
typedef struct swcNameStruct {
    uint32_t name;
    void* pointer;
    size_t size;
    struct swcNameStruct* lChild;
    uint32_t lSize;
    struct swcNameStruct* rChild;
    uint32_t rSize;
    struct swcNameStruct* par;
}swcNameStruct;

/**
 * @brief Like the name to give back to any program requesting a data block, and that which is used to retrieved previously allocated data blocks
 * 
 */
typedef uint32_t swcName;


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
    swcNameStruct* namesTree;
    swcArena singleBuffer; /* Arena for single frame storage */
    swcArena doubleBuffer1; /* Don't Know if I'll use double buffers */
    swcArena doubleBuffer2;
    swcArena* curDB;
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

struct swcDiv;

typedef uint32_t(*funcPointer)(struct swcDiv*);
typedef uint32_t(*resizePointer)(struct swcDiv*, uint32_t x, uint32_t y);
typedef uint32_t(*handlePointer)(uint32_t* divs, uint32_t divsSize, XEvent* event);

typedef struct {
    handlePointer func;
    swcArrayName divsName;
}funcHandleArrays;
/**
 * @brief Holds events, honestly i don't really know what this is doing to be honest
 * 
 */
typedef struct {//TODO: change evnt group to use swcArray
    uint32_t eventGroupCount;
    uint32_t events[24];//max of 24 eventtypes
    swcArrayName funcGroup[];//array to array of funcs, every event has its own funcgroup array
}evntGroup;


// eventToFunc.FuncHandle.div

/**
 * @brief Container defining the programs linked and the divs linked to them, is used, element pathname is used to not redeclare the same program, and to assign 
 * 
 */

typedef struct {
    char pathName[256];
    uint32_t programName;
}programNames;

typedef struct {
    uint32_t layer;
    swcArrayName divGroups;
}layerToDivGroups;

/**
 * @brief Probably thought about this for a little too much, ut I have decided that for updating and configuring
 * data within the gpu instead of having individual divs update the data, which sounds extremely ineffecient, unless opengl uses a large
 * buffer for updating these objects... but anyway im pretty sure it doesnt, so when the div updates its graphics or whatever it will
 * change whatever its data is in the cpuSideBufferObjectDat, and it will notify the window.render.bufferDataChanged that the data has changed
 * */
typedef struct {
    uint32_t programName;
    uint32_t gpuBufferDataLocation;//"index" for position in gpu data block, to be used with namedbuffersubdata
    uint32_t gpuBufferDataSize;
    uint32_t vertexBufferObjectName;
    uint32_t cpuBufferObjectDataElementSize;
    swcName cpuSideBufferObjectData;
    swcArrayName divs;
}divGroupGpu;
/**
 * @brief this is the data structure that will be held within the render structure, it will be referenced by a div whenver it updates its graphics, in doing so
 * it will both store the vertexbufferobjectsname that it is attached to and pull change the cpusidebufferobjectdata and give a pointer to that data, obviously giving a pointer is a bit
 * risky, but I think it works... as long as we only mass reallocate after any render cycles...
 * 
 */
typedef struct
{
    uint32_t vertexBufferObjectName;
    uint32_t gpuBufferDataSize;
    uint32_t cpuBufferDataSize;
    uint32_t programName;
    uint32_t layer;
    void* cpuSideBufferObjectData;

}bufferDataChanged;

/**
 * @brief 
 * Struct used for organizational purposes within the window struct, basically it will contain all the references that are needed for rendering
 * purposes for the window
 */
struct render
{
    uint32_t bufferDataChangedSize;
    uint32_t bufferDataChangedElementCount;
    bufferDataChanged* bufferDataChanged;//pointer to SB datablock where buffer names that were updated are stored, renewed every frame
};
/**
 * @brief 
 * Struct used to hold all data related to gpuMem
 * size is the size of the gpu in memory
 * emptyMem is a sorted array of the empty memory space, with data stored as 2 uint32_t, with xy struct below, with the first x being
 * the size of the memory space and the second being the position
 */
typedef struct
{
    GLuint bufferName;
    uint32_t size;
    swcArrayName emptyMem;
}glBuffer;
/**
 * @brief This is the Window... Descriptions are below members
 * 
 */
typedef struct {
    swcName name;
    Display* dis;
    Window mainWin;
    uint32_t event_mask;
    uint32_t eventGroups;//name for eventGroup struct
    swcMemMan* manager;
    glFuncPointers glPointers;
    GLXContext glContext;
    GLXWindow glWindow;
    swcArrayName glProgramNames;//array of program names
    swcArrayName divLayers;
    glBuffer glBuffer;
    /*
    * One More Essay Incoming. Okay, we messed up. We thought the voa stored the buffer objects somehow, or no,
    * we forgot to use voa to point towards buffer objects in our buffer objects... so instead we are going to 
    * do the responsible thing and have one massive buffer object that every voa points too, so now we have to do like
    * actual memory management inside the gpu... ugh
    * okay so what we are doing is simple, first this big buffer will be managed in this way:
    *  basically a big array with data points in two integers regarding where the storage stops for a particular voa and
    *  where it ends, okay i guess this means i have to mention that we will still use voas for every single layer->program group
    *  but these voas will all point to this massive single buffer whose memory will be handled by the window as a whole
    *  like we said that memory will be handled with a simple array holding x,y information with the first being the starting position for empty memory space
    *  and the second being the end of that empty memory space, actually for sorting purposes what we will do is create an object like so { a, b}
    *  with a being the amount of memory that is availbale in that space and b being the starting position, so that we can sort and stuff. we will also hold the largest available memory space
    *  size outside so we can say if you need to expand the memory space or not... yeah and than when we run out of memory space we reorganize it...
    * to do this we have to discuss the voa
    *  the voas will use the old structure for the buffers and just hold the voas current gpu memory storage inside it as well as the other memory
    *  than to reorganize the memory structure we will just use this stuff to create a new memory buffer with very simple <3
    */
    struct render *render;
    /*
    * Div Layers are used for rendering purposes, they control the z layer for div
    * The DivLayer Array is a reference to individual layer arrays
    */
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


/**
 * @brief hi
 * 
 * @var parent 2
 * 
 */
typedef struct swcDiv{
    swcWin* win;
    swcName name;
    swcName parent;
    /*
    * Default Parent is the Root Node if No Parent is assigned
    */
    uint32_t eventMask;//EventMask
    uint32_t children[512];//making this static for now for an easier memory manager, will change eventually if this project expands
    //TODO: change this
    uint32_t handleGroup;// event func group
    uint32_t posx;
    uint32_t posy;
    uint32_t dimx;
    uint32_t dimy;
    uint32_t layer;// window layer div is assigned to
    uint32_t programName;
    _Float32* vba;
    funcPointer drawFunc;
    funcPointer onLoad;
    funcPointer deleteFunc;//do not deallocate the div inside this func, used to dereference the div in any structures that might reference to it
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


/**
 * @brief Just to use as an easy structure
 * 
 */
typedef struct xy
{
    uint32_t x;
    uint32_t y;
}xy;

#endif