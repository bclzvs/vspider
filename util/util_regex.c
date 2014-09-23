/* regex helper */
#include "../include/util_regex.h"
#include <stdlib.h>
#include <string.h>
char *regex_getMatchValue(char *input, char *pattern)
{
	int	status, i;
	regmatch_t pmatch[1];
	int	cflags = REG_EXTENDED;
	size_t	nmatch = 1;
	regex_t	reg;
	regcomp(&reg, pattern, cflags);
	status = regexec(&reg, input, nmatch, pmatch, 0);
	if( status != 0) return NULL;
	int	vlen = pmatch[0].rm_so - pmatch[1].rm_eo;
	char	*ret = malloc(vlen + 1);
	memcpy(ret, input + pmatch[0].rm_so, vlen);
	regfree(&reg);
	return ret;
}
