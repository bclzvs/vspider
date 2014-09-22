VPATH = include:server:spider:template:util:test
objects := main.o log.o spide.o template.o util_url.o util_http.o
tobjects := tmain.o util_http.o test_util_http.o
all:$(objects)
	cc -W -g -o vspider $(objects)
test:$(tobjects)
	cc -o vtest $(tobjects) /usr/lib/libcheck.a
clean:
	-rm *.o
