#ifndef SWCRENDER_H
#define SWCRENDER_H

#include "swcDef.h"
#include "swcMem.h"
#include "GLfuncs/glCreate.h"

uint32_t renderMain(swcWin* win);
uint32_t preRender(swcWin* win);
uint32_t updateRenderBuffer(swcWin* win ,uint32_t vertexBufferObjectName, uint32_t gpuBufferDataSize, uint32_t cpuBufferDataSize, uint32_t programName, uint32_t layer, void* cpuSideBufferObjectData);
uint32_t initGPUMemory(swcWin *win);
uint32_t gpuAlloc(swcWin *win, uint32_t size);
uint32_t gpuRealloc(swcWin* win, divGroupGpu* divGroupGpu, uint32_t newSize);
uint32_t gpuFree(swcWin* win, divGroupGpu* divGroupGpu);
#endif