#include "swcWindow.h"

uint32_t eventHandler(swcWin* win);
uint32_t handleEvents(swcWin* win);
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

    printf("Sizeof(GLXContext): %i || Sizeof(GlxWindow): %i\n",sizeof(GLXContext), sizeof(GLXWindow));

    XVisualInfo* info = retVisual(display, config == 0 ? defConfiguration: config);

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

    win.glHandle = glInitWindow(win.dis, win.mainWin);

    printf(win.glHandle == 0 ? "Failure\n\n\n\n\n\n\n\n\n" : "Success\n\n\n\n\n\n");
    XMapWindow(win.dis, win.mainWin);
    // XFlush(win.dis);

    win.manager = createMan(sizeof(swcDiv) * 5000, 10, 50000, 25000);

    addArena(sizeof(swcDiv) * 2000, sizeof(swcDiv), &win.manager);

    win.eventGroups = initEventGroups(&win, eventMask, 40);

    //for testing divs?
    uint32_t divName = initDiv(&win, 0, 24, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), ButtonPressMask, NULL);
    uint32_t di = initDiv(&win, 0, 24, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), ButtonPressMask, NULL);


    swcDiv *div = retrieveName(divName, &win.manager);

    printf("\n\ndiv.posx: %i\n\n", div->posx);

    // EVENT HANDLER


    for(uint64_t i = 0; i < 20000000; i++)
    {
        handleEvents(&win);
        frameChange(&win.manager);
    }
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
    uint8_t bool = 0;
    swcWin->event_mask = eventGroups;
    for(uint32_t mask = 1; mask < 1 << 26; mask <<= 1)//refer to X.h event definitions for the explanations of this bit mask
    {
        if(eventGroups & (ButtonMotionMask|Button1MotionMask|Button2MotionMask|Button3MotionMask|Button4MotionMask|Button5MotionMask|PointerMotionMask) & mask)
            {
                if(!bool)
                {
                    count++;
                    bool = 1;
                }
                continue;
            }
        if(eventGroups & mask)
        {
            count++;
        }
    }
    
    uint32_t name = allocNamed(sizeof(uint32_t)  * 2 + sizeof(uint32_t) * count + (count * handleToEventCount) * (sizeof(uintptr_t) + sizeof(uint32_t)), &swcWin->manager);
    evntGroup* eventGroup = retrieveName(name, &swcWin->manager);
    
    eventGroup->eventGroupCount = count;
    eventGroup->handleToEventCount = handleToEventCount;
    //um ignore this plz:
    for(uint32_t mask = 1, bool = 0, count = 0; mask < (1 << 26); mask <<= 1)
    {
        if(eventGroups & (ButtonMotionMask|Button1MotionMask|Button2MotionMask|Button3MotionMask|Button4MotionMask|Button5MotionMask|PointerMotionMask) & mask)
        {
            if(!bool)
            {
                eventGroup->events[count++] = PointerMotionMask;//they all use the same event type, motion notify
                bool = 1;
            }
            continue;
        }
        if(eventGroups & mask)
        {
            eventGroup->events[count++] = mask;
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
    for(int i = 0; i < 20000000; i++){

    }
    evntGroup* eventGroups = (evntGroup*)retrieveName(win->eventGroups, &win->manager);
    for(uint32_t i = 0; i < eventGroups->eventGroupCount; i++)
    {
        if(eventMask & eventGroups->events[i])
        {
            uint32_t saveIt = i;
            //TODO: make this better, It does not need to be a for loop and use i
            for(i = 0; i < eventGroups->handleToEventCount; i++)
            {
                uint32_t pos = saveIt * eventGroups->handleToEventCount + i;
                if(eventGroups->funcHandles[pos].func == func)
                {
                    swcName* divNameContainerName = retrieveNameL(eventGroups->funcHandles[pos].divsName, &win->manager);
                    for(i = 0, i < divNameContainerName->size / sizeof(uint32_t); i++;)
                    {
                        if(*((uint32_t*)divNameContainerName->pointer + i) != 0)
                        {
                            *((uint32_t*)divNameContainerName->pointer + i) = divName;
                            return 1;
                        }
                    }
                    //TODO:
                    //reallocate this name container but larger because it reached teh end of the for loop without finding space
                    return 0;
                }
                if(eventGroups->funcHandles[pos].func == 0)
                {
                    //create new eventhandle
                    //add current divname to it
                    eventGroups->funcHandles[pos].func = func;
                    //TODO: look at all allocNamed calls to see if they should really just return a swcName* instead of retrieveName...
                    uint32_t name = allocNamed(InitialHandleToDivCount * sizeof(uint32_t), &win->manager);
                    uint32_t* divContainer = (uint32_t*)retrieveName(name, &win->manager);
                    
                    *divContainer = divName;
                    eventGroups->funcHandles[pos].divsName = name;
                    return 1;
                }
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

    // divC(divPoint, onLoad);

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

/**
 * @brief at end because of nested function
 * 
 * @param win 
 * @return uint32_t 
 */
uint32_t FAKE = 0;
uint32_t handleEvents(swcWin* win)
{
    XEvent event = { 0 };
    while(XCheckWindowEvent(win->dis, win->mainWin, win->event_mask, &event))
    {
        //TODO:
        //Look into GNU C compiler and find how it translates switch statements... I tried but I got lost on file c-tree.h
        //what I would like to do is have a "switch" statement that only contains the necessary cases as provided by the windows
        //event mask, but I don't know how to do that in a more effecient manner than below

        //using gnu c extension nested funcs
        evntGroup* eventGroups = (evntGroup*)retrieveName(win->eventGroups, &win->manager);
        uint32_t handleCount = eventGroups->handleToEventCount;
        uint8_t groupCount = eventGroups->eventGroupCount;

        eventGroups->funcHandles[0].func;

        //TODO:
            //The Way Events are handled is psychotic, think of changing, but its rather specific code, and its done well, just like a madman though
        void pass_event(uint32_t target) { 
            for(uint32_t c = 0; c < (groupCount); c++)
            {
                if(eventGroups->events[c] & target)
                {
                    uint32_t save_it = c * handleCount;
                    for(c = 0; c < handleCount; c++)
                    {
                        handlePointer func = (handlePointer)eventGroups->funcHandles[save_it + c].func;
                        if(func == NULL)
                        {
                            return;
                        }
                        //TODO: retrieve and sort funcs right, for now lazily pass?
                        swcName* name =  retrieveNameL(eventGroups->funcHandles[save_it + c].divsName, &win->manager);

                        swcDiv** divs = (swcDiv**)allocSB(sizeof(swcDiv*) * (name->size / sizeof(uint32_t)), &win->manager);
                        for(uint32_t b = 0; b < name->size / sizeof(uint32_t); b++)
                        {
                            //why am i passing every single div inside this group to the func?
                            divs[b] = retrieveName(*((uint32_t*)name->pointer + b), &win->manager);
                        }
                        func(divs, &event);
                        

                    }
                    //I think a use of a goto __label__ here would be faster than returning to the switch and breaking
                    return;
                }
            }
        }
        switch(event.type)
        {
            case MotionNotify:
                pass_event(ButtonMotionMask|Button1MotionMask|Button2MotionMask|Button3MotionMask|Button4MotionMask|Button5MotionMask|PointerMotionMask);
                break;
            case ButtonPress:
                pass_event(ButtonPressMask);
                break;
            case ButtonRelease:
                pass_event(ButtonReleaseMask);
                break;
            case ColormapNotify:
                pass_event(ColormapChangeMask);
                break;
            case EnterNotify:
                pass_event(EnterWindowMask);
                break;
            case LeaveNotify:
                pass_event(LeaveWindowMask);
                break;
            case Expose:
                pass_event(ExposureMask);
                break;
            case GraphicsExpose:
            //intended no break
            case NoExpose:
                pass_event(GCGraphicsExposures);
                break;
            case FocusIn:
            //intended no break
            case FocusOut:
                pass_event(FocusChangeMask);
                break;
            case KeymapNotify:
                pass_event(KeymapStateMask);
                break;
            case KeyPress:
                pass_event(KeyPressMask);
                break;
            case KeyRelease:
                pass_event(KeyReleaseMask);
                break;
            case PropertyNotify:
                pass_event(PropertyChangeMask);
                break;
            case ResizeRequest:
                //TODO: handle resize event
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
                pass_event(StructureNotifyMask|SubstructureNotifyMask);
                break;
            case CirculateRequest:
            //intended no break
            case ConfigureRequest:
            //intended no break
            case MapRequest:
                pass_event(SubstructureRedirectMask);
                break;
            case VisibilityNotify:
                pass_event(VisibilityChangeMask);
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
    return 0;
}
