#include "swcXmler.h"

uint32_t confused()
{
    xmlDoc *doc = xmlReadFile("swcWindow/test(stolenfromrimworld)Xml.xml", "fake", XML_PARSE_PEDANTIC|XML_PARSE_NOBLANKS);
    xmlNode* node = xmlDocGetRootElement(doc);

    printf("%s\n", node->name);
    node = node->children;
    printf("%s\n", node->name);
    node = node->children;
    printf("%s\n", node->name);
    node = node->children;
    printf("%s\n", node->name);
    node = node->children;

    //copying libxml2/example/tree1.c
    // while(node)
    // {
    //     printf("%s\n", node->name);
    //     if(node->properties)
    //         printf("%s\n", node->properties->name);
    //     if(node->next)
    //         node = node->next;
    //     else if(node->children)
    //         node = node->children;
    // }
    
    xmlFreeDoc(doc);
    return 1;
}