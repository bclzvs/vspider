VPATH = include:server:spider:template:util:test:tool
objects := main.o log.o spide.o template.o util_url.o util_http.o util_regex.o rule.o
tobjects := tmain.o util_http.o test_util_http.o log.o util_regex.o test_util_regex.o rule.o test_rule.o util_pcre.o test_util_pcre.o
ruleobjects := rulemain.o rule.o util_pcre.o
CFLAGS = -Wall -g
LIBS = -lcheck -lrt -lpthread -lm -lxml2
all:$(objects)
	cc $(CFLAGS) -o vspider $(objects) $(LIBS) 
test:$(tobjects)
	cc $(CFLAGS) -o vtest $(tobjects) $(LIBS) -lpcre
rule:$(ruleobjects)
	cc $(CFLAGS) -o rule $(ruleobjects) -lxml2 -lpcre
#/usr/lib/x86_64-linux-gnu/libcheck.a
clean:
	-rm *.o
