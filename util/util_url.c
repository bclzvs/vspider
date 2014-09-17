#include <string.h>
#include "../include/util_url.h"

char *url_gethost(char *url, int l)
{
	char *p = (char *)strcasestr(url, "://");
	if(p != NULL) 
		p+=3;
	else
		p = url;

	//char *ep = strchr(p, "/");
	return p;
}
