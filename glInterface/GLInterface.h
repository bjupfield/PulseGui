#ifndef GLINTERFACE_H
#define GLINTERFACE_H

#include <GL/glx.h>
#include <stdarg.h>//not to sure about this macro, might attempt to solve the variadic function without use of it...
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
#define farClip 10
#define nearClip 0 

typedef struct scissorR {
    uint32_t xL, xR, yT, yB;
} scissorR;

enum uniformType 
{
    _1f,
    _1i,
    _1ui,
    _2f,
    _2i,
    _2ui,
    _3f,
    _3i,
    _3ui,
    _4f,
    _4i,
    _4ui,
    _1fv,
    _2fv,
    _3fv,
    _4fv,
    _1iv,
    _2iv,
    _3iv,
    _4iv,
    _1uiv,
    _2uiv,
    _3uiv,
    _4uiv,
    _x2fv,
    _x3fv,
    _x4fv,
    _2x4fv,
    _3x4fv,
    _3x2fv,
    _4x2fv,
    _4x3fv
};
typedef struct GLIUniforms
{
    uint32_t location;
    enum uniformType type;
    char name[64];
}GLIUniforms;

typedef struct
{
    uint32_t program;
    GLIUniforms uniforms[128];
}GLIProgram;

typedef struct GLIState {
    uint32_t program;/* Current Program */
    uint32_t pCount;/* Current Program Count */
    uint32_t pTotal;/* Total Possible Programs */
    scissorR region;/* Scissor Window X*/
    Window window;/* Current Window */
    void* managedMemory;/* This Contains the Cache for Compiled Shaders and a list of GLIPorgrams 
    * The GLIPrograms are first in memory and are continuous in memory, but the "cache" for shaders is created and
    * destroyed by the batch_Create_Shader, and end_Batch_Create_Shader
    */

} GLIState;
/**
 * @brief This Function Would Alert GLIState that you are creating a large amount of programs, and thus allows it to save shaders, to prevent repeat compilation of shaders
 * that are used in multiple pipelines
 * 
 * @return uint32_t 
 */
uint32_t batch_Create_Shader();

/**
 * @brief Would Alert GLIState that the batch creation is finished, it would clear the shader cache and "delete" the shaders.
 * 
 * @return uint32_t 
 */
uint32_t end_Batch_Create_shader();

uint32_t update_Uniform(GLIProgram* program, const char* name, ...);

/*
* Checks if Server Has OpenGl Available
*/
int queryServer(Display* display);

int retrieveConfig(Display* display, uint32_t* confg);

XVisualInfo *retrieveVisual(Display* display);

int initializeWindow(Display* display, Window window);

int destroyGLX(Display* display);

int createAttachProgram(Display* display, XWindowAttributes attributes);

int adjustViewport(Display* display, int width, int height);

XVisualInfo* retVisual(Display* display, uint32_t* config);

uint64_t glInitWindow(Display* display, Window win);//hmmm

#endif