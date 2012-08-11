CC=gcc
CFLAGS=-Wall -ansi -pedantic -D_GNU_SOURCE -I/usr/X11R6/include
LDFLAGS=-pthread -L/usr/X11R6/lib -L/usr/local/lib -lm -lGL -lGLU -lglut
OBJS=main.o my_endian.o pcx.o scene.o

prtunnel:	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o main

clean:
	rm -f main
	rm -f $(OBJS)

main.o: main.c
my_endian.o: my_endian.c
pcx.o: pcx.c
scene.o: scene.c
