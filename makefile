VPATH = include:server:spider:template
objects := main.o log.o spide.o template.o
all:$(objects)
	cc -W -g -o vspider $(objects)
