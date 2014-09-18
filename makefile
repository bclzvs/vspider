VPATH = include:server:spider:template:util
objects := main.o log.o spide.o template.o util_url.o util_http.o
all:$(objects)
	cc -W -g -o vspider $(objects)
clean:
	-rm *.o
