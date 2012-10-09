OPENGL = -L/usr/X11R6/lib -L/usr/local/lib -lglut -lGLU -lGL -lm -lGLEW
OPT = -O2

OBJS = IOstuff.o proj2.o
CPPS = IOstuff.cpp proj2.cpp

all: proj2

proj2: objects
	g++ $(OBJS) -o proj2 $(OPT) $(OPENGL)

objects: $(CPPS)
	g++ -c $(CPPS) $(OPT)

clean: 
	rm *.o
	rm proj2

