#ifndef GLINTERFACE_H
#define GLINTERFACE_H

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLBindings.h"


/*
* Checks if Server Has OpenGl Available
*/
int queryServer(Display* display);

int retrieveConfig(Display* display);

XVisualInfo *retrieveVisual(Display* display);

int initializeWindow(Display* display, Window window);

int currenting(Display* display);

int destroyGLX(Display* display);

int createAttachProgram();

#endif