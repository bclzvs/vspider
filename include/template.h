#ifndef _TEMPLATE_H
#define	_TEMPLATE_H 1
struct template_t {
	char *name;
	char *host;
	char *url;		
	struct template_t *next;
};
typedef struct template_t Template;
typedef struct template_t *TemplateP;

TemplateP load_all_template();
#endif
