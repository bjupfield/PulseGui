
#include "glCreate.h"

/**
 * @brief Returns Screen Number based of current mouse pointer position, rets default screen number (0)
 * if none are found
 * 
 * @param dis 
 * @return uint32_t 
 */
uint32_t findCurScreenNumber(Display *dis)
{
    uint32_t screenCount = ScreenCount(dis);

    Window root, child;
    uint32_t root_x, root_y, child_x, child_y, mask;
    printf("ScreenCount: %i", screenCount);
    fflush(stdout);

    for(uint32_t i = 0; i < screenCount; i++)
    {
        if(XQueryPointer(dis, RootWindow(dis, i), &root, &child, &root_x, &root_y, &child_x, &child_y, &mask))
        {
            printf("Screen Number is: %i\n", i);
            return i;
        }
    }
    return 0;
}

/**
 * @brief Retrieves Visual, Returns NUll if the server does not exist or the config does not exist, 
 * 
 * If visual exist it sets curconfig
 *  
 * @param display 
 * @param config 
 * @return XVisualInfo* 
 */
XVisualInfo* retVisualT(Display* display, GLXFBConfig config)
{
    uint32_t error_Base, event_Base;
    uint32_t bool = glXQueryExtension(display, &error_Base, &event_Base);

    if(!bool)
    {
        return NULL;
    }

    return glXGetVisualFromFBConfig(display, config);
}
/**
 * @brief Rets Zero if Fail, else returns 1
 * 
 * @param display 
 * @param win 
 * @return uint64_t 
 */
uint64_t glInitWindowT(Display* display, uint32_t* config, swcWin* swc, uint64_t eventMask)
{
    int count;
    GLXFBConfig* configs = glXChooseFBConfig(display, 0, config, &count);//TODO: change this to the correct screen
    if(count == 0)
    {
        printf("No Matching Configs\n");
        return 0;
    }
    XVisualInfo* info = retVisualT(display, configs[0]);

    if(info == NULL)
    {
        //no matching configs...
        return 0;
    }


    XSetWindowAttributes wa = {
		.override_redirect = False,
  		//.background_pixmap = ParentRelative,
		.event_mask = (config == 0 ? defMask : eventMask),
		.background_pixel = BlackPixel(display, 0),
        .colormap = XCreateColormap(display, RootWindow(display, 0), info->visual, AllocNone),
	};

    swc->mainWin = XCreateWindow(
        display, RootWindow(display, 0), 400, 400, 300, 300, 0, 					
        info->depth, CopyFromParent, info->visual, 
        CWBackPixel|CWEventMask|CWColormap|CWOverrideRedirect, &wa);

    swc->glWindow = glXCreateWindow(display, configs[0], swc->mainWin, NULL);
    swc->glContext = glXCreateNewContext(display, configs[0],  GLX_RGBA_TYPE, NULL, True);
    retrieveGLFuncs(swc);

    XFree(configs);

    return 1;

}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
//this is the first time I've actually used a macro
//sadly cant make the pragma target the macro... very undestandable
#define procMacro(function, name, reference) \
    reference.function = glXGetProcAddressARB((const GLubyte*)name); \
    if(function == NULL) \
    { \
        printf("Failed to load %s", name); \
        return 0; \
    }
uint32_t retrieveGLFuncs(swcWin *win)
{
    //suffer
    procMacro(sigBindBuffer, "glBindBuffer", win->glPointers);
    procMacro(sigCreateBuffers, "glCreateBuffers", win->glPointers);
    procMacro(sigBufferData, "glBufferData", win->glPointers);
    procMacro(sigBufferStorage, "glBufferStorage", win->glPointers);
    procMacro(sigBufferSubData, "glBufferSubData", win->glPointers);
    procMacro(sigDeleteBuffers, "glDeleteBuffers", win->glPointers);

    //shader
    procMacro(sigCreateShader, "glCreateShader", win->glPointers);
    procMacro(sigShaderSource, "glShaderSource", win->glPointers);
    procMacro(sigCompileShader, "glCompileShader", win->glPointers);
    procMacro(sigAttachShader, "glAttachShader", win->glPointers);
    procMacro(sigDeleteShader, "glDeleteShader", win->glPointers);
    procMacro(sigReleaseShaderCompiler, "glReleaseShaderCompiler", win->glPointers);
    procMacro(sigGetShaderiv, "glGetShaderiv", win->glPointers);
    procMacro(sigGetShaderInfoLog, "glGetShaderInfoLog", win->glPointers);

    //fragshader
    procMacro(sigBindFragDataLocation, "glBindFragDataLocation", win->glPointers);

    //vao
    procMacro(sigCreateVertexArrays, "glCreateVertexArrays", win->glPointers);
    procMacro(sigBindVertexArray, "glBindVertexArray", win->glPointers);
    procMacro(sigVertexAttribPointer, "glVertexAttribPointer", win->glPointers);
    procMacro(sigEnableVertexAttribArray, "glEnableVertexAttribArray", win->glPointers);
    procMacro(sigDisableVertexAttribArray, "glDisableVertexAttribArray", win->glPointers);

    //program
    procMacro(sigCreateProgram, "glCreateProgram", win->glPointers);
    procMacro(sigLinkProgram, "glLinkProgram", win->glPointers);
    procMacro(sigUseProgram, "glUseProgram", win->glPointers);
    procMacro(sigDeleteProgram, "glDeleteProgram", win->glPointers);
    procMacro(sigGetProgramiv, "glGetProgramiv", win->glPointers);
    procMacro(sigGetProgramInfoLog, "glGetProgramInfoLog", win->glPointers);

    //uniforms
    procMacro(sigProgramUniform1f, "glProgramUniform1f", win->glPointers);
    procMacro(sigProgramUniform2f, "glProgramUniform2f", win->glPointers);
    procMacro(sigProgramUniform3f, "glProgramUniform3f", win->glPointers);
    procMacro(sigProgramUniform4f, "glProgramUniform4f", win->glPointers);

    procMacro(sigProgramUniform1i, "glProgramUniform1i", win->glPointers);
    procMacro(sigProgramUniform2i, "glProgramUniform2i", win->glPointers);
    procMacro(sigProgramUniform3i, "glProgramUniform3i", win->glPointers);
    procMacro(sigProgramUniform4i, "glProgramUniform4i", win->glPointers);

    procMacro(sigProgramUniform1ui, "glProgramUniform1ui", win->glPointers);
    procMacro(sigProgramUniform2ui, "glProgramUniform2ui", win->glPointers);
    procMacro(sigProgramUniform3ui, "glProgramUniform3ui", win->glPointers);
    procMacro(sigProgramUniform4ui, "glProgramUniform4ui", win->glPointers);

    procMacro(sigProgramUniform1fv, "glProgramUniform1fv", win->glPointers);
    procMacro(sigProgramUniform2fv, "glProgramUniform2fv", win->glPointers);
    procMacro(sigProgramUniform3fv, "glProgramUniform3fv", win->glPointers);
    procMacro(sigProgramUniform4fv, "glProgramUniform4fv", win->glPointers);

    procMacro(sigProgramUniform1iv, "glProgramUniform1iv", win->glPointers);
    procMacro(sigProgramUniform2iv, "glProgramUniform2iv", win->glPointers);
    procMacro(sigProgramUniform3iv, "glProgramUniform3iv", win->glPointers);
    procMacro(sigProgramUniform4iv, "glProgramUniform4iv", win->glPointers);

    procMacro(sigProgramUniform1uiv, "glProgramUniform1uiv", win->glPointers);
    procMacro(sigProgramUniform2uiv, "glProgramUniform2uiv", win->glPointers);
    procMacro(sigProgramUniform3uiv, "glProgramUniform3uiv", win->glPointers);
    procMacro(sigProgramUniform4uiv, "glProgramUniform4uiv", win->glPointers);

    procMacro(sigProgramUniformMatrix2fv,   "glProgramUniformMatrix2fv", win->glPointers);
    procMacro(sigProgramUniformMatrix3fv,   "glProgramUniformMatrix3fv", win->glPointers);
    procMacro(sigProgramUniformMatrix4fv,   "glProgramUniformMatrix4fv", win->glPointers);
    procMacro(sigProgramUniformMatrix2x3fv, "glProgramUniformMatrix2x3fv", win->glPointers);
    procMacro(sigProgramUniformMatrix3x2fv, "glProgramUniformMatrix3x2fv", win->glPointers);
    procMacro(sigProgramUniformMatrix2x4fv, "glProgramUniformMatrix2x4fv", win->glPointers);
    procMacro(sigProgramUniformMatrix4x2fv, "glProgramUniformMatrix4x2fv", win->glPointers);
    procMacro(sigProgramUniformMatrix3x4fv, "glProgramUniformMatrix3x4fv", win->glPointers);
    procMacro(sigProgramUniformMatrix4x3fv, "glProgramUniformMatrix4x3fv", win->glPointers);

    //querry
    procMacro(sigGetIntegerv, "glGetIntegerv", win->glPointers);
}
#pragma GCC diagnostic pop