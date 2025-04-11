#ifndef GLINTERFACE_H
#define GLINTERFACE_H

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLBindings.h"

/*
* The Near and far plane will be the z dimensions that 
* define the different layers that the "divs" can be
* displayed in
*/
#define farClip = 10
#define nearClip = 0 



/*
* Checks if Server Has OpenGl Available
*/
int queryServer(Display* display);

int retrieveConfig(Display* display);

XVisualInfo *retrieveVisual(Display* display);

int initializeWindow(Display* display, Window window);

int destroyGLX(Display* display);

int createAttachProgram(Display* display, XWindowAttributes attributes);

int adjustViewport(Display* display, int width, int height);

#endif