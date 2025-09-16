#include "swcBaseDivs.h"

typedef struct testDataStructure
{
    float x;
    float y;
    float z;
}testDataStructure;

uint32_t bOnLoad(swcDiv* div)
{
    printf("hi\n\n\n\nn\n\n\n\n\n\n\n\n\n\n\n");

    //must contian the divs layer, otherwise previous code has failed
    layerToProgram temp = {.layer = div->layer};
    temp = *((layerToProgram *)swcAddArray(div->win->divLayers, temp, nameToDivSorter, div->win->manager));//it should exist
    nameToDiv temp2 = {.programName = div->programName};
    nameToDiv *pointer = ((nameToDiv*)swcAddArray(temp.programGroups, temp2, nameToDivSorter, div->win->manager));//it should exist
    if(pointer->cpuSideBufferObjectData == NULL)
    {
        pointer->cpuSideBufferObjectData = allocNamed(sizeof(testDataStructure) * 50 + sizeof(uint32_t), div->win->manager);
        *((uint32_t*)retrieveName(pointer->cpuSideBufferObjectData, div->win->manager)) = 0;
    }
    swcNameStruct *dataName = retrieveNameL(pointer->cpuSideBufferObjectData, div->win->manager);
    testDataStructure* data = (testDataStructure*)dataName->pointer;
    if(dataName->size / sizeof(testDataStructure) < pointer->cpuBufferObjectDataElementCount)
    {
        //explode ran out of space ignoring for now... just test smh
    }
    else
    {
        //first triangle
        data[pointer->cpuBufferObjectDataElementCount].x = 0;
        data[pointer->cpuBufferObjectDataElementCount].y = 0;
        data[pointer->cpuBufferObjectDataElementCount].z = 0;
        pointer->cpuBufferObjectDataElementCount += 1;
        data[pointer->cpuBufferObjectDataElementCount].x = 0;
        data[pointer->cpuBufferObjectDataElementCount].y = -1;
        data[pointer->cpuBufferObjectDataElementCount].z = 0;
        pointer->cpuBufferObjectDataElementCount += 1;
        data[pointer->cpuBufferObjectDataElementCount].x = 1;
        data[pointer->cpuBufferObjectDataElementCount].y = -1;
        data[pointer->cpuBufferObjectDataElementCount].z = 0;
        pointer->cpuBufferObjectDataElementCount += 1;
        
        //have to make three to render smh
        //second triangle
        data[pointer->cpuBufferObjectDataElementCount].x = div->posx + div->dimx;
        data[pointer->cpuBufferObjectDataElementCount].y = div->posy;
        data[pointer->cpuBufferObjectDataElementCount].z = div->layer;
        pointer->cpuBufferObjectDataElementCount += 1;
        data[pointer->cpuBufferObjectDataElementCount].x = div->posx;
        data[pointer->cpuBufferObjectDataElementCount].y = div->posy - div->dimy;
        data[pointer->cpuBufferObjectDataElementCount].z = div->layer;
        pointer->cpuBufferObjectDataElementCount += 1;
        data[pointer->cpuBufferObjectDataElementCount].x = div->posx + div->dimx;
        data[pointer->cpuBufferObjectDataElementCount].y = div->posy - div->dimy;
        data[pointer->cpuBufferObjectDataElementCount].z = div->layer;
        pointer->cpuBufferObjectDataElementCount += 1;
    }

    updateRenderBuffer(div->win, pointer->vertexBufferObjectName, pointer->gpuBufferDataSize, pointer->cpuBufferObjectDataElementCount, div->programName, div->layer, data);
    return 0;
}
funcPointer baseLoad = bOnLoad;

uint32_t bDrawFunc(swcDiv* div)
{
    return 0;
}
funcPointer baseDraw = bDrawFunc;

uint32_t bResizeFunc(swcDiv* div, uint32_t x, uint32_t y)
{
    printf("%i, %i\n", x, y);
    return 0;
}
resizePointer baseResize = bResizeFunc;

uint32_t bEventFunc(uint32_t* divs, uint32_t divsSize, XEvent* event)
{
    printf("HELLO I AM HERE\n");
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