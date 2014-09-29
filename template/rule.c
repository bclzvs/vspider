#include "../include/rule.h"
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#define FILENAME_SIZE	100
#define PATH_SIZE	200
#define SET_ATTR(a,x) if(!xmlStrcmp(t->name, BAD_CAST x)){\
		a = (char *)xmlGetProp(node, BAD_CAST x); \
		continue;\
		}
xmlDocPtr rule_load(const char *siteName)
{
	xmlDocPtr doc;
	char	fileName[FILENAME_SIZE];	
	memset(fileName, 0, FILENAME_SIZE);
	snprintf(fileName, FILENAME_SIZE, "template/%s.conf", siteName);
	doc = xmlReadFile(fileName, "utf-8", XML_PARSE_RECOVER);
	return doc;
}

rule_t *rule_first(xmlDocPtr doc)
{
	xmlNodePtr	curNode;
	curNode = xmlDocGetRootElement(doc);
	if(curNode == NULL) return NULL;
	curNode = curNode->xmlChildrenNode;
	if(curNode == NULL) return NULL;
	curNode = curNode->next; // because curNode is text node \r\t	
	return rule_new(curNode);
}

rule_t *rule_new(xmlNodePtr node)
{
	rule_t *pr = malloc(sizeof(rule_t));
	pr->node = node;
	xmlAttrPtr attrPtr = node->properties;
	xmlAttrPtr t = NULL;
	while(attrPtr != NULL){
		t	= attrPtr;
		attrPtr = attrPtr->next;
		SET_ATTR(pr->pattern, "pattern");
		SET_ATTR(pr->match, "match");
		SET_ATTR(pr->name, "name");
	}
	return pr;
}

rule_t *rule_findby(xmlDocPtr doc,const char *name)
{
	xmlXPathContextPtr	context;
	xmlXPathObjectPtr	xpathObj;
	context = xmlXPathNewContext(doc);
	char	path[PATH_SIZE];
	snprintf(path, PATH_SIZE, "//rule[@name='%s']", name);
	xpathObj = xmlXPathEvalExpression((xmlChar *)path, context);	
	xmlXPathFreeContext(context);
	
	if(xpathObj == NULL) return NULL;
	xmlNodePtr node = xpathObj->nodesetval->nodeTab[0];
	return rule_new(node);
}
