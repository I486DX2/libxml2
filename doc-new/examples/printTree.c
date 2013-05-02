/* Prints a tree of tags in the XML document.
 *
 * For example, for this this XML document:
 *
 * <hello>world<test>of</test>XML</hello>
 *
 * it will print:
 *
 * #document
 *   hello
 *     #text : world
 *     test
 *       #text : of
 *     #text : XML
 */

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef LIBXML_TREE_ENABLED
#error Tree support was not compiled in
#endif

static int printNode(xmlNodePtr node, int indent)
{
    int success = 1;
    xmlChar *name = NULL;
    xmlChar *content = NULL;
    
    name = xmlGetNodeName(node);
    content = xmlGetNodeContent(node);
    
    if (name == NULL) {
        success = 0;
        goto finish;
    }
    
    for (i = 0; i < indent; i++)
        printf("  ");

    if (content)
        printf("%s : %s\n", name, content);
    else
        printf("%s\n", name);

finish:
    xmlFree(name);
    xmlFree(content);
    
    return success;
}

static int printTree(xmlNodePtr node, int indent)
{
    xmlNodePtr cur;
    int i;

    for (cur = node; cur != NULL; cur = xmlGetNextSibling(cur)) {
        if (!printNode(cur, indent))
            return 0;

        if (!printTree(cur, indent + 1))
            return 0;
    }

    return 1;
}

int main(int argc, char **argv)
{
    xmlDocPtr doc;
    
    if (argc != 2) {
        printf("usage: %s file.xml\n", argv[0]);
        return 1;
    }

    LIBXML_TEST_VERSION
    
    doc = xmlReadFile(argv[1], NULL, 0);
    
    if (doc == NULL) {
        printf("error: could not parse file %s\n", argv[1]);
    }

    if (!printTree(xmlCastDocToNode(doc), 0)) {
        printf("cound not complete printing\n");
    }

    xmlFreeDoc(doc);
    
    xmlCleanupParser();

    return 0;
}

