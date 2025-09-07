#ifndef SWCDIV_H
#define SWCDIV_H

#include "swcDef.h"
#include "swcMem.h"
#include "GLfuncs/glCreate.h"

uint32_t initDiv(swcWin* win, uint32_t parent, 
    uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy, uint32_t layer,  
    funcPointer onLoad, funcPointer drawFunc, funcPointer deleteFunc,
    resizePointer resizeFunc, handlePointer eventFunc, size_t size,
    uint32_t eventTypeMask, const char pathName[256], void* excData);
uint32_t delDiv(swcWin* win, swcName divName);
#endif