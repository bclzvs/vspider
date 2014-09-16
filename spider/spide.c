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
#include "stdio.h"
#include <errno.h>
#include "../include/util_http.h"

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
	char	*host = url_gethost(url,l);
	if( (hptr = gethostbyname(host)) == NULL){
		log_err("gethostbyname error for host:%s:%s",url, hstrerror(h_errno));
		ret->status = SPIDE_EDNS;
		return ret;
	}

	pptr = (struct in_addr**) hptr->h_addr_list;
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
		return ret;
	}
	log_info("connected\n");
	char sendBuf[1024];	
	sprintf(sendBuf, "GET %s HTTP/1.1\n"
"Host: %s\n"
"Connection: keep-alive\n"
"Accept: text/html\n"
"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1707.0 Safari/537.36\n"
//"Accept-Encoding: gzip,deflate,sdch\n"
"TE: trailers, chunked"
"Accept-Language: zh-CN,zh;q=0.8,en;q=0.6\n"
"\n"
, url, host);			
	if(send(sockfd, sendBuf, 1024, 0) == -1){
		log_err("send error:%s\n", strerror(errno));
	}
	char hbuf[2048];	// save headers;
	int	bufsize = 8192;
	char rbuf[bufsize];
	char *bodybuf = malloc(bufsize);
	int	bodybufsize = bufsize;
	int bodylen = 0;
	int 	count = 0;
	int	hbufed = 0;
	int	bodyi = 0;
	// get head
	while( (count = read(sockfd, rbuf,bufsize)) > 0){
		printf("bufstarted-------\n%s\n",rbuf);	 
		if(hbufed == 0){
			//printf("first buffer:%s\n", rbuf);
			int i = 1;
			for(;i < count; i++){
				if(rbuf[i-1] == '\n' && rbuf[i] == '\r'){
					log_debug("found split line %d\n",i);
					memcpy(hbuf, rbuf, i);	 
					//hbuf[i+1] = '\0';
					bodyi = i + 2; // \r\n
					hbufed = 1;
					memcpy(bodybuf, rbuf+i+2, count-i-2);
					bodylen = count-i-2;
				}
			}
		} else {
			if( (bodylen + count) > bodybufsize){
				bodybuf = realloc(bodybuf, bodylen+count+1);
				//memcpy(bodybuf, 	
			}	
			memcpy(bodybuf+bodylen, rbuf, count);	
			bodylen += count;
		}
	}
//	printf("header:\n%s",hbuf);
	int	http_status = http_getStatus(hbuf);
	if(http_status >= 400) {
		ret->status = SPIDE_EHTTPSTATUS;
		return ret;
	}

//	printf("len:%d html:%s\n",bodylen, bodybuf);
	ret->html = bodybuf;
	return ret;
}
