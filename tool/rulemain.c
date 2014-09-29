/* check site.conf */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rule.h"
#include "../include/util_regex.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

int main(const int argc, const char **argv)
{
	if(argc != 4){
		printf("usage:%s inputfile conffile rulename\n", argv[0]);
		exit(1);
	}	
	const char *file = argv[1];
	int	fd;
	struct stat fstat = {};
	if(stat(file, &fstat) != 0){
		printf("get input file stat err:%s\n", strerror(errno));
		exit(errno);
	}
	char	*input = malloc(fstat.st_size + 1);	
	fd = open(argv[1], 0);	
	if(fd == -1){
		printf("read input file error:%s\n", strerror(errno));
		exit(errno);
	}
	read(fd, input, fstat.st_size);
	input[fstat.st_size] = '\0';

	xmlDocPtr doc = rule_load(argv[2]);
	rule_t *prule = rule_findby(doc, argv[3]);		
	char *val = regex_getMatchValue(input, prule->pattern);
	printf("match:[%s]\n", val);
	return 0;
}
