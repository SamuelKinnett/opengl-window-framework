CC=gcc
CXX=g++
RM=rm -f

SRCS=window.cpp main.cpp
OBJS=rendering.o glfont.o textbox.o window.o main.o
LDLIBS=-lGL -LGLU  -lglut

all: opengl-window-framework

opengl-window-framework: $(OBJS) 
	$(CXX) -Wall -g -o ../bin/opengl-window-framework $(OBJS) $(LDLIBS)

window.o: window.cpp window.h element.h windowinfo.h rendering.h
	g++ -Wall -g -c window.cpp -o window.o

main.o: main.cpp windowinfo.h rendering.h
	g++ -Wall -g -c main.cpp -o main.o

textbox.o: textbox.cpp textbox.h element.h windowinfo.h rendering.h glfont.h
	g++ -Wall -g -c textbox.cpp -o textbox.o

rendering.o: rendering.cpp rendering.h element.h
	g++ -Wall -g -c rendering.cpp -o rendering.o

glfont.o: glfont.cpp glfont.h
	g++ -Wall -g -c glfont.cpp -o glfont.o

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) opengl-window-framework
