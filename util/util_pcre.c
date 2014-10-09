#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>
#include "../include/util_pcre.h"
#define OVECCOUNT 30
char *pcre_getMatchVal(char *subject, char *pattern, int subject_len)
{
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc;

	re = pcre_compile(
	  pattern,              /* the pattern */
	  0,                    /* default options */
	  &error,               /* for error message */
	  &erroffset,           /* for error offset */
	  NULL);                /* use default character tables */

	/* Compilation failed: print the error message and exit */

	if (re == NULL)
	{
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}

	rc = pcre_exec(
	  re,                   /* the compiled pattern */
	  NULL,                 /* no extra data - we didn't study the pattern */
	  subject,              /* the subject string */
	  subject_len,       /* the length of the subject */
	  0,                    /* start at offset 0 in the subject */
	  0,                    /* default options */
	  ovector,              /* output vector for substring information */
	  OVECCOUNT);           /* number of elements in the output vector */

	if (rc < 0 )
	{
		if(rc != PCRE_ERROR_NOMATCH) printf("Matching error %d\n", rc); 
		pcre_free(re); 
		return NULL;
	}
	
	size_t	rlen = ovector[1] - ovector[0];
	char	*result = malloc(rlen+1);
	memcpy(result, subject + ovector[0], rlen);
	result[rlen] = '\0';
	pcre_free(re);
	return result;	
}

struct pcre_capture* pcre_getMatchCapture(char *subject, char *pattern, int subject_len)
{
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc, i;
	int namecount;
	int name_entry_size;
	unsigned char *name_table;
	pcre_capture_t *head = NULL;
	pcre_capture_t *cur = NULL;

	re = pcre_compile(
	  pattern,              /* the pattern */
	  0,                    /* default options */
	  &error,               /* for error message */
	  &erroffset,           /* for error offset */
	  NULL);                /* use default character tables */

	/* Compilation failed: print the error message and exit */

	if (re == NULL)
	{
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}

	rc = pcre_exec(
	  re,                   /* the compiled pattern */
	  NULL,                 /* no extra data - we didn't study the pattern */
	  subject,              /* the subject string */
	  subject_len,       /* the length of the subject */
	  0,                    /* start at offset 0 in the subject */
	  0,                    /* default options */
	  ovector,              /* output vector for substring information */
	  OVECCOUNT);           /* number of elements in the output vector */

	if (rc < 0 )
	{
		if(rc != PCRE_ERROR_NOMATCH) printf("Matching error %d\n", rc); 
		pcre_free(re); 
		return NULL;
	}

	if (rc == 0)
	{
		rc = OVECCOUNT/3;
		printf("ovector only has room for %d captured substrings\n", rc - 1);
	}

	(void)pcre_fullinfo(
	  re,                   /* the compiled pattern */
	  NULL,                 /* no extra data - we didn't study the pattern */
	  PCRE_INFO_NAMECOUNT,  /* number of named substrings */
	  &namecount);          /* where to put the answer */
	head = malloc(sizeof(pcre_capture_t));
	size_t len = ovector[1] - ovector[0];
	head->value = malloc(len+1);	
	memcpy(head->value, subject + ovector[0], len);
	head->value[len] = '\0'; 
	cur = head;
	if (namecount <= 0) {
		for (i = 1; i < rc; i++)
		{
			pcre_capture_t *t = malloc(sizeof(struct pcre_capture));
			memset(t, 0, sizeof(pcre_capture_t));
			int vallen = ovector[2*i+1] - ovector[2*i];
			char *value = malloc(vallen + 1);
			memcpy(value, subject + ovector[2*i], vallen);
			value[vallen] = '\0';	
			t->value = value;
			
			cur->next = t;
			cur = cur->next;
		}
	} else {
		unsigned char *tabptr;

		/* Before we can access the substrings, we must extract the table for
		translating names to numbers, and the size of each entry in the table. */

		(void)pcre_fullinfo(
		re,                       /* the compiled pattern */
		NULL,                     /* no extra data - we didn't study the pattern */
		PCRE_INFO_NAMETABLE,      /* address of the table */
		&name_table);             /* where to put the answer */

		(void)pcre_fullinfo(
		re,                       /* the compiled pattern */
		NULL,                     /* no extra data - we didn't study the pattern */
		PCRE_INFO_NAMEENTRYSIZE,  /* size of each entry in the table */
		&name_entry_size);        /* where to put the answer */

		/* Now we can scan the table and, for each entry, print the number, the name,
		and the substring itself. */

		tabptr = name_table;
		for (i = 0; i < namecount; i++)
		{
			int n = (tabptr[0] << 8) | tabptr[1];
			//printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
			//ovector[2*n+1] - ovector[2*n], subject + ovector[2*n]);
			pcre_capture_t *t = malloc(sizeof(struct pcre_capture));
			char *name = malloc(name_entry_size-3+1);
			strncpy(name, (const char *)tabptr + 2, name_entry_size - 3 + 1);
			t->name = name;
			int vallen = ovector[2*n+1] - ovector[2*n];
			char *value = malloc(vallen + 1);
			memcpy(value, subject + ovector[2*n], vallen);
			value[vallen] = '\0';	
			t->value = value;
			
			cur->next = t;
			cur = cur->next;
			tabptr += name_entry_size;
		}
	}
	return head;
}

pcre_match_t *pcre_getMatches(char *subject, char *pattern, int subject_len)
{
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc, i;
	int namecount;
	int name_entry_size;
	int subject_offset = 0;
	unsigned char *name_table;
	pcre_match_t *matchhead = malloc(sizeof(pcre_match_t));
	pcre_match_t *curmatch= NULL;
	memset(matchhead, 0, sizeof(pcre_match_t));
	char *matchmsg = malloc(sizeof(MATCH_MSG_SIZE));
	matchhead->msg = matchmsg;
	re = pcre_compile(
	  pattern,              /* the pattern */
	  0,                    /* default options */
	  &error,               /* for error message */
	  &erroffset,           /* for error offset */
	  NULL);                /* use default character tables */

	/* Compilation failed: print the error message and exit */

	if (re == NULL)
	{
		snprintf(matchmsg,MATCH_MSG_SIZE, "PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return matchhead;
	}
	for(;;){
		pcre_match_t *matcht= malloc(sizeof(pcre_match_t));
		memset(matcht, 0, sizeof(pcre_match_t));
		pcre_capture_t *head = NULL;
		pcre_capture_t *cur = NULL;
		rc = pcre_exec(
		  re,                   /* the compiled pattern */
		  NULL,                 /* no extra data - we didn't study the pattern */
		  subject,              /* the subject string */
		  subject_len,       /* the length of the subject */
		  subject_offset, 
		  0,                    /* default options */
		  ovector,              /* output vector for substring information */
		  OVECCOUNT);           /* number of elements in the output vector */

		if (rc < 0 )
		{
			if(rc != PCRE_ERROR_NOMATCH) {
				snprintf(matchmsg, MATCH_MSG_SIZE, "Matching error %d\n", rc); 
			}
			//pcre_free(re); 
			break;
		}

		if (rc == 0)
		{
			rc = OVECCOUNT/3;
			snprintf(matchmsg, MATCH_MSG_SIZE, "ovector only has room for %d captured substrings\n", rc - 1);
			break;
		}

		(void)pcre_fullinfo(
		  re,                   /* the compiled pattern */
		  NULL,                 /* no extra data - we didn't study the pattern */
		  PCRE_INFO_NAMECOUNT,  /* number of named substrings */
		  &namecount);          /* where to put the answer */
		head = malloc(sizeof(pcre_capture_t));
		size_t len = ovector[1] - ovector[0];
		head->value = malloc(len+1);	
		memcpy(head->value, subject + ovector[0], len);
		head->value[len] = '\0'; 
		cur = head;
		subject_offset = ovector[1]; /* next match start offset */
		if (namecount <= 0) {
			for (i = 1; i < rc; i++)
			{
				pcre_capture_t *t = malloc(sizeof(struct pcre_capture));
				memset(t, 0, sizeof(pcre_capture_t));
				int vallen = ovector[2*i+1] - ovector[2*i];
				char *value = malloc(vallen + 1);
				memcpy(value, subject + ovector[2*i], vallen);
				value[vallen] = '\0';	
				t->value = value;
				
				cur->next = t;
				cur = cur->next;
			}
		} else {
			unsigned char *tabptr;

			/* Before we can access the substrings, we must extract the table for
			translating names to numbers, and the size of each entry in the table. */

			(void)pcre_fullinfo(
			re,                       /* the compiled pattern */
			NULL,                     /* no extra data - we didn't study the pattern */
			PCRE_INFO_NAMETABLE,      /* address of the table */
			&name_table);             /* where to put the answer */

			(void)pcre_fullinfo(
			re,                       /* the compiled pattern */
			NULL,                     /* no extra data - we didn't study the pattern */
			PCRE_INFO_NAMEENTRYSIZE,  /* size of each entry in the table */
			&name_entry_size);        /* where to put the answer */

			/* Now we can scan the table and, for each entry, print the number, the name,
			and the substring itself. */

			tabptr = name_table;
			for (i = 0; i < namecount; i++)
			{
				int n = (tabptr[0] << 8) | tabptr[1];
				//printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
				//ovector[2*n+1] - ovector[2*n], subject + ovector[2*n]);
				pcre_capture_t *t = malloc(sizeof(struct pcre_capture));
				char *name = malloc(name_entry_size-3+1);
				strncpy(name, (const char *)tabptr + 2, name_entry_size - 3 + 1);
				t->name = name;
				int vallen = ovector[2*n+1] - ovector[2*n];
				char *value = malloc(vallen + 1);
				memcpy(value, subject + ovector[2*n], vallen);
				value[vallen] = '\0';	
				t->value = value;
				
				cur->next = t;
				cur = cur->next;
				tabptr += name_entry_size;
			}
		}
		if(matchhead->pcapture == NULL){
			matchhead->success = 1;
			matchhead->pcapture = head;
			curmatch = matchhead;
		} else {
			matcht->pcapture = head;
			curmatch->next = matcht;
			curmatch = curmatch->next;
		}	
		
	}
	pcre_free(re);
	return matchhead;
}

pcre_capture_t *pcre_capture_findByName(pcre_capture_t *pcapture, char *name)
{
	if(pcapture == NULL || name == NULL) return NULL;
	for(; pcapture != NULL; pcapture = pcapture->next){
		if(pcapture->name == NULL) continue;
		if(!strcmp(pcapture->name, name)) return pcapture;
	}	
	return NULL;
}

pcre_capture_t *pcre_capture_findByIndex(pcre_capture_t *pcapture, int index)
{
	if(pcapture == NULL) return NULL;
	int	i;
	for(i = 0; pcapture != NULL; i++, pcapture = pcapture->next){
		if(i == index) return pcapture;
	}	
	return NULL;
}

void free_pcre_capture(pcre_capture_t *pcapture)
{
	if(pcapture == NULL) return;
	for(; pcapture != NULL; pcapture = pcapture->next){
		if(pcapture->name != NULL) free(pcapture->name);
		if(pcapture->value != NULL) free(pcapture->value);
		free(pcapture);	
	}	
	return;
}
void free_pcre_match(pcre_match_t *pmatch)
{
	if(pmatch == NULL) return;
	for(; pmatch != NULL; pmatch = pmatch->next){
		free(pmatch->pcapture);
		if(pmatch->msg != NULL) free(pmatch->msg);
		free(pmatch);		
	}
}
