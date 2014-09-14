#include <string.h>
#include "../include/template.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#define BUFF_SIZE 4096

static char buf[BUFF_SIZE];

char* getKeyValue(const char* key, int keyLength, int lineStart,int lineEnd);
TemplateP load_all_template()
{
	int	fd;
	size_t	count;
	if( (fd = open("template/template.conf",O_RDONLY)) < 0){
		log_err("can't open template");
	}
	int	lineStart = 0;
	TemplateP root,template;
	root = NULL;
	template = NULL;
	int	splitIndex = 0;
	while ( (count = read(fd, buf, BUFF_SIZE)) > 0){
		//log_debug("template lenth:%d\n",count);
		int	i = 0;
		for(i; i < count; i++){
			//continue;
			//log_debug("%c",buf[i]);
			if(buf[i] == '\n'){
				//log_debug("line readed\n");
				if(buf[i-1] == ']' && buf[lineStart] == '['){
					// [xxx] is new template item
					if(template == NULL){
						template = malloc(sizeof(Template));
						root = template;
					} else {
						template->next = malloc(sizeof(Template));
						template = template->next;
					}
		//			log_debug("cretae template\n");
				} else {

					char *name;
					name = getKeyValue("name", 4, lineStart,i);
					if(name != NULL) template->name = name;
					//log_debug("t->name:%s\n",template->name);
					char *url;
					url = getKeyValue("url", 3, lineStart,i);
					if(url!=NULL) template->url=url;
				}
				lineStart = i+1;	 				
				//log_debug("lineStart:%c\n",buf[lineStart]);
			} 
		}	
	}

	return root;
}

char* getKeyValue(const char* key, int keyLength, int lineStart,int lineEnd)
{
	if(!strncmp(key,buf+lineStart,keyLength) && !strncmp("=",buf+lineStart+keyLength,1) ){
		// equal key= 
		//log_debug("eq\n");
		size_t valueSize = lineEnd-lineStart-keyLength; // keylength+=+\0
		//log_debug("%s size:%d\n",key,valueSize);
		char* value = (char *)malloc(valueSize);
		//log_debug("name malloced");
		memcpy(value, buf+lineStart+keyLength+1, valueSize-1); // keylength+1
		value[valueSize] = '\0';
		//log_debug("%s:%s\n",key,value);
		return value;
		//template->name = name;
	} else { 
		return NULL;
	}
}
