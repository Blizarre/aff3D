CC		=	g++
CPPFLAGS	=	-std=c++0x -D_GNU_SOURCE=1 -D_REENTRANT -O0 -pg -ggdb -I/usr/include/SDL
LDFLAGS		=	-lSDL -lpthread -lm


aff3d: transfo.o triangle.o vertex.o IO.o
