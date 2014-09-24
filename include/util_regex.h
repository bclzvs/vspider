#ifndef _UTIL_REGEX_H
#define _UTIL_REGEX_H
#include <regex.h>
/* get value from match[0]*/
char *regex_getMatchValue(char *input, char *pattern);
/* get value from input by pmatch's so and eo */
char *regex_getValue(char *input, regmatch_t *pmatch);
/* get all matches */
regmatch_t* regex_getMatches(char *input, char*pattern);
#endif
