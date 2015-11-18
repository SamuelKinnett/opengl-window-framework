#ifndef WINDOW_H
#define WINDOW_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include "element.h"

//A class describing a window
class Window : public Element {

	float xPosition, yPosition, width, height;
	int windowType;	//WINDOW_DISCRETE or WINDOW_SCALING
	char *windowTitle;
	int colour[4]; //R, G, B, A
	int screenWidth, screenHeight;

public:
	Window(int x, int y, int width, int height, float parentX, float parentY);
	Window(float x, float y, float width, float height, float parentX, float parentY);
	~Window();

	virtual void Draw(float, float);
	virtual void Resize(int, int);
	virtual void Move(int x, int y, float, float);
	virtual int Click(int x, int y, int* clickLocation, float, float);
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);
private:
	void PixelToFloat(int x, int y, float* returnArray);
	void FloatToPixel(float x, float y, int* returnArray);
};

#endif
