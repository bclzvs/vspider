VPATH = include:server:spider:template:util:test
objects := main.o log.o spide.o template.o util_url.o util_http.o
tobjects := tmain.o util_http.o test_util_http.o log.o util_regex.o test_util_regex.o
CFLAGS = -W -g
all:$(objects)
	cc $(CFLAGS) -o vspider $(objects)
test:$(tobjects)
	cc $(CFLAGS) -o vtest $(tobjects) /usr/lib/libcheck.a
clean:
	-rm *.o
