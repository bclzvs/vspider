/* check site.conf */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rule.h"
#include "../include/util_regex.h"

int main(const int argc, const char **argv)
{
	if(argc != 4){
		printf("usage:%s inputfile conffile rulename\n", argc[0]);
		exit(1);
	}	
	
	
}
