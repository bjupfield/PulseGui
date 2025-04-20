#include "swcWindow.h"


uint32_t initEventGroups(swcWin* swcWin, uint32_t eventGroups, uint32_t handleToEventCount);

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

    initEventGroups(&win, eventMask, 40);

    uint32_t divName = initDiv(&win, 0, 24, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), PointerMotionMask, NULL);

    swcDiv *div = retrieveName(divName, &win.manager);

    printf("\n\ndiv.posx: %i\n\n", div->posx);

    //EVENT HANDLER



    //DRAWER


    //remove dummy
    desWindow(&win);

    return win;
}

uint32_t desWindow(swcWin* win)
{
    //TODO: make this func?
    freeMemMan(&win->manager);
    XDestroyWindow(win->dis, win->mainWin);
    return 1;
}
/**
 * @brief 
 * 
 * @param swcWin 
 * @param size
 * @return uint32_t 
 */
uint32_t initEventGroups(swcWin* swcWin, uint32_t eventGroups, uint32_t handleToEventCount)
{
    uint32_t count = 0; 
    for(uint32_t mask = 1; mask < 1 <26; mask <<= 1)//refer to X.h event definitions for the explanations of this bit mask
    {
        if(mask & eventGroups)
        {
            count++;
        }
    }
    
    uint32_t name = allocNamed(sizeof(uint32_t)  * 2 + sizeof(uint32_t) * count + (count * handleToEventCount) * (sizeof(uintptr_t) + sizeof(uint32_t)), &swcWin->manager);
    void* eventGroup = retrieveName(name, &swcWin->manager);
    
    *((uint32_t*)(eventGroup)) = count;
    *(((uint32_t*)eventGroup) + 1) = handleToEventCount;
    //um ignore this plz:
    uint8_t bool = 0;
    for(uint32_t mask = 1, count = 0; mask < 1 <26; mask <<= 1)
    {
        if(mask & eventGroups)
        {
            if(mask & ButtonMotionMask|Button1MotionMask|Button2MotionMask|Button3MotionMask|Button4MotionMask|Button5MotionMask|PointerMotionMask)
            {
                if(!bool)
                {
                    
                    *(uint32_t*)((char*)eventGroup + sizeof(uint32_t) * 2 + (sizeof(uint32_t) + (sizeof(uintptr_t) + sizeof(uint32_t) * handleToEventCount)) * count++) = PointerMotionMask;//they all use the same event type, motion notify
                    bool = 1;
                }
                continue;
            }
            *(uint32_t*)((char*)eventGroup + sizeof(uint32_t) * 2 + (sizeof(uint32_t) + (sizeof(uintptr_t) + sizeof(uint32_t) * handleToEventCount)) * count++) = mask;
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
 * @return uint32_t || 1 If Success 0 If Fail
 */
uint32_t addToEvents(uint32_t divName, uint32_t eventMask, uintptr_t func, swcWin* win)
{
    void* eventGroups = retrieveName(win->eventGroups, &win->manager);
    for(uint32_t i = 0; i < *(uint32_t*)eventGroups; i++)
    {
        char *pos = ((char*)eventGroups + sizeof(uint32_t) * 2 + i * (sizeof(uint32_t) + /*handletopointercount*/*((uint32_t*)eventGroups + 1) * (sizeof(uintptr_t) + sizeof(uint32_t))));
        if(eventMask & *(uint32_t*)pos)
        {
            //TODO: make this better, It does not need to be a for loop and use i
            pos += sizeof(uint32_t);
            for(i = 0; i < *((uint32_t*)eventGroups + 1); i++)
            {
                if(*(uintptr_t*)pos == func)
                {
                    swcName* divNameContainerName = retrieveNameL(*(uintptr_t*)(pos + sizeof(uint32_t)), &win->manager);
                    for(i = 0, i < divNameContainerName->size / sizeof(uint32_t); i++;)
                    {
                        if(*((uint32_t*)divNameContainerName->pointer + i) != 0)
                        {
                            *((uint32_t*)divNameContainerName->pointer + i) = 0;
                            return 1;
                        }
                    }
                    //TODO:
                    //reallocate this name container but larger because it reached teh end of the for loop without finding space
                    return 0;
                }
                if(*(uintptr_t*)pos == 0)
                {
                    //create new eventhandle
                    //add current divname to it
                    *(uintptr_t*)pos = func;
                    //TODO: look at all allocNamed calls to see if they should really just return a swcName* instead of retrieveName...
                    uint32_t name = allocNamed(InitialHandleToDivCount, &win->manager);
                    uint32_t* divContainer = (uint32_t*)retrieveName(name, &win->manager);
                    
                    *divContainer = divName;
                    *(uint32_t*)(pos + sizeof(uintptr_t)) = name;
                    return 1;
                }
                pos += sizeof(uint32_t) + sizeof(uintptr_t);
            }
        }
    }
    return 0;
}

//TODO:
//realloc
uint32_t reallocEvents(swcWin *win)
{
    //in this func we need
    return 0;
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

    addToEvents(div, eventTypeMask, (uintptr_t)eventFunc, win);

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
/**
 * @brief at end because of nested function
 * 
 * @param win 
 * @return uint32_t 
 */
uint32_t handleEvents(swcWin* win)
{
    uint32_t events = XEventsQueued(win->dis, QueuedAfterFlush);
    for(events; events > 0; events--)
    {
        XEvent* event;
        XNextEvent(win->dis, event);

        //TODO:
        //Look into GNU C compiler and find how it translates switch statements... I tried but I got lost on file c-tree.h
        //what I would like to do is have a "switch" statement that only contains the necessary cases as provided by the windows
        //event mask, but I don't know how to do that in a more effecient manner than below

        //using gnu c extension nested funcs
        uint32_t* eventGroups = retrieveName(win->eventGroups, &win->manager);
        uint32_t size = *(eventGroups + 1);
        uint8_t groupCount = *(eventGroups);
        void pass_event(uint32_t target) { 
            for(uint8_t i = 0; i < (groupCount); i++)
            {
                if(*(eventGroups + 2 + 2 * i) & target)
                {
                    for(uint32_t c = 0; c < *(eventGroups + 3 + 2 * i); c++)
                    {
                        //((swcDiv*)retrieveName(*(eventGroups + 2 + 2 * groupCount + i * size), &win->manager))->eventFunc(event);

                    }
                    //I think a use of a goto __label__ here would be faster than returning to the switch and breaking
                    return;
                }
            }
        }
        switch(event->type)
        {
            case MotionNotify:
                break;
            case ButtonPress:
                break;
            case ButtonRelease:
                break;
            case ColormapNotify:
                break;
            case EnterNotify:
                break;
            case LeaveNotify:
                break;
            case Expose:
                break;
            case GraphicsExpose:
            //intended no break
            case NoExpose:
                break;
            case FocusIn:
            //intended no break
            case FocusOut:
                break;
            case KeymapNotify:
                break;
            case KeyPress:
                break;
            case KeyRelease:
                break;
            case PropertyNotify:
                break;
            case ResizeRequest:
                break;
            case CirculateNotify:
            //intended no break
            case ConfigureNotify:
            //intended no break
            case DestroyNotify:
            //intended no break
            case GravityNotify:
            //intended no break
            case MapNotify:
            //intended no break
            case ReparentNotify:
            //intended no break
            case UnmapNotify:
                break;
            case CirculateRequest:
            //intended no break
            case ConfigureRequest:
            //intended no break
            case MapRequest:
                break;
            case VisibilityNotify:
                break;
            //TODO: HANDLE BELOW EVENTS
            case ClientMessage:
                break;
            case MappingNotify:
                break;
            case SelectionClear:
                break;
            case SelectionNotify:
                break;
            case SelectionRequest:
                break;
        }
    }
}
