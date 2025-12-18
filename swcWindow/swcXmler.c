#include "swcXmler.h"

#define URLMaxLength 1024
#define nameMaxLength 32
typedef struct programObject
{
    uint8_t shaderType;
    char name[nameMaxLength];
    char URL[URLMaxLength];
}programObject;

uint32_t textNodeChildCpy(char *restrict dest, uint32_t maxLength, const xmlNode* parent);
uint32_t schemaValidate(xmlDoc* doc);

/**
 * @brief Inteprets programsObjects.xml and passes to GL(tobenamed) to save program objects to a table with
 * assigned names
 * 
 * @param win 
 * @return uint32_t 
 */
uint32_t programObjectsXML(swcWin* win)
{
    xmlDoc *doc = xmlReadFile("swcWindow/swcXml/programObjects.xml", "", 0);

    #ifdef DEBUG

    if(schemaValidate(doc))
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
    struct funcs
    {
        char draw[nameMaxLength];
        char delete[nameMaxLength];
        char event[nameMaxLength];
        char load[nameMaxLength];
        char resize[nameMaxLength];
    };
    struct pipeline
    {
        
    };
}divObjectDef;

/**
 * @brief Interprets XML file and passes to GL(tobenamed) that instantiates pipelines and saves the object defaults somewhere
 * 
 * @param win 
 * @return uint32_t 
 */
uint32_t divObjectDefXML(swcWin* win)
{
    xmlDoc *doc = xmlReadFile("swcWindow/swcXml/divOrObjects.xml", "", 0);

    #ifdef DEBUG

    if(schemaValidate(doc))
    {
        xmlFreeDoc(doc);
        return 0;
    }

    #endif


    xmlNode *node = xmlDocGetRootElement(doc);
    node = xmlFirstElementChild(node);
    xmlNode *children;
    char *name = allocSB(sizeof(char) * nameMaxLength, win->manager);
    while(node != NULL)
    {
        children = xmlFirstElementChild(node);
        if(children != NULL &&  (char)(*(children->name)) == 'N')
        {
            if(textNodeChildCpy(name, nameMaxLength, children))
            {
                children = xmlNextElementSibling(children);

            }
        }
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
    if(child != NULL && child->type == XML_TEXT_NODE && strlen(child->content) < maxLength - 1)
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