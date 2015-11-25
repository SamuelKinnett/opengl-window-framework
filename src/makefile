CC=gcc
CXX=g++
RM=rm -f

SRCS=window.cpp main.cpp
OBJS=rendering.o glfont.o textbox.o window.o main.o button.o
LDLIBS=-lGL -LGLU  -lglut
ARGS=-std=c++11 -Wall -g

all: opengl-window-framework

opengl-window-framework: $(OBJS) 
	$(CXX) $(ARGS) -o ../bin/opengl-window-framework $(OBJS) $(LDLIBS)

window.o: window.cpp window.h element.h windowinfo.h rendering.h
	g++ $(ARGS) -c window.cpp -o window.o

main.o: main.cpp windowinfo.h rendering.h
	g++ $(ARGS) -c main.cpp -o main.o

textbox.o: textbox.cpp textbox.h element.h windowinfo.h rendering.h glfont.h
	g++ $(ARGS) -c textbox.cpp -o textbox.o

rendering.o: rendering.cpp rendering.h element.h
	g++ $(ARGS) -c rendering.cpp -o rendering.o

glfont.o: glfont.cpp glfont.h
	g++ $(ARGS) -c glfont.cpp -o glfont.o

button.o: button.cpp button.h rendering.h element.h windowinfo.h
	g++ $(ARGS) -c button.cpp -o button.o

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) opengl-window-framework
