VPATH = include:server:spider:template:util:test
objects := main.o log.o spide.o template.o util_url.o util_http.o util_regex.o rule.o
tobjects := tmain.o util_http.o test_util_http.o log.o util_regex.o test_util_regex.o rule.o test_rule.o
rule := rulemain.o rule.o util_regex.o
CFLAGS = -Wall -g
LIBS = -lcheck -lrt -lpthread -lm -lxml2
all:$(objects)
	cc $(CFLAGS) -o vspider $(objects) $(LIBS) 
test:$(tobjects)
	cc $(CFLAGS) -o vtest $(tobjects) $(LIBS) 
#/usr/lib/x86_64-linux-gnu/libcheck.a
clean:
	-rm *.o
