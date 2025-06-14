#ifndef GLCREATE_H
#define GLCREATE_H

#include <stdio.h>
#include <stdint.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "../swcDef.h"
#include "glPointers.h"

uint32_t findCurScreenNumber(Display *dis);

XVisualInfo* retVisualT(Display* display, uint32_t* config);

uint64_t glInitWindow(Display* display, Window win);

uint32_t retrieveGLFuncs(swcWin *win);

#endif