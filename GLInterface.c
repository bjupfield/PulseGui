#include "GLInterface.h"


int queryServer(Display* display)
{
    int error_Base, event_Base;
    Bool bool = glXQueryExtension(display, &error_Base, &event_Base);

    return bool;
}

int printConfigs(Display* display)
{
    int count;
    int fifty[] = 
    {
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        
        None    
    };
    glXGetFBConfigs(display, 0, &count);
    glXChooseFBConfig(display, 0, fifty, &count);
    return count;
}
