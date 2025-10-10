#include "swcDiv.h"
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
 * @param deleteFunc
 * @param resizeFunc 
 * @param eventFunc
 * @param size
 * @param vertexPerDiv The amount of vertexes each div produces in for its rendering
 * @param eventTypeMask Event Mask that Assigns Divs to recieve events from window, event mask is identical to XORG Input Event Mask
 * @param excData
 * @return Returns Div Name 
 */
uint32_t initDiv(swcWin* win, uint32_t parent, 
    uint32_t posx, uint32_t posy, uint32_t dimx, uint32_t dimy, uint32_t layer,  
    funcPointer onLoad, funcPointer drawFunc, funcPointer deleteFunc,
    resizePointer resizeFunc, handlePointer eventFunc, size_t size,
    uint32_t vertexPerDiv, uint32_t renderType,
    uint32_t eventTypeMask, const char pathName[256], void* excData)
{
    swcName div = allocNamed(size, win->manager);
    swcDiv* divPoint = (swcDiv*)retrieveName(div, win->manager);

    if(divPoint == NULL)
    {
        return 0;
    }

    divPoint->name = div;
    divPoint->dimx = dimx;
    divPoint->dimy = dimy;
    divPoint->posx = posx;
    divPoint->posy = posy;
    divPoint->layer = layer;
    divPoint->parent = parent;
    divPoint->onLoad = onLoad;
    divPoint->drawFunc = drawFunc;
    divPoint->resizeFunc = resizeFunc;
    divPoint->eventFunc = eventFunc;
    divPoint->deleteFunc = deleteFunc;
    divPoint->size = size;
    divPoint->eventMask = eventTypeMask;
    divPoint->programName = addToProgram(DefaultInitialDivGroupCount, div, layer, vertexPerDiv, renderType, pathName, win);
    divPoint->win = win;

    addToEvents(div, eventTypeMask, (uintptr_t)eventFunc, win);

    size_t excSize = size - sizeof(swcDiv);
    memcpy((char*)divPoint + sizeof(swcDiv), excData, excSize);
    

    divC(divPoint, onLoad);

    return div;

}

/**
 * @brief 
 * 
 * @param win 
 * @param divName
 * @return uint32_t || Deleted div name if success if fail 0
 */
uint32_t delDiv(swcWin* win, swcName divName)
{

    swcDiv* div = (swcDiv*)retrieveName(divName, win->manager);
    if(div == 0)
    {
        return 0;
    }

    if(!removeFromProgram(divName, div->programName, div->layer, win))
    {
        return 0;
    }

    if(!removeFromEvents(divName, div->eventMask, (uintptr_t)(div->eventFunc), win))
    {
        return 0;
    }

    if(!(divC(div, deleteFunc)))
    {
        return 0;
    }
    uint32_t success = deallocNamed(divName, win->manager);
    return success;
}