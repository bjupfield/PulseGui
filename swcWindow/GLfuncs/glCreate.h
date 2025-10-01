#ifndef GLCREATE_H
#define GLCREATE_H

#include <stdio.h>
#include <stdint.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "../swcDef.h"
#include "../swcMem.h"
#include "glPointers.h"


uint32_t findCurScreenNumber(Display *dis);

XVisualInfo* retVisualT(Display* display, GLXFBConfig config);

uint64_t glInitWindowT(Display* display, uint32_t* config, swcWin* swc, uint64_t eventMask);

uint32_t retrieveGLFuncs(swcWin *win);

uint32_t createProgram(const char pathName[256], swcWin* win);

uint32_t glGenBuffer(swcWin* win);

uint32_t glInitBuffer(swcWin* win, uint32_t size);

#endif