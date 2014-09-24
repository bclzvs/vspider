/* regex helper */
#include "../include/util_regex.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char *regex_getMatchValue(char *input, char *pattern)
{
	int	status, i;
	//printf("input:%s\n", input);
	//printf("pattern:%s\n", pattern);
	regmatch_t pmatch[1];
	int	cflags = REG_EXTENDED;
	size_t	nmatch = 1;
	regex_t	reg;
	regcomp(&reg, pattern, cflags);
	status = regexec(&reg, input, nmatch, pmatch, 0);
	if( status != 0){
		regfree(&reg);
		return NULL;
	}
	char	*ret = regex_getValue(input, pmatch);
	//printf("match value:%s\n", ret);
	regfree(&reg);
	return ret;
}

char *regex_getValue(char* input, regmatch_t *pmatch)
{
	int	vlen = pmatch->rm_eo - pmatch->rm_so;
	char	*ret = malloc(vlen + 1);
	memcpy(ret, input + pmatch->rm_so, vlen);
	ret[vlen] = '\0';
	return ret;
}

regmatch_t* regex_getMatches(char *input, char*pattern)
{
	int	status, i;
	//printf("input:%s\n", input);
	//printf("pattern:%s\n", pattern);
	regmatch_t *pmatch = malloc(sizeof(regmatch_t) * 9);
	int	cflags = REG_EXTENDED;
	size_t	nmatch = 9;
	regex_t	reg;
	regcomp(&reg, pattern, cflags);
	status = regexec(&reg, input, nmatch, pmatch, 0);
	if( status != 0){
		regfree(&reg);
		return NULL;
	}
	regfree(&reg);
	return pmatch;
}
