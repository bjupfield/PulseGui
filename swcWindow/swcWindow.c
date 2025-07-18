#include "swcWindow.h"

uint32_t eventHandler(swcWin* win);
uint32_t handleEvents(swcWin* win);
uint32_t initEventGroups(swcWin* swcWin, uint32_t eventGroups, uint32_t handleToEventCount);
uint32_t initProgramGroups(swcWin* win, uint32_t initialSize);


//plz for the love of god delete this
typedef struct
{
    uint32_t sortByThis;
    uint64_t miscellaneous;
}fake;
uint32_t fakeSorter(void* left, void* right)
{
    fake *fakel = (fake*)left;
    fake *faker = (fake*)right;
    if(fakel->sortByThis < faker->sortByThis) 
    {
        return 0;
    }
    if(fakel->sortByThis == faker->sortByThis)
        return 1;
    return 2;
}
void fakeInsert(void* left, void* right)
{
    fake *fakel = (fake*)left;
    fake *faker = (fake*)right;
    faker->miscellaneous = fakel->miscellaneous;
    faker->sortByThis = fakel->sortByThis;
}

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
    Display* display = XOpenDisplay(NULL);

    // TODO:
    //      Set ErrorHandler
    //      XSetErrorHandler(errorHandler);



    swcMemMan manager = createMan(40, 20000, 50000, 25000);

    addArena(sizeof(swcDiv) * 2000, + sizeof(swcWin), &manager);

    uint32_t windowName = allocNamed(sizeof(swcWin), &manager);
    
    fflush(stdout);

    fake fakest = {1, 2};

    // swcArrayName array = allocArray(30, sizeof(fake), fakeSorter, fakeInsert, &manager);
    swcArrayName array = swcAllocArray(30, fake, &manager)

    fake fakest2 = {2, 2};

    swcAddArray(array, fakest, fakeSorter, &manager)
    swcAddArray(array, fakest2, fakeSorter, &manager)
    swcAddArray(array, fakest, fakeSorter, &manager)
    fakest.sortByThis = 0;
    swcAddArray(array, fakest, fakeSorter, &manager)
    // swcRemoveArray(array, fakest2, fakeSorter, &manager)


    swcArray* b = (swcArray*)retrieveName(array, &manager);

    fake *fakest3 = (fake*)(b->data);
    for(int i = 0; i < 3; i++)
    {
        printf("SortByThisis: %i\n", fakest3[i]);
    }

    if(!glInitWindowT(display, config, (swcWin*)retrieveName(windowName, &manager), eventMask))
    {
        //failed to initialize gl stuff
        return null;
    }

    ((swcWin*)retrieveName(windowName, &manager))->manager = &manager;

    ((swcWin*)retrieveName(windowName, &manager))->dis = display;

    addArena(sizeof(swcDiv) * 2000, + sizeof(swcWin), &manager);
    addArena(sizeof(swcDiv) * 2000, + sizeof(swcWin), &manager);

    initProgramGroups(((swcWin*)retrieveName(windowName, &manager)), InitialProgramSize); 
    ((swcWin*)retrieveName(windowName, &manager))->eventGroups = initEventGroups(((swcWin*)retrieveName(windowName, &manager)), eventMask, InitialEventToHandleSize);

    uint32_t divName = initDiv(((swcWin*)retrieveName(windowName, &manager)), 0, 24, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), ButtonPressMask, NULL);


    XMapWindow(((swcWin*)retrieveName(windowName, &manager))->dis, ((swcWin*)retrieveName(windowName, &manager))->mainWin);
    XFlush(((swcWin*)retrieveName(windowName, &manager))->dis);

    for(uint64_t i = 0; i < 2000000; i++)
    {
        handleEvents((swcWin*)retrieveName(windowName, &manager));
        frameChange(&manager);

    }

    desWindow((swcWin*)retrieveName(windowName, &manager));
    return null;

    // swcWin win = {
    //     .mainWin = XCreateWindow(
    //     display, RootWindow(display, 0), 400, 400, 300, 300, 0, 					
    //     info->depth, CopyFromParent, info->visual, 
	// 	CWBackPixel|CWEventMask|CWColormap|CWOverrideRedirect, &wa),
    // };

    // XFree(info);

    // win.dis = display;


    // //in.glHandle = glInitWindow(win.dis, win.mainWin);

    // //printf(win.glHandle == 0 ? "Failure\n\n\n\n\n\n\n\n\n" : "Success\n\n\n\n\n\n");
    // XMapWindow(win.dis, win.mainWin);
    // // XFlush(win.dis);

    // manager = createMan(sizeof(swcDiv) * 5000, 10, 50000, 25000);

    // win.manager = &manager;

    // addArena(sizeof(swcDiv) * 2000, sizeof(swcDiv), win.manager);

    // win.eventGroups = initEventGroups(&win, eventMask, 40);

    // //for testing divs?
    // uint32_t divName = initDiv(&win, 0, 24, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), ButtonPressMask, NULL);
    // uint32_t di = initDiv(&win, 0, 24, 0, 0, 0, baseLoad, baseDraw, baseResize, baseEvent, sizeof(swcDiv), ButtonPressMask, NULL);


    // swcDiv *div = retrieveName(divName, win.manager);

    // printf("\n\ndiv.posx: %i\n\n", div->posx);

    // // EVENT HANDLER

    // for(uint64_t i = 0; i < 20000; i++)
    // {
    //     handleEvents(&win);
    //     frameChange(win.manager);

    // }
    //DRAWER


    //remove dummy
    // desWindow(&win);

    // return win;
}

uint32_t desWindow(swcWin* win)
{
    //TODO: make this func?
    XDestroyWindow(win->dis, win->mainWin);
    freeMemMan(win->manager);
    return 1;
}

uint32_t handleSorter(void* left, void* right);
uint32_t nameToDivSorter(void* left, void* right);
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
    
    uint32_t name = allocNamed(sizeof(evntGroup) + sizeof(swcArrayName) * count, swcWin->manager);
    evntGroup* eventGroup = retrieveName(name, swcWin->manager);
    
    eventGroup->eventGroupCount = count;
    //um ignore this plz:
    for(uint32_t mask = 1, bool = 0, count = 0; mask < (1 << 26); mask <<= 1)
    {
        if(eventGroups & (ButtonMotionMask|Button1MotionMask|Button2MotionMask|Button3MotionMask|Button4MotionMask|Button5MotionMask|PointerMotionMask) & mask)
        {
            if(!bool)
            {
                eventGroup->events[count] = PointerMotionMask;//they all use the same event type, motion notify
                eventGroup->funcGroup[count++] = swcAllocArray(InitialEventToHandleSize, funcHandleArrays, swcWin->manager); 
                bool = 1;
            }
            continue;
        }
        if(eventGroups & mask)
        {
            eventGroup->events[count] = mask;
            eventGroup->funcGroup[count++] = swcAllocArray(InitialEventToHandleSize, funcHandleArrays, swcWin->manager);
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
 * @return 0 if Fail (No EventGroups Allowed in Window or Bug) | 1 if Success
 */
uint32_t addToEvents(uint32_t divName, uint32_t eventMask, uintptr_t func, swcWin* win)
{
    uint8_t b = 0;
    evntGroup* eventGroups = (evntGroup*)retrieveName(win->eventGroups, win->manager);
    for(uint32_t i = 0; i < eventGroups->eventGroupCount; i++)
    {
        if(eventMask & eventGroups->events[i])
        {
            
            funcHandleArrays fake = {func, 0};
            
            funcHandleArrays *handle = swcAddArray(eventGroups->funcGroup[i], fake, handleSorter, win->manager);

            if(handle->divsName == 0)
            {
                //function div container did not exist, create
                handle->divsName = swcAllocArray(InitialHandleToDivSize, swcArrayName, win->manager);
            }
            swcArrayName *retName = swcAddArray(handle->divsName, divName, nameToDivSorter, win->manager);
            if(*retName != divName)
            {
                //failed for some reason?
                return 0;
            }
            b = 1;
        }
    }
    if(b == 1)
    {
        return 1;
    }
    
    return 0;
}

//CREATE: make this func
uint32_t removeFromEvents(uint32_t divName, uint32_t eventMask, uintptr_t func, swcWin* win)
{

}


uint32_t handleSorter(void* left, void* right)
{
    uint64_t *leftN = (uint64_t*)left;
    uint64_t *rightN = (uint64_t*)right;
    if(*leftN < *rightN)
        return 0;
    if(*leftN == * rightN)
        return 1;
    return 2;
}

uint32_t programNameSorter(void* left, void* right);


/**
 * @brief 
 * 
 * @param win 
 * @param initialSize Amount Of Programs to Initialize for
 * @return Return 0 if Fail | Return 1 if Success
 */
uint32_t initProgramGroups(swcWin* win, uint32_t initialSize)
{
    win->glProgramNames = swcAllocArray(initialSize, programNames, win->manager);
    win->glNamesToDivs = swcAllocArray(initialSize, nameToDiv, win->manager);
    return 1;
}

/**
 * @brief Checks to see if shader is already compiled and linked, if not creates program and assigns to it, 
 * else assigns to the already created program group
 * 
 * @param divName 
 * @param pathName 
 * @param win 
 * @return 0 if Failure | Program Name if Success if Success
 */
uint32_t addToProgram(uint32_t divName, const char pathName[256], swcWin* win)
{
    programNames b;
    strcpy(b.pathName, pathName);
    b.programName = 0;//hope it doesnt throw a 0 program name, fix later if a problem
    programNames *retrieved = (programNames*)swcAddArray(win->glProgramNames, b, programNameSorter, win->manager);
    if(retrieved == 0)
    {
        //Failure has occured
        return 0;
    }
    nameToDiv *retrieved2;
    if(retrieved->programName == 0)
    {
        //TODO:
        //create a new program and add its name

        retrieved->programName = 1;// assign it here

        //create new programname to div container
        nameToDiv newProgramToDiv;
        newProgramToDiv.programName = retrieved->programName;
        newProgramToDiv.divs = swcAllocArray(InitialProgramToDivSize, uint32_t, win->manager);
        
        //add name container to container and retrieve
        retrieved2 = (nameToDiv *)swcAddArray(win->glNamesToDivs, newProgramToDiv, nameToDivSorter, win->manager);
    }
    else
    {
        //retreive continer from container
        retrieved2 = (nameToDiv *)swcAddArray(win->glNamesToDivs, (retrieved->programName), nameToDivSorter, win->manager);
    }
    if(retrieved2 != 0)
    {
        //assign the current div to container and check if it assigned
            uint32_t *retName = (uint32_t *)swcAddArray(retrieved2->divs, divName, nameToDivSorter, win->manager);
            if(retName != 0)
            {
                return *retName;
            }
    }
    return 0;
}


uint32_t removeFromProgram(uint32_t divName, uint32_t programName, swcWin* win)
{
    if(swcRemoveArray(win->glNamesToDivs, programName, nameToDivSorter, win->manager))
    {
        printf("it was removed\\n\n\n\n\n\n\nn\n\n\n\n");
        return 1;
    }
    return 0;
    
}


uint32_t programNameSorter(void* left, void* right)
{
    programNames *leftP = (programNames*)left;
    programNames *rightP = (programNames*)right;
    uint8_t i = 0;
    while(leftP->pathName[i] != rightP->pathName[i] || leftP->pathName[i] == '\n')
    {
        i++;
    }
    if(leftP->pathName[i] < rightP->pathName[i]) 
        return 0;
    if(leftP->pathName[i] == rightP->pathName[i])
        return 1;
    return 2;
}

uint32_t nameToDivSorter(void* left, void* right)
{
    uint32_t *leftN = (uint32_t*)left;
    uint32_t *rightN = (uint32_t*)right;
    if(*leftN < *rightN)
        return 0;
    if(*leftN == * rightN)
        return 1;
    return 2;
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
    uint32_t div = allocNamed(size, win->manager);
    swcDiv* divPoint = (swcDiv*)retrieveName(div, win->manager);

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
    divPoint->programName = addToProgram(div, "fake", win);

    addToEvents(div, eventTypeMask, (uintptr_t)eventFunc, win);

    size_t excSize = size - sizeof(swcDiv);
    memcpy((char*)divPoint + sizeof(swcDiv), excData, excSize);

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
{//TODO: remove all references


    uint32_t success = deallocNamed(div, win->manager);
    return success;
}

/**
 * @brief at end because of nested function
 * 
 * @param win 
 * @return uint32_t 
 */
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
        evntGroup* eventGroups = (evntGroup*)retrieveName(win->eventGroups, win->manager);
        uint8_t groupCount = eventGroups->eventGroupCount;

        //TODO:
            //The Way Events are handled is psychotic, think of changing, but its rather specific code, and its done well, just like a madman though
        void pass_event(uint32_t target) { 
            for(uint32_t c = 0; c < (groupCount); c++)
            {
                if(eventGroups->events[c] & target)
                {
                    swcArray *funcs = retrieveArray(eventGroups->funcGroup[c], win->manager);
                    funcHandleArrays* funcs2 = (funcHandleArrays*)funcs->data;
                    for(c = 0; c < funcs->curSize; c++)
                    {
                        swcArray *names = retrieveArray(funcs2[c].divsName, win->manager);
                        uint32_t* divNames = allocSB(names->curSize, win->manager);
                        memcpy(divNames, names->data, names->curSize * sizeof(uint32_t));
                        funcs2[c].func(divNames, names->curSize, &event); 
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
