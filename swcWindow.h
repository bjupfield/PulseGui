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

struct swcWinS {
    Window mainWin;
};

typedef struct swcWinS swcWin;


swcWin initWindow(uint32_t* config, uint64_t eventMask, uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy);
// int adjustWinSize();
// int addDiv();
// vertexBI drawWindow();

