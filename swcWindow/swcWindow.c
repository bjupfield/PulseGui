#include "swcWindow.h"

/**
 * @brief Attempts to create and return window based on passed params, pass null to config for default config and mask
 * 
 * @param config GLX Visual Config To Be Used in Func glXGetConfig, Leave Null To Choose Default
 * @param eventMask X11 Window Event Mask
 * @param posx Left Corner X Screen Position of Window
 * @param posy Left Corner Y Screen Position of Window
 * @param dimx Window Width
 * @param dimy Window Height
 * @return swcWin 
 */
swcWin initWindow(uint32_t* config, uint64_t eventMask, uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy)
{
    swcWin null = {};
    // TODO: 
    //     Retrive Display Based On Mouse Position
    Display* display = XOpenDisplay(NULL);
    // TODO:
    //      Set ErrorHandler
    //      XSetErrorHandler(errorHandler);

    if(!queryServer(display))
    {
        return null;
    }


    XVisualInfo *info = NULL;
    if(!retrieveConfig(display,(config == 0 ? defConfiguration: config)))
    {
        return null;
    }
    info = retrieveVisual(display);

    if(info == NULL)
    {
        return null;
    }

    //No Choice on Background or Colormap
    XSetWindowAttributes wa = {
		.override_redirect = False,
  		//.background_pixmap = ParentRelative,
		.event_mask = (config == 0 ? defMask : eventMask),
		.background_pixel = BlackPixel(display, 0),
        .colormap = XCreateColormap(display, RootWindow(display, 0), info->visual, AllocNone),
	};

    swcWin win = {
        .mainWin = XCreateWindow(
        display, RootWindow(display, 0), 400, 400, 300, 300, 0, 					
        info->depth, CopyFromParent, info->visual, 
		CWBackPixel|CWEventMask|CWColormap|CWOverrideRedirect, &wa),
    };

    XFree(info);

    win.dis = display;

    win.manager = createMan(sizeof(swcDiv) * 5000, 10);

    addArena(sizeof(swcDiv) * 2000, sizeof(swcDiv), &win.manager);

    initDiv(&win, 0, 0, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), NULL);

    //remove dummy
    desWindow(win);

    return win;
}

swcWin desWindow(swcWin win)
{
    freeMemMan(&win.manager);
    XDestroyWindow(win.dis, win.mainWin);
    return win;
}

/**
 * @brief UGGHHHH
 * 
 * @param win 
 * @param parent 
 * @param posx 
 * @param posy 
 * @param dimx 
 * @param dimy 
 * @param onLoad 
 * @param drawFunc 
 * @param resizeFunc 
 * @param eventFunc 
 * @param excData
 * @return Returns Div Name 
 */
uint32_t initDiv(swcWin* win, uint32_t parent, uint32_t posx, uint32_t posy, 
    uint32_t dimx, uint32_t dimy, funcPointer onLoad, funcPointer drawFunc,
    resizePointer resizeFunc, handlePointer eventFunc, size_t size, void* excData)
{
    uint32_t div = allocNamed(size, &win->manager);
    swcDiv* divPoint = (swcDiv*)retrieveName(div, &win->manager);

    if(divPoint == NULL)
    {
        return 0;
    }

    divPoint->dimx = dimx;
    divPoint->dimy = dimy;
    divPoint->posx = posx;
    divPoint->posy = posy;
    divPoint->parent = parent;
    divPoint->onLoad = onLoad;
    divPoint->drawFunc = drawFunc;
    divPoint->resizeFunc = resizeFunc;
    divPoint->eventFunc = eventFunc;
    divPoint->size = size;

    size_t excSize = size - sizeof(swcDiv);
    memcpy((char*)divPoint + sizeof(swcDiv), excData, excSize);

    divC(divPoint, onLoad);

    return div;
    return 0;

}

/**
 * @brief 
 * 
 * @param win 
 * @param div 
 * @return uint32_t || Deleted div name if success if fail 0
 */
uint32_t delDiv(swcWin* win, uint32_t div)
{
    uint32_t success = deallocNamed(div, &win->manager);
    return success;
}

