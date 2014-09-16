#include <string.h>
#include <stdlib.h>
#include "../include/util_http.h"
#include <stdio.h>
/* get status from header
 * header format:
 * HTTP/1.1 200 OK
 * return 200
 */
int http_getStatus(char *headerBuf)
{
	char *sp = strstr(headerBuf, " ");
	//printf("%d\n", sp - headerBuf);
	char *ep = strstr(sp+1, " ");
	char *statusStr = malloc(ep-sp+1);
	memcpy(statusStr, sp, ep-sp);	
	statusStr[ep-sp+1] = '\0';
	//printf("status:%s\n", statusStr);
	int	status = 0;
	status = atoi(statusStr);
	free(statusStr);
	return status;
}

/* get header value from header buf */
char *http_getHeaderVal(char *headBuf, char *key, int len)
{
	char *fkey = malloc(len+4);
	fkey[0] = "\r";
	fkey[1] = "\n";
	memcpy(fkey+2, key, len);
	fkey[2+len] = ":";
	fkey[3+len] = "\0";
	strstr(headBuf, fkey); 
}
