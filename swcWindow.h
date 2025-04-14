/*
* Naming the Window Program SWC
* Standing for Simple Window Creator
*/
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdint.h>
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


typedef uint32_t(*funcPointer)(swcDiv*);
typedef uint32_t(*resizePointer)(swcDiv*, uint32_t x, uint32_t y);
typedef uint32_t(*handlePointer)(swcDiv**);
/**
 * @brief hi
 * 
 * @var parent 2
 * 
 */
typedef struct {
    swcWin* win;
    swcDiv* parent;
    uint32_t handleGroup;
    uint32_t posx;
    uint32_t posy;
    uint32_t dimx;
    uint32_t dimy;
    uint32_t programGroup;//these two can probably be the same
    uint32_t programName;
    _Float32 vba;
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
}swcDiv;

swcWin initWindow(uint32_t* config, uint64_t eventMask, uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy);
// int adjustWinSize();
// int addDiv();
// vertexBI drawWindow();

