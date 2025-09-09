#ifndef SWCRENDER_H
#define SWCRENDER_H

#include "swcDef.h"
#include "swcMem.h"

uint32_t renderMain(swcWin* win);
uint32_t preRender(swcWin* win);
uint32_t updateRenderBuffer(swcWin* win ,uint32_t vertexBufferObjectName, uint32_t gpuBufferDataSize, uint32_t cpuBufferDataSize, uint32_t programName, uint32_t layer, void* cpuSideBufferObjectData);
#endif