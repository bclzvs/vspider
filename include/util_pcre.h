#ifndef _UTIL_PCRE_H
#define _UTIL_PCRE_H
#include <pcre.h>
#define MATCH_MSG_SIZE 200
struct pcre_capture {
	char *name;
	char *value;
	struct pcre_capture *next;
};
typedef struct pcre_capture pcre_capture_t; 
struct pcre_match {
	int	success;
	char	*msg;
	struct pcre_capture *pcapture;
	struct pcre_match *next;
	
};
typedef struct pcre_match pcre_match_t;
char *pcre_getMatchVal(char *subject, char *pattern, int subject_len);
struct pcre_capture* pcre_getMatchCapture(char *subject, char *pattern, int subject_len);
pcre_match_t *pcre_getMatches(char *subject, char *pattern, int subject_len);

pcre_capture_t *pcre_capture_findByName(pcre_capture_t *pcapture, char *name);
pcre_capture_t *pcre_capture_findByIndex(pcre_capture_t *pcapture, int index);
void free_pcre_capture(pcre_capture_t *pcapture);
void free_pcre_match(pcre_match_t *pmatch);
#endif
