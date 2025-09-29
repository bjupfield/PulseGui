#include "swcWindow.h"

uint32_t eventHandler(swcWin* win);
uint32_t handleEvents(swcWin* win);
uint32_t initEventGroups(swcWin* swcWin, uint32_t eventGroups, uint32_t handleToEventCount);
uint32_t initProgramGroups(swcWin* win, uint32_t initialProgramCount, uint32_t initialLayerCount);


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
swcWin initWindow(uint32_t* config, uint64_t eventMask, uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy, uint32_t layerCount)
{
//TODO:
    //MAKE NO RETURN

    swcWin null = {};
    Display* display = XOpenDisplay(NULL);

    // TODO:
    //      Set ErrorHandler
    //      XSetErrorHandler(errorHandler);


    //Okay I'm going to cdocumenbt this function:

    //create the memory manager
    swcMemMan manager = createMan(40, 20000, 50000, 25000);

    //add memory space to the manager and add the windows space too it
    addArena(sizeof(swcDiv) * 2000, + sizeof(swcWin), &manager);
    swcName windowName = allocNamed(sizeof(swcWin), &manager);
    ((swcWin*)retrieveName(windowName, &manager))->name = windowName;

    //itiitalize gl window data into window space
    if(!glInitWindowT(display, config, (swcWin*)retrieveName(windowName, &manager), eventMask))
    {
        //failed to initialize gl stuff
        return null;
    }

    //add the managers pointer to thte window struct
    ((swcWin*)retrieveName(windowName, &manager))->manager = &manager;
    //add the display to the window struct
    ((swcWin*)retrieveName(windowName, &manager))->dis = display;

    //increase arena space for some reason???? like i don't know what this why is this here?
    addArena(sizeof(swcDiv) * 2000, + sizeof(swcWin), &manager);
    addArena(sizeof(swcDiv) * 2000, + sizeof(swcWin), &manager);

    //intialize all arrayName things
    initProgramGroups(((swcWin*)retrieveName(windowName, &manager)), InitialProgramSize, (layerCount == 0 ? DefaultLayerCount : layerCount)); 
    ((swcWin*)retrieveName(windowName, &manager))->eventGroups = initEventGroups(((swcWin*)retrieveName(windowName, &manager)), eventMask, InitialEventToHandleSize);

    preRender((swcWin*)retrieveName(windowName, &manager));

    //fake div creation
    char exampleVertPath[256] = "GlShaders/exampleVert.vert\0";
    swcName divName = initDiv(((swcWin*)retrieveName(windowName, &manager)), 0, 24, 0, 0, 0, 1, baseLoad, baseDraw, baseDeleteFunc, baseResize, baseEvent, sizeof(swcDiv), ButtonPressMask, exampleVertPath, NULL);

    //mapping window and pushing to xorg... I think?
    XMapWindow(((swcWin*)retrieveName(windowName, &manager))->dis, ((swcWin*)retrieveName(windowName, &manager))->mainWin);
    XFlush(((swcWin*)retrieveName(windowName, &manager))->dis);

    //test del div
    //delDiv(((swcWin*)retrieveName(windowName, &manager)), divName);

    //main loop? 
    for(uint64_t i = 0; i < 4000; i++)
    {
        handleEvents((swcWin*)retrieveName(windowName, &manager));
        renderMain((swcWin*)retrieveName(windowName, &manager));
        frameChange(&manager);
        preRender((swcWin*)retrieveName(windowName, &manager));
        printf("hi");
        fflush(stdout);
    }


    desWindow((swcWin*)retrieveName(windowName, &manager));
    return null;
}

uint32_t desWindow(swcWin* win)
{
    //TODO: make this func? need to add something that deletes the divs
    XDestroyWindow(win->dis, win->mainWin);
    freeMemMan(win->manager);
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
    
    swcName name = allocNamed(sizeof(evntGroup) + sizeof(swcArrayName) * count, swcWin->manager);
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
uint32_t addToEvents(swcName divName, uint32_t eventMask, uintptr_t func, swcWin* win)
{
    uint8_t b = 0;
    evntGroup* eventGroups = (evntGroup*)retrieveName(win->eventGroups, win->manager);
    for(uint32_t i = 0; i < eventGroups->eventGroupCount; i++)
    {
        if(eventMask & eventGroups->events[i])
        {
            
            funcHandleArrays fake = {(handlePointer)func, 0};
            
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

/**
 * @brief Removes Div From Event->Function->Div Groups
 * 
 * @param divName 
 * @param eventMask 
 * @param func 
 * @param win 
 * @return Always returns 1?
 */
uint32_t removeFromEvents(swcName divName, uint32_t eventMask, uintptr_t func, swcWin* win)
{
    evntGroup* eventGroups = (evntGroup*)retrieveName(win->eventGroups, win->manager);
    for(uint32_t i = 0; i < eventGroups->eventGroupCount; i++)
    {
        if(eventMask & eventGroups->events[i])
        {
            
            funcHandleArrays fake = {(handlePointer)func, 0};

            if(!swcContainsArray(eventGroups->funcGroup[i], fake, handleSorter, win->manager))
            {
                //function is not created under group continue (shouldnt ever occur but whatever)
                continue;
            }
            funcHandleArrays *handle = swcAddArray(eventGroups->funcGroup[i], fake, handleSorter, win->manager);
            if(!swcRemoveArray(handle->divsName, divName, nameToDivSorter, win->manager))
            {
                //div is not under function group (shouldnt ever occur)
                continue;
            }
        }
    }
    return 1;
}



/**
 * @brief Initializes arrays for rendering purposes
 * 
 * @param win 
 * @param initialSize Amount Of Programs to Initialize for
 * @return Return 0 if Fail | Return 1 if Success
 */
uint32_t initProgramGroups(swcWin* win, uint32_t initialProgramCount, uint32_t initialLayerCount)
{
    win->glProgramNames = swcAllocArray(initialProgramCount, programNames, win->manager);
    win->divLayers = swcAllocArray(initialLayerCount, layerToProgram, win->manager);
    for(uint32_t i = 0; i < initialLayerCount; i++)
    {
        layerToProgram add;
        add.layer = i;
        add.programGroups = swcAllocArray(initialProgramCount, nameToDiv, win->manager);
        swcAddArray(win->divLayers, add, nameToDivSorter, win->manager);
    }
    return 1;
}

/**
 * @brief Checks to see if shader is already compiled and linked, if not creates program and assigns to it, 
 * else assigns to the already created program group
 * 
 * in the process of changing how the programs are created and assigned, first there will be two blocks for program data
 * the first block will relate the program path to the program name, the second will be how rendering is done, first an array that holds 
 * arrays, these held arrays will represent layers, and each layer will hold spaces or other arrays representing the individual programs, so
 * on render it will activate layer by layer, iterating through each program group in the layers.
 * to do this in this function first a check must be ran on the programArray, to see if the program already exist, if it does do nothing in this array but save the glName value,
 * if not create the program and add it to the array and save the glName value. than for the second array check the layer that the div is assigned to, see if that layer has a program space of the tpye that
 * this div is assigned to through the saved glprogramname, if it does assign that div to it, if it does not create the space and than add the div to it
 * 
 * 
 * @param divName 
 * @param pathName 
 * @param win 
 * @return 0 if Failure | Program Name if Success if Success
 */
uint32_t addToProgram(swcName divName, uint32_t layer, const char pathName[256], swcWin* win)
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

    uint32_t programName;
    nameToDiv holder;
    if(retrieved->programName == 0)//program has not been assigned/created... create program
    {
        retrieved->programName = createProgram(pathName, win);
        holder.divs = swcAllocArray(InitialProgramToDivSize, uint32_t, win->manager);    
    }
    programName = retrieved->programName;
    

    //retrieve layers programgroups
    layerToProgram *tempLayerToProgram = (layerToProgram*)allocSB(sizeof(layerToProgram), win->manager);
    tempLayerToProgram->layer = layer;
    tempLayerToProgram = (layerToProgram*)swcAddArray(win->divLayers, *tempLayerToProgram, nameToDivSorter, win->manager);
    //currently not checking to see if a layer exist or not assuming it does

    //retrieve programsname divgroup
    nameToDiv *tempNameToDivs = (nameToDiv*)allocSB(sizeof(nameToDiv), win->manager);;
    tempNameToDivs->programName = programName;
    tempNameToDivs->divs = NULL;
    tempNameToDivs = (nameToDiv*)swcAddArray(tempLayerToProgram->programGroups, *tempNameToDivs, nameToDivSorter, win->manager);
    if(tempNameToDivs->divs == NULL)
    {
        //attach vertex buffer object to array
        tempNameToDivs->vertexBufferObjectName = glGenBuffer(win);
        if(tempNameToDivs->vertexBufferObjectName == 0)
        {
            return 0;
        }
        //allocate new array to hold divs, as it does not exist
        tempNameToDivs->divs = swcAllocArray(InitialProgramToDivSize, swcName, win->manager);
        tempNameToDivs->cpuSideBufferObjectData = NULL;
        tempNameToDivs->gpuBufferDataSize = 0;
        tempNameToDivs->cpuBufferObjectDataElementCount = 0;
    }
    

    //add this div to that layer->programgroup->divs array
    if(swcAddArray(tempNameToDivs->divs, divName, nameToDivSorter, win->manager) != 0)
    {
        return programName;
    }

    return 0;
}


uint32_t removeFromProgram(swcName divName, uint32_t programName, uint32_t layer, swcWin* win)
{
    layerToProgram tempLayer;
    tempLayer.layer = layer;
    uint32_t index = swcContainsArray(win->divLayers, tempLayer, nameToDivSorter, win->manager);
    if(index == -1)
    {
        //layer does not exist
        return 0;
    }
    swcArray* arr = retrieveArray(win->divLayers, win->manager);
    tempLayer = ((layerToProgram*)((retrieveArray(win->divLayers, win->manager))->data))[index];

    nameToDiv tempNameToDiv;
    tempNameToDiv.programName = programName;
    index = swcContainsArray(tempLayer.programGroups, tempNameToDiv, nameToDivSorter, win->manager);
    if(index == -1)
    {
        //program group in layer does not exist
        return 0;
    }

    tempNameToDiv = ((nameToDiv*)((retrieveArray(tempLayer.programGroups, win->manager))->data))[index];
    
    return swcRemoveArray(tempNameToDiv.divs, divName, nameToDivSorter, win->manager);
    
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
                        swcName* divNames = allocSB(names->curSize, win->manager);
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


