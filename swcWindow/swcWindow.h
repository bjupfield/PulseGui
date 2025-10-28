/*
* Naming the Window Program SWC
* Standing for Simple Window Creator
*/
#ifndef SWCWINDOW_H
#define SWCWINDOW_H

#include "swcDef.h"
#include "swcMem.h"
#include "swcBaseDivs.h"
#include "swcDiv.h"
#include "swcRender.h"
#include "GLfuncs/glCreate.h"

#define zLayers 10;

//TODO: 
// DO SOMETHING ABOUT NO RETURN, I DON'T KNOW HOW TO HANDLE THIS
//^
/**
 * @brief 
 * 
 * @param config 
 * @param eventMask 
 * @param posx 
 * @param posy 
 * @param dimx 
 * @param dimy 
 * @param layerCount Leave 0 for default case
 * @return swcWin 
 */
swcWin initWindow(uint32_t* config, uint64_t eventMask, uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy, uint32_t layerCount);
// int adjustWinSize();
// int addDiv();
// vertexBI drawWindow();
uint32_t desWindow(swcWin* swcWin);

uint32_t addToProgram(uint32_t initialDivEstimate, swcName divName, uint32_t layer, uint32_t vertexPerDiv, uint32_t renderType, const char pathName[256], swcWin* win);

uint32_t removeFromProgram(swcName divName, uint32_t programName, uint32_t layer, swcWin* win);

uint32_t addToEvents(swcName divName, uint32_t eventMask, uintptr_t func, swcWin* win);

uint32_t removeFromEvents(swcName divName, uint32_t eventMask, uintptr_t func, swcWin* win);

#endif