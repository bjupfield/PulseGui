#include "swcXmler.h"

uint32_t confused()
{
    printf("hi");
    // xml
    xmlDoc *doc = xmlReadFile("fake", "fake", 0);
    return 1;
}