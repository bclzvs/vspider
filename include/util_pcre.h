#ifndef _UTIL_PCRE_H
#define _UTIL_PCRE_H
struct pcre_capture {
	char *name;
	char *value;
	struct pcre_capture *next;
};
typedef struct pcre_capture pcre_capture_t; 
char *pcre_getMatchVal(char *subject, char *pattern, int subject_len);

struct pcre_capture* pcre_getMatchCapture(char *subject, char *pattern, int subject_len);
#endif
