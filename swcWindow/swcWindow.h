/*
* Naming the Window Program SWC
* Standing for Simple Window Creator
*/
#ifndef SWCWINDOW_H
#define SWCWINDOW_H

#include "swcDef.h"
#include "swcMem.h"
#include "swcBaseDivs.h"
#include "GLfuncs/glCreate.h"

#define zLayers 10;
#define divC(div, func) div->func(div)

//TODO: 
// DO SOMETHING ABOUT NO RETURN, I DON'T KNOW HOW TO HANDLE THIS
//^
swcWin initWindow(uint32_t* config, uint64_t eventMask, uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy);
// int adjustWinSize();
// int addDiv();
// vertexBI drawWindow();
uint32_t desWindow(swcWin* swcWin);

uint32_t initDiv(swcWin* win, uint32_t parent, uint32_t posx, uint32_t posy, 
    uint32_t dimx, uint32_t dimy, funcPointer onLoad, funcPointer drawFunc,
    resizePointer resizeFunc, handlePointer eventFunc, size_t size, 
    uint32_t eventTypeMask, void* excData);
uint32_t delDiv(swcWin* win, uint32_t div);

#endif