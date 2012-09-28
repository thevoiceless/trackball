
CC = g++ 

LIBOPT = -L/usr/X11R6/lib -L/usr/local/lib -lglut -lGLU -lGL -lm

all : proj2

proj2 : proj2.o
	$(CC) -o proj2 proj2.o $(LIBOPT) -lGLEW

%.o: %.c *.h Makefile
	$(CC) $(OPT) -c -o $@ $< 

clean : 
	rm *.o proj2

