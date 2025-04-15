#include "swcWindow.h"


uint32_t defConfiguration[] =
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

uint64_t defMask = ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|
    EnterWindowMask|LeaveWindowMask|ExposureMask|PointerMotionMask|
    VisibilityChangeMask|StructureNotifyMask;


//TODO:
//  CREATE A MEMEORY MANAGER THAT HANDLES ALL ARENAS.... OR AT LEAST MOST OF THEM.... HOPEFULLY
//  SIMPLE JUST HAVE AN ARRAY OF ARENAS, THAT IT ASSIGNS THE LATEST DATA TO THE LAST ONE, WHICH IT
//  HAS A SIMPLE int POINTER TO THE ARRAY FOR, AND THAN HAVE IT SORT AND DISCARD THIS ARRAY EVERY
//  TIME A REALLOC CALL OR WHATEVER WE ARE SAYING NEEDS TO HAPPEN TO FREE MEMORY OCCURS

/**
 * @brief Create and return an arena of size 
 * 
 * @param size The amount of memory to allocate
 * @param avgData Around how large most data points should be, used to allocate assigned memory locater
 * because of this, this shouldnt be used for small and known data types, only large and uknown
 * @return swcArena 
 */
swcArena creArena(size_t size, size_t avgData)
{
    //TODO: couple with garbage collector
    size_t calcSize = size + ((size / (avgData * 2)) * sizeof(swcFreedPointer));
    swcArena arena = {0};
    arena.origin = calloc(1, calcSize);//need initialization to zero for assigned pointers
    arena.beg = arena.origin;
    arena.end = arena.beg + (ptrdiff_t)calcSize;
    arena.assignedPointers = arena.end;

    return arena;
}
/**
 * @brief Destroys arena
 * 
 * @param arena 
 * @return uint32_t 
 */

uint32_t desArena(swcArena *arena)
{
    //TODO: add decoupling from garbage collector
    free(arena->origin);
    memset(arena, 0, sizeof(swcArena));
}

/**
 * @brief return pointer with allocated size * count
 * 
 * @param size 
 * @param count 
 * @return void* || Returns allocated data if success, if fail nullptr if failed(no space left in arena) 
 */
void *alloc(swcArena *a, size_t size)
{
    //everythings aligned on 16 bytes
    //dereference pointer at the end of all data so on dealloc calls can say data is gone
    //first find padding for original data
    //second alignment with dereference pointer
    size_t padding = -(size_t)a->beg & (uint32_t)15;
    size_t dereferencePointer = size + (-(size + padding) & (alignof(size_t) - 1));//location of dereference pointer
    size_t remaining = a->assignedPointers - a->beg;

    if(remaining < dereferencePointer + sizeof(size_t) + padding)
    {
        return 0;
        //TODO: handle this
        //needs like an outside func that catches a null pointer and than allocates a new arena to hold more data
    }

    void *pointer;
    a->beg += padding;
    
    a->assignedPointers -= sizeof(size_t);
    *(a->assignedPointers) = (size_t)a->beg;

    pointer = a->beg;
    
    a->beg += dereferencePointer;
    *(a->beg) = (size_t)a->assignedPointers;

    a->beg += sizeof(size_t);

    return pointer;
}

//above func heavily inspired by:
//https://nullprogram.com/blog/2023/09/27/
//srry for not a proper reference :P

/**
 * @brief Deallocates pointers so on next realloc of an overflowing arena its memory is wiped
 * 
 * @param pointer data location
 * @param size size of data
 * @return 1 if success
 */

uint32_t deAlloc(void* pointer, size_t size)
{
    (*(size_t*)(*(size_t*)(pointer + size + (-(size + (size_t)pointer) & (alignof(size_t) - 1))))) = 0;
    //grab the pointers pointer to a pointer and dereference that pointers pointer by setting it to zero
    return 1;
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

    //remove dummy
    XDestroyWindow(display, win.mainWin);

    swcArena a;
    ;

    return win;
}
// struct theWindow mainWin;
// int initializeWindow()
// {
//     struct theWindow mainWin = 
//     {
//         .divs = (winDiv*)malloc(sizeof(winDiv) * 512),
//         .divArrSize = 512,
//         .divCount = 0,
//     };
//     return 1;
// }
// int adjustWinSize()
// {
//     return 0;
// }
// //lazy right now
// int addDiv(int posx, int posy, int width, int height)
// {
//     //shouldnt ever happen
//     if(mainWin.divCount >= mainWin.divArrSize)
//     {
//         mainWin.divArrSize *= 2;
//         mainWin.divs = (winDiv*)realloc(mainWin.divs, sizeof(winDiv) * mainWin.divArrSize);
//     }

//     winDiv* init = &mainWin.divs[mainWin.divCount];
//     init->height = height;
//     init->width = width;
//     init->xPos = posx;
//     init->yPos = posy;

//     mainWin.divCount += 1;

//     return 1;
// }
// vertexBI drawWindow()
// {
//     struct corner
//     {
//         int x;
//         int y;
//     };
//     typedef struct corner corn;
//     vertexBI ret;
//     ret.elements = 6;
//     ret.vertexCount = mainWin.divCount * 6;
//     ret.vertexBuffer = mainWin.vBuffer;//hmmm
//     //TODO:
//     // to make this like analyze what actually needs to be adjusted, like flag vertexses that are wrong... this is complicated so we push it off

//     ret.vertexBuffer = (float*)malloc(sizeof(float) * ret.elements * ret.vertexCount);

//     float c1[3] = {1.0, 0.0, 0.0};
//     float c2[3] = {0.0, 1.0, 0.0};
//     float c3[3] = {0.0, 0.0, 1.0};

//     for(int i = 0; i < ret.vertexCount; i++)
//     {
//         winDiv drawDiv = mainWin.divs[i];  
//         corn leftU = 
//         {
//             .x = drawDiv.xPos,
//             .y = drawDiv.yPos
//         };
//         corn rightU = 
//         {
//             .y = leftU.y,
//             .x = drawDiv.width + leftU.x,
//         };
//         corn leftL = 
//         {
//             .y = leftU.y + drawDiv.height,
//             .x = leftU.x,
//         };
//         corn rightL = 
//         {
//             .y = leftL.y,
//             .x = rightU.x,
//         };

//         //ll corner t1
//         float horror[3] = {leftL.x, leftL.y, 0.0 };
//         memcpy((float*)ret.vertexBuffer + i * ret.elements, horror, 12);
//         memcpy((float*)ret.vertexBuffer + i * ret.elements + 3, c1, 12);

//         //ul corner t1
//         float horror[3] = {leftU.x, leftU.y, 0.0};
//         memcpy((float*)ret.vertexBuffer + (i + 1) * ret.elements, horror, 12);
//         memcpy((float*)ret.vertexBuffer + (i + 1) * ret.elements + 3, c2, 12);

//         //ur corner t1
//         float horror[3] = {rightU.x, rightU.y, 0.0};
//         memcpy((float*)ret.vertexBuffer + (i + 2) * ret.elements, horror, 12);
//         memcpy((float*)ret.vertexBuffer + (i + 2) * ret.elements + 3, c3, 12);

//         //ll corner t2
//         float horror[3] = {leftL.x, leftL.y, 0.0};
//         memcpy((float*)ret.vertexBuffer + (i + 3) * ret.elements, horror, 12);
//         memcpy((float*)ret.vertexBuffer + (i + 3) * ret.elements + 3, c1, 12);

//         //lr corner t2
//         float horror[3] = {rightL.x, rightL.y, 0.0};
//         memcpy((float*)ret.vertexBuffer + (i + 4) * ret.elements, horror, 12);
//         memcpy((float*)ret.vertexBuffer + (i + 4) * ret.elements + 3, c2, 12);

//         //ur corner t2
//         float horror[3] = {rightU.x, rightU.y, 0.0};
//         memcpy((float*)ret.vertexBuffer + (i + 5) * ret.elements, horror, 12);
//         memcpy((float*)ret.vertexBuffer + (i + 5) * ret.elements + 3, c3, 12);
//         //i need to plan this...
//     }

//     return ret;
// }
