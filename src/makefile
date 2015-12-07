CC=gcc
CXX=g++
RM=rm -f

SRCS=window.cpp main.cpp
OBJS=container.o rendering.o glfont.o textbox.o window.o main.o button.o
LDLIBS=-lGL -LGLU  -lglut
ARGS=-std=c++11 -Wall -g

ELEMENT_DEPENDENCIES=element.h rendering.h animationstate.h windowinfo.h

all: opengl-window-framework

opengl-window-framework: $(OBJS) 
	$(CXX) $(ARGS) -o ../bin/opengl-window-framework $(OBJS) $(LDLIBS)

window.o: window.cpp window.h $(ELEMENT_DEPENDENCIES)
	g++ $(ARGS) -c window.cpp -o window.o

main.o: main.cpp main.h windowinfo.h rendering.h container.h
	g++ $(ARGS) -c main.cpp -o main.o

textbox.o: textbox.cpp textbox.h glfont.h $(ELEMENT_DEPENDENCIES)
	g++ $(ARGS) -c textbox.cpp -o textbox.o

rendering.o: rendering.cpp rendering.h element.h
	g++ $(ARGS) -c rendering.cpp -o rendering.o

glfont.o: glfont.cpp glfont.h
	g++ $(ARGS) -c glfont.cpp -o glfont.o

button.o: button.cpp button.h $(ELEMENT_DEPENDENCIES)
	g++ $(ARGS) -c button.cpp -o button.o

container.o: container.h element.h
	g++ $(ARGS) -c container.cpp -o container.o

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) opengl-window-framework
