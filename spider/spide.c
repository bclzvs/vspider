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

SR spide_url(char *url);
SpideResultP spide(TemplateP template)
{
	log_info("spide name:%s, url:%s\n",template->name, template->url);
	SR ret = spide_url(template->url); 
	if(ret->status == SPIDE_SUCCESS) log_info("spide success\n");
	return ret;
}

SR spide_url(char *url)
{
	int	sockfd;
	//char	*ptr, **pptr;
	struct in_addr **pptr;
	struct hostent *hptr;
	struct sockaddr_in addr;
	char	ipstr[16];
	SR	ret;
	ret = malloc(sizeof(struct spide_result_t));
	int	l = strlen(url)+1;
	if( (hptr = gethostbyname(url_gethost(url,l))) == NULL){
		log_err("gethostbyname error for host:%s:%s",url, hstrerror(h_errno));
		ret->status = SPIDE_EDNS;
		return ret;
	}
	pptr = (struct in_addr**) hptr->h_addr_list;
	for(; *pptr != NULL; pptr++) {
	//	inet_ntop(hptr->h_addrtype, *pptr,ipstr,16);
	//	log_info("\taddress:%s\n",ipstr);
	}	
	pptr--;
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)	
		log_err("socket error\n");
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port	= htons(80);
	memcpy(&addr.sin_addr, *pptr, sizeof(struct in_addr));
	log_info("connecting\n");
	if( connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0){
		inet_ntop(hptr->h_addrtype, *pptr,ipstr,16);
		log_err("connect %s:%s failed\n", url, ipstr);
		ret->status = SPIDE_ECONN;
	} else {
		log_info("connected\n");
	}
//	return "tmp html";	
	return ret;
}
