SHELL = /bin/sh
CC = gcc
RM = rm -f

CPPFLAGS = -I.
CFLAGS = -Wall -O3
LDFLAGS = -lm -lGL -lglut -lGLU

PROGNAME = octree
PACKAGE=$(PROGNAME)
VERSION = 1.0
HEADERS = head.h
SOURCES = main.c src.c ppm.c temps.c
OBJ = $(SOURCES:.c=.o)


all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)
	octree img/3.ppm

%.o: %.c
	$(CC) -g $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(PROGNAME) $(OBJ)