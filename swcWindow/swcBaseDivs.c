#include "swcBaseDivs.h"

uint32_t bOnLoad(swcDiv* div)
{
    printf("hi\n\n\n\nn\n\n\n\n\n\n\n\n\n\n\n");
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