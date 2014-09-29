#ifndef _RULE_H
#define _RULE_H
#include <libxml/tree.h>
#include <libxml/parser.h>
struct rule {
	xmlNodePtr	node;
	char	*name;
	char	*pattern;
	char	*match;
	char	*action;
	char	*actionarg;		
};
typedef struct rule rule_t;
xmlDocPtr rule_load(const char *siteName);
rule_t *rule_first(xmlDocPtr doc);
rule_t *rule_new(xmlNodePtr node);
rule_t *rule_findby(xmlDocPtr doc, const char *name);
#endif
