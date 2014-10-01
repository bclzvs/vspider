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
			char *substring_start = subject + ovector[2*i];
			int substring_length = ovector[2*i+1] - ovector[2*i];
			printf("%2d: %.*s\n", i, substring_length, substring_start);
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
