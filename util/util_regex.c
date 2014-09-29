/* regex helper */
#include "../include/util_regex.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char *regex_getMatchValue(char *input, char *pattern)
{
	int	status;
	regmatch_t pmatch[1];
	int	cflags = REG_EXTENDED; 
	size_t	nmatch = 1;
	regex_t	reg;
	status = regcomp(&reg, pattern, cflags);
	if(status != 0){
		char	errbuf[100];
		size_t el = regerror(status, &reg, errbuf, 100);
		errbuf[el] = '\0';
		printf("pattern error:%s of --->", errbuf);
		regfree(&reg);
		return NULL;
	}	
	status = regexec(&reg, input, nmatch, pmatch, 0);
	if( status != 0){
		regfree(&reg);
		return NULL;
	}
	char	*ret = regex_getValue(input, pmatch);
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
	int	status;
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

list_rmatch_t* regex_getListMatch(char *input, char *pattern, int *pcount)
{
	int	status;
	int	cflags = REG_EXTENDED;
	size_t	nmatch = 9;
	regex_t	reg;
	regcomp(&reg, pattern, cflags);

	list_rmatch_t *head = NULL;// = malloc(sizeof(list_rmatch_t));
	list_rmatch_t *cl = NULL;
	list_rmatch_t *p = NULL;
	int	off = 0;
	int	count = 0;
	for(;;){
		//regmatch_t *match = regex_getMatches(input, pattern);
		regmatch_t *pmatch = malloc(sizeof(regmatch_t) * 9);
		memset(pmatch, 0 ,sizeof(regmatch_t) * 9);
		status = regexec(&reg, input + off, nmatch, pmatch, 0);
		if( status != 0){
			break;
		}
		int	i = 0;
		for(; i < 9; i++){
			if(pmatch[i].rm_so < 0) break;
			pmatch[i].rm_so += off;
			pmatch[i].rm_eo += off;
		}
		cl = malloc(sizeof(list_rmatch_t));
		if(p!=NULL) p->next = cl;
		cl->pmatch = pmatch;
		if(head == NULL) head = cl;
		p = cl;
		cl = p->next;

		off += pmatch->rm_eo; // pmatch 0 rm_eo
		count++;
	}
	if(pcount != NULL) *pcount = count;
	regfree(&reg);
	return head;
}
