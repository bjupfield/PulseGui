#ifndef SWCXMLER_H
#define SWCXMLER_H

#include "swcDef.h"
#include "swcMem.h"
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlschemas.h>

//look i know this isnt seriously needed but im already doing something ridiculous
#define char_to_ui8 sizeof(uint8_t) / sizeof(char)
#define char_to_ui16 sizeof(uint16_t) / sizeof(char)
#define char_to_ui32 sizeof(uint32_t) / sizeof(char)
#define char_to_ui64 sizeof(uint64_t) / sizeof(char)
#define char_to_i8 sizeof(int8_t) / sizeof(char)
#define char_to_i16 sizeof(int16_t) / sizeof(char)
#define char_to_i32 sizeof(int32_t) / sizeof(char)
#define char_to_i64 sizeof(int64_t) / sizeof(char)

#define URLMaxLength 1024
#define nameMaxLength 32

enum bitIdentifier
{
    BI_UI8,
    BI_UI16,
    BI_UI32,
    BI_UI64,
    BI_I8,
    BI_I16,
    BI_I32,
    BI_I64,
    BI_S,
};
typedef struct transform 
{
    struct Pos 
    {
        uint32_t X;
        uint32_t Y;
        uint32_t Z;
    }Pos;
    struct Scale
    {
        uint32_t X;
        uint32_t Y;
        uint32_t Z;
    }Scale;
    /*
    Mystical future rotational info....
    union Rotation
    {
        struct Euler
        {
            uint32_t X;
            uint32_t Y;
            uint32_t Z;
        }Euler;
        struct Quaternion
        {
            uint32_t X;
            uint32_t Y;
            uint32_t Z;
            uint32_t W;
        }Quaternion;
    }Rotation;
     */
     
}transform;

typedef struct xmlObjectData
{
    char name[nameMaxLength];
    transform transform;
    char childrenAndChildData[];
    //first 32bits are the children count, after are children pointers, each being 64bits
    //after are miscellaneous data, starting with mscellaneous data count
}xmlObjectData;


uint32_t programObjectsXML(swcWin* win);
uint32_t divObjectDefXML(swcWin* win);
uint32_t startupXML(swcWin *win);

#endif