#ifndef _UTIL_HTTP_H
#define _UTIL_HTTP_H

// get status from header buffer;
int http_getStatus(char *headBuf);
/* get header value from header buffer */
char *http_getHeaderVal(char *headBuf, char *key);
char *http_rmchunk(char *bodybuf);
#endif
