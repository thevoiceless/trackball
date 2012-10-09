OPENGL = -L/usr/X11R6/lib -L/usr/local/lib -lglut -lGLU -lGL -lm -lGLEW
OPT = -O2

OBJS = IOstuff.o proj2.o
CPPS = IOstuff.cpp proj2.cpp
EXES = proj2

all: proj2

proj2: objects $(OBJS)
	g++ $(OBJS) -o proj2 $(OPT) $(OPENGL)

objects: $(CPPS)
	g++ -c $(CPPS) $(OPT)

clean: 
	rm $(EXES)
	rm $(OBJS)

