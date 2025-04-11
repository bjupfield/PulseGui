#include "windowElements.h"
#include "stdlib.h"
#include "string.h"

int initializeWindow()
{
    struct theWindow mainWin = 
    {
        .divs = (winDiv*)malloc(sizeof(winDiv) * 512),
        .divArrSize = 512,
        .divCount = 0,
    };
    return 1;
}
int adjustWinSize()
{
    return 0;
}
//lazy right now
int addDiv(int posx, int posy, int width, int height)
{
    //shouldnt ever happen
    if(mainWin.divCount >= mainWin.divArrSize)
    {
        mainWin.divArrSize *= 2;
        mainWin.divs = (winDiv*)realloc(mainWin.divs, sizeof(winDiv) * mainWin.divArrSize);
    }

    winDiv* init = &mainWin.divs[mainWin.divCount];
    init->height = height;
    init->width = width;
    init->xPos = posx;
    init->yPos = posy;

    mainWin.divCount += 1;

    return 1;
}
vertexBI drawWindow()
{
    struct corner
    {
        int x;
        int y;
    };
    typedef struct corner corn;
    vertexBI ret;
    ret.elements = 6;
    ret.vertexCount = mainWin.divCount * 6;
    ret.vertexBuffer = mainWin.vBuffer;//hmmm
    //TODO:
    // to make this like analyze what actually needs to be adjusted, like flag vertexses that are wrong... this is complicated so we push it off

    ret.vertexBuffer = (float*)malloc(sizeof(float) * ret.elements * ret.vertexCount);

    float c1[3] = {1.0, 0.0, 0.0};
    float c2[3] = {0.0, 1.0, 0.0};
    float c3[3] = {0.0, 0.0, 1.0};

    for(int i = 0; i < ret.vertexCount; i++)
    {
        winDiv drawDiv = mainWin.divs[i];  
        corn leftU = 
        {
            .x = drawDiv.xPos,
            .y = drawDiv.yPos
        };
        corn rightU = 
        {
            .y = leftU.y,
            .x = drawDiv.width + leftU.x,
        };
        corn leftL = 
        {
            .y = leftU.y + drawDiv.height,
            .x = leftU.x,
        };
        corn rightL = 
        {
            .y = leftL.y,
            .x = rightU.x,
        };

        //ll corner t1
        float horror[3] = {leftL.x, leftL.y, 0.0 };
        memcpy((float*)ret.vertexBuffer + i * ret.elements, horror, 12);
        memcpy((float*)ret.vertexBuffer + i * ret.elements + 3, c1, 12);

        //ul corner t1
        float horror[3] = {leftU.x, leftU.y, 0.0};
        memcpy((float*)ret.vertexBuffer + (i + 1) * ret.elements, horror, 12);
        memcpy((float*)ret.vertexBuffer + (i + 1) * ret.elements + 3, c2, 12);

        //ur corner t1
        float horror[3] = {rightU.x, rightU.y, 0.0};
        memcpy((float*)ret.vertexBuffer + (i + 2) * ret.elements, horror, 12);
        memcpy((float*)ret.vertexBuffer + (i + 2) * ret.elements + 3, c3, 12);

        //ll corner t2
        float horror[3] = {leftL.x, leftL.y, 0.0};
        memcpy((float*)ret.vertexBuffer + (i + 3) * ret.elements, horror, 12);
        memcpy((float*)ret.vertexBuffer + (i + 3) * ret.elements + 3, c1, 12);

        //lr corner t2
        float horror[3] = {rightL.x, rightL.y, 0.0};
        memcpy((float*)ret.vertexBuffer + (i + 4) * ret.elements, horror, 12);
        memcpy((float*)ret.vertexBuffer + (i + 4) * ret.elements + 3, c2, 12);

        //ur corner t2
        float horror[3] = {rightU.x, rightU.y, 0.0};
        memcpy((float*)ret.vertexBuffer + (i + 5) * ret.elements, horror, 12);
        memcpy((float*)ret.vertexBuffer + (i + 5) * ret.elements + 3, c3, 12);
        //i need to plan this...
    }

    return ret;
}
