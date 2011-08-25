CC		=	g++

#PROF = -fprofile-generate
#PROF = -fprofile-arcs -fprofile-values -fvpt

#PROF = -fprofile-use
#PROF = -fbranch-probabilities -fvpt -funroll-loops -fpeel-loops -ftracer


#CPPFLAGS   =   -std=c++0x -D_GNU_SOURCE=1 -D_REENTRANT -O3 -I/usr/include/SDL
#CPPFLAGS   =    $(PROF)  -std=c++0x -D_GNU_SOURCE=1 -D_REENTRANT -g -O2 -I/usr/include/SDL
CPPFLAGS	=	-std=c++0x -D_GNU_SOURCE=1 -D_REENTRANT -O0 -pg -ggdb -I/usr/include/SDL
LDFLAGS		=	$(PROF) -lSDL -lpthread -lm 


aff3d: transfo.o triangle.o vertex.o IO.o

clean:
	rm *.o
