#include <string.h>
#include <stdlib.h>
#include "../include/util_http.h"
#include <stdio.h>
#include "../include/log.h"
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
char *http_getHeaderVal(char *headBuf, char *key)
{
	int len = strlen(key);
	int fl = len + 4; // \r \n : \0 so is 4
	char *fkey = malloc(fl);
	fkey[0] = '\r';
	fkey[1] = '\n';
	memcpy(fkey+2, key, len);
	fkey[2+len] = ':';
	fkey[3+len] = '\0';
	char *p = strstr(headBuf, fkey); 
	free(fkey);
	if(p == NULL) return NULL;
	p += fl;	
	char *ep = strchr(p, '\r');
	char *ret = malloc(ep - p);
	memcpy(ret, p, ep-p);
	return ret;	
}

/* remove chunk tag */
char *http_rmchunk(char *bodybuf, int *pretlen)
{
	char lenbuf[20];
	int	retlen = 0;
	char *ret = NULL;// = malloc(retlen);
	char *chunk_len_index = bodybuf;
	char *chunk_len_end;
	char *con_index; // content index
	int len = 0;
	len = strtol(chunk_len_index, NULL, 16);
	for(; len > 0; len = strtol(chunk_len_index, NULL, 16)){
		printf("chunk len:%d\n", len);
		chunk_len_end = strchr(chunk_len_index, '\r');
		con_index = chunk_len_end + 2;  // skip \r\n move to content start
		ret = (char *)realloc(ret, retlen + len + 1);
		memcpy(ret + retlen, con_index, len); // append content to ret
		retlen += len;
		ret[retlen] = '\0';	
//		log_debug("chunk:%s\n", ret);
		chunk_len_index = con_index + len + 2; // locate to next chunk len, 2 is pre \r\n
		//e = s;
	}
	*pretlen = retlen;
	return ret; 
}
