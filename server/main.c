// main
#include "../include/main.h"

int main()
{
	TemplateP template = load_all_template();

	while(template != NULL){
		//printf("template name:%s, url:%s\n", template->name, template->url);
		spide(template);
		template = template->next;
	}
}
