#ifndef _UTIL_REGEX_H
#define _UTIL_REGEX_H
#include <regex.h>
struct list_rmatch {
	regmatch_t *pmatch;
	struct list_rmatch *next;
};
typedef struct list_rmatch list_rmatch_t;
/* get value from match[0]*/
char *regex_getMatchValue(char *input, char *pattern);
/* get value from input by pmatch's so and eo */
char *regex_getValue(char *input, regmatch_t *pmatch);
/* get all matches */
regmatch_t* regex_getMatches(char *input, char*pattern);
/* get list matches */
list_rmatch_t* regex_getListMatch(char *input, char *pattern, int *pcount);
#endif
