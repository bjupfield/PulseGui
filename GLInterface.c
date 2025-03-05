#include "GLInterface.h"

GLXFBConfig config;
GLXWindow mainWindow;
GLXContext context;

/*
*   Returns True if the X Server has the Glx extension
*/
int queryServer(Display* display)
{
    int error_Base, event_Base;
    Bool bool = glXQueryExtension(display, &error_Base, &event_Base);

    return bool;
}
/*
*   Returns True if a Config was retrieved and Saved that Matches the Specifications
*/
int retrieveConfig(Display* display)
{
    int count;
    int fifty[] = 
    {
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_BUFFER_SIZE, 32,
        GLX_DEPTH_SIZE, 16,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_X_RENDERABLE, True,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_CONFIG_CAVEAT, GLX_NONE,
        GLX_ACCUM_RED_SIZE, 1,
        GLX_ACCUM_GREEN_SIZE, 1,
        GLX_ACCUM_BLUE_SIZE, 1,
        
        None    
    };
    GLXFBConfig* configs = glXChooseFBConfig(display, 0, fifty, &count);
    if(count > 0)
    {
        
        config = configs[0];
        return count;
    }
    return False;
}

/*
*   Returns Null if No Visual can be Associated, retrieve config must be run before this function
*/
XVisualInfo *retrieveVisual(Display* display)
{
    return glXGetVisualFromFBConfig(display, config);
}
/*
*   Returns True if Initilization Was a Success
*/
int initializeWindow(Display* display, Window window)
{
    mainWindow = glXCreateWindow(display, config, window, NULL);
    context = glXCreateNewContext(display, config,  GLX_RGBA_TYPE, NULL, True);


    /*
    * TODO:
    * I don't really know how to check if it was able to create the window, we would need to somehow 
    * check if the errorhandler was triggered and had this error inside it? I think? Don't want to do it now,
    * would rather work on actual rendering
    */

    return 1;
}
/*
* Destroy all objects associated with OpenGl
*/
int destroyGLX(Display* display)
{
    glXDestroyWindow(display, mainWindow);
    glXDestroyContext(display, context);
}
/*
* Create and Release Current Context Test
* 
*/
int currenting(Display* display)
{

    glXMakeCurrent(display, mainWindow, context);

    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f,  1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glEnd();

    glXSwapBuffers(display, mainWindow);
    // glFinish();


    // /*
    // * This is how the Specification on pg 41 recommends releasing the context
    // * Only applies if releasing it fully, that is there is no other context to draw in
    // */
    glXMakeCurrent(display, None, NULL);

    return 0;
}