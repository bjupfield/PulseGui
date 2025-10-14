#ifndef SWCRENDER_H
#define SWCRENDER_H

#include "swcDef.h"
#include "swcMem.h"
#include "GLfuncs/glCreate.h"

uint32_t renderMain(swcWin* win);
uint32_t preRender(swcWin* win);
void *updateRenderBuffer(uint32_t layer, swcName div, uint32_t programName, swcWin* win);
uint32_t initGPUMemory(swcWin *win);
uint32_t gpuAlloc(uint32_t count, uint32_t stride, swcWin *win);
uint32_t gpuRealloc(divGroupGpu* divGroupGpu, uint32_t count, swcWin* win);
uint32_t gpuFree(divGroupGpu* divGroupGpu, swcWin *win);
#endif