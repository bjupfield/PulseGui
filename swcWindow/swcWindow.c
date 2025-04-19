#include "swcWindow.h"


uint32_t initEventGroups(swcWin* swcWin, uint32_t eventGroups, size_t size);

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
//TODO:
    //MAKE NO RETURN

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

    initEventGroups(&win, eventMask, 800);

    uint32_t divName = initDiv(&win, 0, 24, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), 0, NULL);

    swcDiv *div = retrieveName(divName, &win.manager);

    printf("\n\ndiv.posx: %i\n\n", div->posx);

    //EVENT HANDLER



    //DRAWER


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
 * @brief 
 * 
 * @param swcWin 
 * @param size
 * @return uint32_t 
 */
uint32_t initEventGroups(swcWin* swcWin, uint32_t eventGroups, size_t size)
{
    uint32_t count = 0; 
    for(uint32_t mask = 1; mask < 1 <26; mask <<= 1)//refer to X.h event definitions for the explanations of this bit mask
    {
        if(mask & eventGroups)
        {
            count++;
        }
    }
    
    uint32_t name = allocNamed(sizeof(uint32_t)  * 2 * (1 + count) + size * count, &swcWin->manager);
    uint32_t* eventGroup = (uint32_t*)retrieveName(name, &swcWin->manager);
    
    *eventGroup = count;
    *(eventGroup + 1) = size;
    //um ignore this plz:
    count = 0;
    for(uint32_t mask = 1; mask < 1 <26; mask <<= 1)
    {
        if(mask & eventGroups)
        {
            *(eventGroup + 2 + (count++ * 2)) = mask;
        }
    }
    return name;
}

/**
 * @brief 
 * 
 * @param divName 
 * @param eventMask 
 * @param win 
 * @return uint32_t 
 */
uint32_t addToEvents(uint32_t divName, uint32_t eventMask, swcWin* win)
{
    uint32_t* eventGroups = (uint32_t*)retrieveName(win->eventGroups, &win->manager);
    for(uint32_t i = 0; i < *eventGroups; i++)
    {
        if(eventMask & *(eventGroups + 2 + i * 2))
        {
            uint32_t eventAssignedDivs = *(eventGroups + 3 + i * 2);
            if(eventAssignedDivs > *(eventGroups + 1))
            {
                //TODO: assigned divs have overflowed their container, create a larger one
            }
            *(eventGroups + i * *(eventGroups + 1) + eventAssignedDivs) = divName;//assigns divName to eventgroup that matchs eventmask
        }
    }
}

uint32_t handleEvents(swcWin* win)
{
    uint32_t events = XEventsQueued(win->dis, QueuedAfterFlush);
    for(events; events > 0; events--)
    {
        XEvent* event;
        XNextEvent(win->dis, event);
        event.type;
    }
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
 * @param eventTypeMask Event Mask that Assigns Divs to recieve events from window, event mask is identical to XORG Input Event Mask
 * @param excData
 * @return Returns Div Name 
 */
uint32_t initDiv(swcWin* win, uint32_t parent, uint32_t posx, uint32_t posy, 
    uint32_t dimx, uint32_t dimy, funcPointer onLoad, funcPointer drawFunc,
    resizePointer resizeFunc, handlePointer eventFunc, size_t size,
    uint32_t eventTypeMask, void* excData)
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

    addToEvents(div, eventTypeMask, win);

    divC(divPoint, onLoad);

    return div;

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

uint32_t eventHandler(swcWin* win)
{
    //checks how many events are in queue, if no events are in queue flushes than checks again
    uint32_t eventsQueued = XEventsQueued(win->dis, QueuedAfterFlush);
    
    for(int i = 0; i < eventsQueued; i++)//try not to dequeue events otherwise it blocks
    {
        //TODO: add this to arena, should be in single-buffered memory
        XEvent* event;
        XNextEvent(win->dis, event);
        event;
    }
}


// while(plzDestroy)
// {
//     XNextEvent(display, &event);
//     handleEvent(&event, display, &w);
// }