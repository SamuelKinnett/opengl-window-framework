CC=gcc
CXX=g++
RM=rm -f

SRCS=window.cpp main.cpp
OBJS=window.o main.o
LDLIBS=-lGL -LGLU  -lglut

all: opengl-window-framework

opengl-window-framework: $(OBJS) 
	$(CXX) -Wall -g -o ../bin/opengl-window-framework $(OBJS) $(LDLIBS)

window.o: window.cpp window.h element.h windowinfo.h
	g++ -Wall -g -c window.cpp -o window.o

main.o: main.cpp windowinfo.h
	g++ -Wall -g -c main.cpp -o main.o

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) opengl-window-framework
