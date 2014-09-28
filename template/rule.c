#include "../include/rule.h"
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#define FILENAME_SIZE 100
#define SET_ATTR(x) if(!xmlStrcmp(attrPtr->name, BAD_CAST x)){\
		pr->pattern = (char *)xmlGetProp(node, BAD_CAST x); \
		continue;\
		}
xmlDocPtr rule_load(char *siteName)
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
	while(attrPtr != NULL){
		SET_ATTR("pattern");
		SET_ATTR("match");
		attrPtr = attrPtr->next;
	}
	return pr;
}
