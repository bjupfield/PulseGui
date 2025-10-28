#include "swcBaseDivs.h"

typedef struct testDataStructure
{
    float x;
    float y;
    float z;
}testDataStructure;

uint32_t bOnLoad(swcDiv* div)
{

    /*
    
    {0.0, 1.0, 0.0},
    {-1.0, -1.0, 0.0},
    {1.0, -1.0, 0.0},
    {0.5, 1.0, 0.0},
    {1.0, -1.0, 0.0},
    {1.0, 1.0, 0.0},
    
    
    */

    gpuAlloc(36, 12, div->win);//this doesnt actually do it...

    testDataStructure *test = (testDataStructure *)updateRenderBuffer(div->layer, div->name, div->programName, div->win);
    test[0].x = 0.5;
    test[0].y = 1.0;
    test[0].z = 0.0;

    test[1].x = 1.0;
    test[1].y = -1.0;
    test[1].z = 0.0;
    
    test[2].x = 1.0;
    test[2].y = 1.0;
    test[2].z = 0.0;

    //must contian the divs layer, otherwise previous code has failed
    // layerToDivGroups temp = {.layer = div->layer};
    // temp = *((layerToDivGroups *)swcAddArray(div->win->divLayers, temp, uint32_tSorter, div->win->manager));//it should exist
    // divGroupGpu temp2 = {.programName = div->programName};
    // divGroupGpu *pointer = ((divGroupGpu*)swcAddArray(temp.divGroups, temp2, uint32_tSorter, div->win->manager));//it should exist
    // if(!pointer->cpuSideBufferObjectData)
    // {
    //     pointer->cpuSideBufferObjectData = allocNamed(sizeof(testDataStructure) * 50 + sizeof(uint32_t), div->win->manager);
    //     *((uint32_t*)retrieveName(pointer->cpuSideBufferObjectData, div->win->manager)) = 0;
    // }
    // swcNameStruct *dataName = retrieveNameL(pointer->cpuSideBufferObjectData, div->win->manager);
    // testDataStructure* data = (testDataStructure*)dataName->pointer;
    // if(dataName->size / sizeof(testDataStructure) < pointer->renderedDivs)
    // {
    //     //explode ran out of space ignoring for now... just test smh
    // }
    // else
    // {
    //     //first triangle
    //     data[pointer->renderedDivs].x = 0;
    //     data[pointer->renderedDivs].y = 0;
    //     data[pointer->renderedDivs].z = 0;
    //     pointer->renderedDivs += 1;
    //     data[pointer->renderedDivs].x = 0;
    //     data[pointer->renderedDivs].y = -1;
    //     data[pointer->renderedDivs].z = 0;
    //     pointer->renderedDivs += 1;
    //     data[pointer->renderedDivs].x = 1;
    //     data[pointer->renderedDivs].y = -1;
    //     data[pointer->renderedDivs].z = 0;
    //     pointer->renderedDivs += 1;
        
    //     //have to make three to render smh
    //     //second triangle
    //     data[pointer->renderedDivs].x = div->posx + div->dimx;
    //     data[pointer->renderedDivs].y = div->posy;
    //     data[pointer->renderedDivs].z = div->layer;
    //     pointer->renderedDivs += 1;
    //     data[pointer->renderedDivs].x = div->posx;
    //     data[pointer->renderedDivs].y = div->posy - div->dimy;
    //     data[pointer->renderedDivs].z = div->layer;
    //     pointer->renderedDivs += 1;
    //     data[pointer->renderedDivs].x = div->posx + div->dimx;
    //     data[pointer->renderedDivs].y = div->posy - div->dimy;
    //     data[pointer->renderedDivs].z = div->layer;
    //     pointer->renderedDivs += 1;
    // }

  // updateRenderBuffer(div->programName, div->layer, pointer->gpuBufferDataLocation, pointer->renderedDivs, dataName->pointer, div->win);
    return 0;
}
funcPointer baseLoad = bOnLoad;

uint32_t bDrawFunc(swcDiv* div)
{
    return 0;
}
funcPointer baseDraw = bDrawFunc;

uint32_t bResizeFunc(swcDiv* div, const XEvent* event)
{
    
    return 0;
}
resizePointer baseResize = bResizeFunc;

uint32_t bEventFunc(uint32_t* divs, uint32_t divsSize, XEvent* event, swcMemMan* manager)
{
    swcDiv *div = (swcDiv *)retrieveName(divs[0], manager);
    div->win->name = 0;


    return 0;
}
handlePointer baseEvent = bEventFunc;

uint32_t bDeleteFunc(swcDiv* div)
{
    printf("Hi, This Div (DIV: #%i) has been deleted\n\n\n", div->name);
    fflush(stdout);
    return 1;
}
funcPointer baseDeleteFunc = bDeleteFunc;