#include "swcXmler.h"

typedef struct programObject
{
    uint8_t shaderType;
    char name[nameMaxLength];
    char URL[URLMaxLength];
}programObject;

uint32_t textNodeChildCpy(char *restrict dest, uint32_t maxLength, const xmlNode* parent);
uint32_t schemaValidate(xmlDoc* doc);
uint32_t startupPass(swcWin *win, xmlNode *rootNode, char *parentChildAssociation, uint32_t pointerMathAdder);

/**
 * @brief Inteprets programsObjects.xml and passes to GL(tobenamed) to save program objects to a table with
 * assigned names
 * 
 * @param win 
 * @return 0 if Unable To interpret file 1 Otherwise, all non-interprable lines flagged for error func
 */
uint32_t programObjectsXML(swcWin* win)
{
    xmlDoc *doc = xmlReadFile("swcWindow/swcXml/programObjects.xml", "", 0);

    #ifdef DEBUG

    if(!schemaValidate(doc))
    {
        xmlFreeDoc(doc);
        return 0;
    }
    
    #endif


    xmlNode *node = xmlDocGetRootElement(doc);
    node = xmlFirstElementChild(node);
    xmlNode *children;
    while(node != NULL)
    {
        children = xmlFirstElementChild(node);
        programObject* tempProgram = allocSB(sizeof(programObject), win->manager);
        if(children != NULL && (char)(*(children->name)) == 'N')
        {
            if(textNodeChildCpy(tempProgram->name, nameMaxLength, children))//name
            {
                children = xmlNextElementSibling(children);
                uint32_t curLength = 0;
                while(children != NULL && (char)(*(children->name)) == 'U' && curLength != URLMaxLength - 1)
                {//urls
                    if(curLength > 0)
                    {
                        //whitespace added between program urls to distinguish
                        curLength += 1;
                        strcat(tempProgram->URL, " ");
                    }
                    if(!textNodeChildCpy(tempProgram->URL, URLMaxLength - curLength, children))//copy textnode into url
                    {
                            //alert error at line
                            curLength = 0;
                            break;
                    }
                    curLength = strlen(tempProgram->URL);
                    children = xmlNextElementSibling(children);
                }
                if(curLength != 0)
                {
                    switch((char)(*(children->name)))
                    {//
                        case 'T':
                        {
                            tempProgram->shaderType = GL_TESS_CONTROL_SHADER_BIT;
                            break;
                        }
                        case 'E':
                        {
                            tempProgram->shaderType = GL_TESS_EVALUATION_SHADER_BIT;
                            break;
                        }
                        case 'G':
                        {
                            tempProgram->shaderType = GL_GEOMETRY_SHADER_BIT;
                            break;
                        }
                        case 'V':
                        {
                            tempProgram->shaderType = GL_VERTEX_SHADER_BIT;
                            break;
                        }
                        case 'F':
                        {
                            tempProgram->shaderType = GL_FRAGMENT_SHADER_BIT;
                            break;
                        }
                        case 'C':
                        {
                            tempProgram->shaderType = GL_COMPUTE_SHADER_BIT;
                            break;
                        }
                        default :
                        {
                            tempProgram->shaderType = 0;
                            //alert error at line
                            continue;
                        }
                    }
                }
                if(tempProgram->shaderType != 0)
                {
                    //pass to gl program instantiator
                }
            }
        }
        node = xmlNextElementSibling(node);
    }
    
    xmlFreeDoc(doc);
    return 1;
}

typedef struct divObjectDef
{
    char name[nameMaxLength];
    char parent[nameMaxLength];
    struct pipeline
    {
        char TCS[nameMaxLength];
        char TES[nameMaxLength];
        char GS[nameMaxLength];
        char VS[nameMaxLength];
        char FS[nameMaxLength];
        char CS[nameMaxLength];
    }pipeline;
}divObjectDef;

/*
* This Func introduces the necessity for a c++ style function table system,
* for some reason i am quite determined to do this in c only, so obviously here is the system
* that a func table or dynamic func loading would work. 
* First a precompiler phase would be needed that collects all funcs and stores them in a func table 
* by assigning the funcs to func pointers and associating them with a name (likely the function identifier)
* and any other miscellaneous information, like their return type.
* Second, this one's kind of obvious but of course every object needs its own constructor, that looks at
* this func table, compares it to the function names provided to it by the below function, and than inserts the
* function into its own struct by whatever means the constructor decrees.
* Pretty simple, but i just needed to type it so I would stop thinking of it...
* Third, (maybe), I guess to prevent unnecessary object loading, which would occur if only the first two steps
* were followed, because the "loaded" object defs would have to be held in uncompressed memory somewhere, what would
* actually need to happen would be another precompilation phase where a constructor is for each objectdef is
* created and than this is used for object instantiation, which would prevent a dummy object from having to be
* spawned to hold everything in memory, which yeah, thats pretty much c++, I mean the functionality and dynanism,
* is similar to c++, but the functionality that I am more directly imitating with a seperate file being used
* to load different functions into different object defs is that of game engines like UE, Unity, Godot, which
* I imagine use a similar process to achieve the same functionality, creating "ghost classes" to serve 
* for objects with dynamically loaded functions 
*/

/**
 * @brief Interprets XML file and passes to GL(tobenamed) that instantiates pipelines and saves the object defaults somewhere
 * 
 * @param win 
 * @return 0 if Unable To interpret file 1 Otherwise, all non-interprable lines flagged for error func
 */
uint32_t divObjectDefXML(swcWin* win)
{
    xmlDoc *doc = xmlReadFile("swcWindow/swcXml/divOrObjects.xml", "", 0);

    #ifdef DEBUG

    if(!schemaValidate(doc))
    {
        xmlFreeDoc(doc);
        return 0;
    }

    #endif

    xmlNode *node = xmlDocGetRootElement(doc);
    node = xmlFirstElementChild(node);
    xmlNode *children;
    xmlNode *secondChild;
    xmlNode *thirdChild;
    char *funcs;
    /*
    * This is an array of type "name funcpointer name funcpointer name funcpointer"
    */
    uint32_t funcCount;
    uint32_t curFunc;
    char *packedData;
    /*
    * This is an array of type, "byteIdentifier datesizeofbyteidentifier"
    * where the byteIdentifier is the enum listed in the header file
    * strings will have a length attached before their datasize like
    * ex: "stringbyteidentifier stringsize string"
    * with the string size being a uint32_t
    */
    uint32_t packedDataSize = 0;
    uint32_t packedDataPos = 0;
    uint64_t evilMemcpy;
    divObjectDef *tempDivObjectDef;
    while(node != NULL)
    {
        tempDivObjectDef = allocSB(sizeof(divObjectDef), win->manager);//remember to deallocate in instatiator func
        children = xmlFirstElementChild(node);
        if(children != NULL &&  (char)(*(children->name)) == 'N')
        {
            if(!textNodeChildCpy(tempDivObjectDef->name, nameMaxLength, children))
            {
                //alert error at line
                node = xmlNextElementSibling(node);
                continue;
            }
        }
        children = xmlNextElementSibling(children);
        if(children != NULL && (char)(*(children->name)) == 'P')
        {
            if(!textNodeChildCpy(tempDivObjectDef->parent, nameMaxLength, children))
            {
                node = xmlNextElementSibling(node);
                continue;
            }
            children = xmlNextElementSibling(children);
        }
        if(children != NULL && (char)(*(children->name) == 'F'))
        {
            secondChild = xmlFirstElementChild(children);
            funcCount = xmlChildElementCount(children);
            if(funcCount > 0)
            {
                funcs = allocSB(sizeof(char) * funcCount * nameMaxLength * 2, win->manager);
                curFunc = 0;
                //allocate maxname size * 2, for name and function pointer (name being like draw func or hitboxfunc)
                while(secondChild != NULL && (char)(*(secondChild->name)) == 'F')
                {
                    thirdChild = xmlFirstElementChild(secondChild);
                    secondChild = xmlNextElementSibling(secondChild);
                    if(thirdChild != NULL && (char)(*(thirdChild->name)) == 'N')
                    {
                        if(!textNodeChildCpy(funcs + (curFunc * nameMaxLength * 2) * sizeof(char), nameMaxLength, thirdChild))
                        {
                            curFunc++;
                            continue;
                        }

                    }
                    thirdChild = xmlNextElementSibling(thirdChild);
                    if(thirdChild != NULL && (char)(*(thirdChild->name)) == 'F')
                    {
                        textNodeChildCpy(funcs + (curFunc++ * nameMaxLength * 2 + nameMaxLength) * sizeof(char), nameMaxLength, thirdChild);
                    }
                }
            }
            children = xmlNextElementSibling(children);
        }
        if(children != NULL && (char)(*(children->name)) == 'P')
        {
            secondChild = xmlFirstElementChild(children);
            while(secondChild != NULL)
            {
                switch((char)(*(secondChild->name)))
                {
                    case 'T':
                    {
                        textNodeChildCpy(tempDivObjectDef->pipeline.TCS, nameMaxLength, secondChild);
                        break;
                    }
                    case 'E':
                    {
                        textNodeChildCpy(tempDivObjectDef->pipeline.TES, nameMaxLength, secondChild);
                        break;
                    }
                    case 'G':
                    {
                        textNodeChildCpy(tempDivObjectDef->pipeline.GS, nameMaxLength, secondChild);
                        break;
                    }
                    case 'V':
                    {
                        textNodeChildCpy(tempDivObjectDef->pipeline.VS, nameMaxLength, secondChild);
                        break;
                    }
                    case 'F':
                    {
                        textNodeChildCpy(tempDivObjectDef->pipeline.FS, nameMaxLength, secondChild);
                        break;
                    }
                    case 'C':
                    {
                        textNodeChildCpy(tempDivObjectDef->pipeline.CS, nameMaxLength, secondChild);
                        break;
                    }
                    default:
                    {
                        //alert error at line
                        //secondChild->line
                        break;
                    }
                }
                secondChild = xmlNextElementSibling(secondChild);
            }
        }
        children = xmlNextElementSibling(children);
        if(children != NULL && (char)(*(children->name)) == 'C')
        {
            //first pass
            secondChild = xmlFirstElementChild(children);
            while(secondChild != NULL)
            {
                switch((char)(*(secondChild->name)))
                {
                    case 'u':
                    //intended no break
                    case 'i':
                    {
                        packedDataSize += char_to_ui8;
                        if(xmlStrEqual(secondChild->name, "uint8_t"))
                        {
                            packedDataSize += char_to_ui8;
                        }
                        else if(xmlStrEqual(secondChild->name, "uint16_t"))
                        {
                            packedDataSize += char_to_ui16;
                        }
                        else if(xmlStrEqual(secondChild->name, "uint32_t"))
                        {
                            packedDataSize += char_to_ui32;
                        }
                        else if(xmlStrEqual(secondChild->name, "uint64_t"))
                        {
                            packedDataSize += char_to_ui64;
                        }
                        else if(xmlStrEqual(secondChild->name, "int8_t"))
                        {
                            packedDataSize += char_to_i8;
                        }
                        else if(xmlStrEqual(secondChild->name, "int16_t"))
                        {
                            packedDataSize += char_to_i16;
                        }
                        else if(xmlStrEqual(secondChild->name, "int32_t"))
                        {
                            packedDataSize += char_to_i32;
                        }
                        else if(xmlStrEqual(secondChild->name, "int64_t"))
                        {
                            packedDataSize += char_to_i64;
                        }
                        break;
                    }
                    case 's':
                    {
                        thirdChild = secondChild->children;
                        if(thirdChild != NULL)
                            packedDataSize += strlen(thirdChild->content) + char_to_ui32 + char_to_ui8;
                        break;
                    }
                    default :
                    {
                        break;
                    }
                }

                secondChild = xmlNextElementSibling(secondChild);
            }
            if(packedDataSize == 0)
            {
                //pass 
                //divobjectDef
                //funcs //check if funccount is 0 send null if
                //funccount
                //null
                //0
                //to gl instatiator
                node = xmlNextElementSibling(node);
                continue;
            }
            packedData = allocSB(packedDataSize, win->manager);
            
            //second pass
            secondChild = xmlFirstElementChild(children);
            while(secondChild != NULL)
            {
                thirdChild = secondChild->children;
                if(thirdChild != NULL)
                {
                    switch((char)(*(secondChild->name)))
                    {
                        case 'i':
                        //intended no break
                        case 'u':
                        {
                            if(xmlStrEqual(secondChild->name, "uint8_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_UI8);
                                *(packedData + packedDataPos + sizeof(char)) = atoi(thirdChild->content);
                                packedDataPos += char_to_ui8;
                            }
                            else if(xmlStrEqual(secondChild->name, "uint16_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_UI16);
                                evilMemcpy = atoi(thirdChild->content);
                                memcpy((packedData + packedDataPos + char_to_ui8), &evilMemcpy, sizeof(uint16_t));
                                packedDataPos += char_to_ui16;
                            }
                            else if(xmlStrEqual(secondChild->name, "uint32_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_UI32);
                                evilMemcpy = atoi(thirdChild->content);
                                memcpy((packedData + packedDataPos + char_to_ui8), &evilMemcpy, sizeof(uint32_t));
                                packedDataPos += char_to_ui32;
                            }
                            else if(xmlStrEqual(secondChild->name, "uint64_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_UI64);
                                evilMemcpy = atol(thirdChild->content);
                                memcpy((packedData + packedDataPos + char_to_ui8), &evilMemcpy, sizeof(uint64_t));
                                packedDataPos += char_to_ui64;
                            }
                            else if(xmlStrEqual(secondChild->name, "int8_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_I8);
                                *(packedData + packedDataPos + sizeof(char)) = atoi(thirdChild->content);
                                packedDataPos += char_to_i8;
                            }
                            else if(xmlStrEqual(secondChild->name, "int16_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_I16);
                                evilMemcpy = atoi(thirdChild->content);
                                memcpy((packedData + packedDataPos + char_to_ui8), &evilMemcpy, sizeof(uint16_t));
                                packedDataPos += char_to_i16;
                            }
                            else if(xmlStrEqual(secondChild->name, "int32_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_I32);
                                evilMemcpy = atoi(thirdChild->content);
                                memcpy((packedData + packedDataPos + char_to_ui8), &evilMemcpy, sizeof(uint32_t));
                                packedDataPos += char_to_i32;
                            }
                            else if(xmlStrEqual(secondChild->name, "int64_t"))
                            {
                                *(packedData + packedDataPos) = (unsigned char)(BI_I64);
                                evilMemcpy = atol(thirdChild->content);
                                memcpy((packedData + packedDataPos + char_to_ui8), &evilMemcpy, sizeof(uint64_t));
                                packedDataPos += char_to_i64;
                            }
                            packedDataPos += char_to_ui8;
                            break;
                        }
                        case 's':
                        {
                            *(packedData + packedDataPos) = (unsigned char)(BI_S);
                            packedDataPos += char_to_ui8;
                            evilMemcpy = strlen(thirdChild->content);
                            memcpy((packedData + packedDataPos), &evilMemcpy, sizeof(uint32_t));
                            packedDataPos += char_to_ui32;
                            textNodeChildCpy((packedData + packedDataPos), evilMemcpy, secondChild);
                            packedDataPos += evilMemcpy;
                            break;
                        }
                        default :
                        {
                            break;
                        }
                    }
                }
                secondChild = xmlNextElementSibling(secondChild);
            }
            packedDataPos = 0;
        }
        //pass 
        //divobjectDef
        //funcs //check if funccount is 0 send null if
        //funccount
        //packeddata
        //packeddatasize
        //to gl program instatiator
        node = xmlNextElementSibling(node);
    }
    // xmlFreeNode(thirdChild);
    // xmlFreeNode(secondChild);
    // if(children != NULL)
    //     xmlFreeNode(children);//attempting to free the above crashes the program... don't know why
    xmlFreeNode(node);
    xmlFreeDoc(doc);
    return 1;
}

/**
 * @brief Interprets startup XML file and passes to GL(tobenamed) that creates objects on start through reading the object defaults
 * and constructors created through the divOrObjects XML
 * 
 * @param win 
 * @return uint32_t 
 */
uint32_t startupXML(swcWin *win)
{

    FILE *file = fopen("swcWindow/swcXml/startup.xml", "r");
    fseek(file, 0, SEEK_END);
    uint32_t fileLength = ftell(file);
    xmlDoc *doc = xmlReadFile("swcWindow/swcXml/startup.xml", "", 0);

    #ifdef DEBUG

    if(!schemaValidate(doc))
    {
        xmlFreeDoc(doc);
        return 0;
    }

    #endif

    xmlNode *node = xmlDocGetRootElement(doc);
    char* parentChildAssociation = callocSB(fileLength * sizeof(uint64_t), win->manager);
    if(node!= NULL)
    {
        return startupPass(win, node, parentChildAssociation, 0);
    }
    return 0;
}

/**
 * @brief attempting to optimize wtih tail recursion in mind, 
 * refer to gnu optimize options for even the possibility of optimization,
 * 
 * @param win 
 * @param rootNode Parent Node of Either InstDivs or Children
 * @return uint32_t 
 */
uint32_t startupPass(swcWin *win, xmlNode *rootNode, char *parentChildAssociation, uint32_t pointerMathAdder)
{
    
    xmlNode *child = xmlFirstElementChild(rootNode);
    xmlNode *secondChild;
    xmlNode *thirdChild;
    uint32_t maybeThisShouldGoInItsOwnScope;
    xmlObjectData *curXmlObjectData = (xmlObjectData *)(parentChildAssociation + pointerMathAdder);
    uint32_t childCount;
    uint64_t *data;
    uint32_t siblingNum = 0;
    while(child != NULL)
    {
        // while(*(curXmlObjectData->name) != 0)
        // {
            
        //     maybeThisShouldGoInItsOwnScope =  *((uint32_t *)(curXmlObjectData->childrenAndChildData)) * sizeof(uint64_t) + sizeof(uint32_t);
        //     pointerMathAdder += (*(uint64_t *)(curXmlObjectData->childrenAndChildData + maybeThisShouldGoInItsOwnScope)) * sizeof(uint64_t) + sizeof(xmlObjectData) + maybeThisShouldGoInItsOwnScope;
        //     curXmlObjectData = (xmlObjectData *)(parentChildAssociation + pointerMathAdder);//should be the space assigned to the current temporary object for memory storage
        // }
        secondChild = xmlFirstElementChild(child);
        if(secondChild != NULL && *(secondChild->name) == 'N')
        {
            textNodeChildCpy(curXmlObjectData->name, nameMaxLength, secondChild);
        }
        secondChild = xmlNextElementSibling(secondChild);
        if(secondChild != NULL && *(secondChild->name) == 'T')
        {
            thirdChild = xmlFirstElementChild(secondChild);
            if(!xmlStrEqual(thirdChild->name, "PosX") && thirdChild->children != NULL)
            {
                //alert error at line
                return 0;
            }
            curXmlObjectData->transform.Pos.X =  atoi(thirdChild->children->content);
            
            thirdChild = xmlNextElementSibling(thirdChild);
            if(!xmlStrEqual(thirdChild->name, "PosY") && thirdChild->children != NULL)
            {
                //alert error at line
                return 0;
            }
            curXmlObjectData->transform.Pos.Y =  atoi(thirdChild->children->content);

            thirdChild = xmlNextElementSibling(thirdChild);
            if(!xmlStrEqual(thirdChild->name, "PosZ") && thirdChild->children != NULL)
            {
                //alert error at line
                return 0;
            }
            curXmlObjectData->transform.Pos.Z =  atoi(thirdChild->children->content);

            thirdChild = xmlNextElementSibling(thirdChild);
            if(!xmlStrEqual(thirdChild->name, "ScaleX") && thirdChild->children != NULL)
            {
                //alert error at line
                return 0;
            }
            curXmlObjectData->transform.Scale.X =  atoi(thirdChild->children->content);

            thirdChild = xmlNextElementSibling(thirdChild);
            if(!xmlStrEqual(thirdChild->name, "ScaleY") && thirdChild->children != NULL)
            {
                //alert error at line
                return 0;
            }
            curXmlObjectData->transform.Scale.Y =  atoi(thirdChild->children->content);

            thirdChild = xmlNextElementSibling(thirdChild);
            if(!xmlStrEqual(thirdChild->name, "ScaleZ") && thirdChild->children != NULL)
            {
                //alert error at line
                return 0;
            }
            curXmlObjectData->transform.Scale.Z =  atoi(thirdChild->children->content);
            secondChild = xmlNextElementSibling(secondChild);
        }
        pointerMathAdder += sizeof(xmlObjectData);
        if(secondChild != NULL && *(secondChild->name) == 'C')
        {
            childCount = xmlChildElementCount(secondChild);
            secondChild = xmlNextElementSibling(secondChild);
        }
        else
        { 
            childCount = 0;
        }
        pointerMathAdder += childCount * sizeof(uint64_t) + sizeof(uint32_t);
        *((uint32_t *)(curXmlObjectData->childrenAndChildData)) = childCount;
        if(secondChild != NULL && *(secondChild->name) == 'D')
        {
            *(uint32_t *)(curXmlObjectData->childrenAndChildData + childCount * sizeof(uint64_t) + sizeof(uint32_t)) = xmlChildElementCount(secondChild);
            data = (uint64_t *)(curXmlObjectData->childrenAndChildData + childCount * sizeof(uint64_t) + sizeof(uint32_t) * 2);
            pointerMathAdder += (*(((uint32_t*)data) - 1) * sizeof(uint64_t)) + sizeof(uint32_t);   
            thirdChild = xmlFirstElementChild(secondChild);
            while(thirdChild != NULL && thirdChild->children != NULL)
            {
                switch(*(thirdChild->name))
                {
                    case 'u' :
                    {
                        switch(thirdChild->name[4])
                        {
                            case '8' :
                            //intended no break
                            case '1' :
                            //intended no break
                            case '3' :
                            {
                                *data = atoi(thirdChild->children->content);
                                break;
                            }
                            case '6' :
                            {
                                *data = atol(thirdChild->children->content);
                                break;
                            }
                            default :
                            {
                                //error at line
                                break;
                            }
                        }
                        break;
                    }
                    case 'i' :
                    {
                        switch(thirdChild->name[3])
                        {
                            case '8' :
                            //intended no break
                            case '1' :
                            //intended no break
                            case '3' :
                            {
                                *data = atoi(thirdChild->children->content);
                                break;
                            }
                            case '6' :
                            {
                                *data = atol(thirdChild->children->content);
                                break;
                            }
                            default :
                            {
                                //error at line
                                break;
                            }
                        }
                        break;
                    }
                    case 's' :
                    {
                        *data = (uint64_t)allocSB(strlen(thirdChild->children->content), win->manager);
                        strcpy((char *)(*data), thirdChild->children->content);
                        break;
                    }
                    default :
                    {
                        //error at line
                        break;
                    }
                }
                data++;
                thirdChild = xmlNextElementSibling(thirdChild);
            }
        }
        if(curXmlObjectData != (xmlObjectData *)parentChildAssociation)
        {//means working in a child, add this child to its parent
        //look at recursive call to understand this
            *((uint64_t *)(parentChildAssociation + sizeof(xmlObjectData) + sizeof(uint32_t) + siblingNum * sizeof(uint64_t))) = (uint64_t)curXmlObjectData;
        }
        //pass 
        //curXmlObjectData
        //to object constructer factory thing
        siblingNum++;
        child = xmlNextElementSibling(child);
    } 
    if(*(rootNode->name) == 'C')
    {//in a child data, meaning that a parent has been completely filled, push pointer forward past parent
        parentChildAssociation += sizeof(xmlObjectData);
        parentChildAssociation += *((uint32_t *)parentChildAssociation) * sizeof(uint64_t) + sizeof(uint32_t);
        parentChildAssociation += *((uint32_t *)parentChildAssociation) * sizeof(uint64_t) + sizeof(uint32_t);
    }
    while(parentChildAssociation != 0 && *parentChildAssociation != 0 && *((uint32_t *)(parentChildAssociation + sizeof(xmlObjectData))) == 0)
    {
        //skip over node with no children
        parentChildAssociation += sizeof(xmlObjectData) + 2 * sizeof(uint32_t) + (*((uint32_t*)(parentChildAssociation + sizeof(xmlObjectData) + sizeof(uint32_t))) * sizeof(uint64_t));
    }
    if(*parentChildAssociation == 0)
    {
        return 1;
    }
    else
    {
        child = xmlFirstElementChild(rootNode);
        while(child != NULL)
        {
            secondChild = xmlFirstElementChild(child);
            if(secondChild != NULL && *secondChild->name == 'N')
            {
                if(xmlStrEqual(secondChild->children->content, ((xmlObjectData *)parentChildAssociation)->name))
                {
                    secondChild = xmlNextElementSibling(secondChild);
                    if(secondChild != NULL && *secondChild->name == 'T')
                    {
                        secondChild = xmlNextElementSibling(secondChild);
                    }
                    if(secondChild != NULL && *secondChild->name == 'C')
                    {
                        rootNode = secondChild;
                        break;
                    }//else alert error
                } 
            }
            secondChild = xmlNextElementSibling(secondChild);
            if(secondChild != NULL && *secondChild->name == 'T')
            {
                secondChild = xmlNextElementSibling(secondChild);
            }
            if(secondChild != NULL && *secondChild->name == 'C')
            {
                secondChild = xmlFirstElementChild(secondChild);
                while(secondChild != NULL)
                {
                    thirdChild = xmlFirstElementChild(secondChild);
                    if(thirdChild != NULL && *thirdChild->name == 'N')
                    {
                        if(xmlStrEqual(thirdChild->children->content, ((xmlObjectData *)parentChildAssociation)->name))
                        {
                            thirdChild = xmlNextElementSibling(thirdChild);
                            if(thirdChild != NULL && *thirdChild->name == 'T')
                            {
                                thirdChild = xmlNextElementSibling(thirdChild);
                            }
                            if(thirdChild != NULL && *thirdChild->name == 'C')
                            {
                                rootNode = thirdChild;
                                break;
                            }//else alert error
                        }
                    }
                    secondChild = xmlNextElementSibling(secondChild);
                }
                if(child == secondChild)
                {
                    break;
                }
            }
            child = xmlNextElementSibling(child);
        }
        return startupPass(win, rootNode, parentChildAssociation, pointerMathAdder);
    }
    
}
/**
 * @brief 
 * 
 * @param dest String Contaier to copy string into 
 * @param parent The Node Who has a child with a string to extract
 * @return 1 on Success | 0 on Failure, which can occur due to 3 different things
 * 1. Child Not Existing
 * 2. Child not being a TEXT_NODE
 * 3. Child string being beyond the length of the provided maxlength, with maxlength being assumed to be the total string container, including the null terminating character
 */
uint32_t textNodeChildCpy(char *restrict dest, uint32_t maxLength, const xmlNode* parent)
{
    xmlNode* child = parent->children;
    if(child != NULL && child->type == XML_TEXT_NODE && strlen(child->content) <= maxLength)
    {
        strcpy(dest, child->content);
    }
    else
    {
        //maybe send error to file
        //child->line
        return 0;
    }

    return 1;
}
/**
 * @brief Returns 1 if Document is Considered Valid Xml document in comparison to schema
 * 
 * @param doc 
 * @return uint32_t 
 */
uint32_t schemaValidate(xmlDoc* doc)
{
    //check for valid schema
    xmlSchemaParserCtxt *schemaCheck = xmlSchemaNewParserCtxt("swcWindow/swcXml/schema.xsd");
    xmlSchema *schema = xmlSchemaParse(schemaCheck);
    xmlSchemaValidCtxt *validationtest = xmlSchemaNewValidCtxt(schema);
    uint32_t errorCode = xmlSchemaValidateDoc(validationtest, doc);

    xmlSchemaFreeParserCtxt(schemaCheck);
    xmlSchemaFree(schema);
    xmlSchemaFreeValidCtxt(validationtest);

    if(errorCode)
    {
        //alert error
        return 0;
    }
    return 1;
}