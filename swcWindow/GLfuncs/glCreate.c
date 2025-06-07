
#include "glCreate.h"

uint32_t findCurScreenNumber(Display *dis)
{
    uint32_t screenCount = ScreenCount(dis);

    Window root, child;
    uint32_t root_x, root_y, child_x, child_y, mask;
    printf("ScreenCount: %i", screenCount);
    fflush(stdout);

    for(uint32_t i = 0; i < screenCount; i++)
    {
        if(XQueryPointer(dis, RootWindow(dis, i), &root, &child, &root_x, &root_y, &child_x, &child_y, &mask))
        {
            printf("Screen Number is: %i\n", i);
            return i;
        }
    }
    return 0;
}