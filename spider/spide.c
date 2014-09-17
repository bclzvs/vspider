#include "../include/log.h"
#include "../include/template.h"
#include "../include/spide.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#include "../include/util_url.h"

char *spide_url(char *url);
SpideResultP spide(TemplateP template)
{
	log_info("spide name:%s, url:%s\n",template->name, template->url);
	SpideResultP ret = malloc(sizeof(SpideResultP));
	ret->html = spide_url(template->url); 
	return ret;
}

char *spide_url(char *url)
{
	int	sockfd;
	char	*ptr, **pptr;
	struct hostent *hptr;
	char	ipstr[16];
	int	l = strlen(url)+1;
	if( (hptr = gethostbyname(url_gethost(url,l))) == NULL){
		log_err("gethostbyname error for host:%s:%s",url, hstrerror(h_errno));
	}
	log_debug("geted");
	pptr = hptr->h_addr_list;
	/*for(; *pptr != NULL; pptr++) {
		inet_ntop(hptr->h_addrtype, *pptr,ipstr,16);
		log_info("\taddress:%s\n",ipstr);
	}	*/
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)	
		log_err("socket error");
	if( connect(sockfd, pptr, sizeof(pptr)) != 0){
		inet_ntop(hptr->h_addrtype, *pptr,ipstr,16);
		log_err("connect %s:%s failed:", url,pptr);
	}
	return "tmp html";	
}
