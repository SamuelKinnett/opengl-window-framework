#ifndef WINDOW_H
#define WINDOW_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include "element.h"

//A class describing a window
class Window : public Element {

	int _xPosition, _yPosition, _width, _height;
	char *windowTitle;
	int colour[3]; //R, G, B

public:
	Window(int x, int y, int width, int height);
	Window(float x, float y, float width, float height);
	~Window();

	virtual void Draw();
	virtual void Resize();
};

#endif
