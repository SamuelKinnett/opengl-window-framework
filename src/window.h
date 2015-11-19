#ifndef WINDOW_H
#define WINDOW_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include "element.h"
#include "windowinfo.h"
//A class describing a window
class Window : public Element {

	int windowType;	//WINDOW_DISCRETE or WINDOW_SCALING
	char *windowTitle;
	int screenWidth, screenHeight, childCount;
public:
	Window(int x, int y, int width, int height, window_t);
	Window(float x, float y, float width, float height, window_t);
	~Window();

	virtual void Draw(window_t);
	virtual void Resize(int, int);
	virtual void Move(int x, int y, window_t);
	virtual int Click(int x, int y, int* clickLocation, window_t);
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);
private:
	void PixelToFloat(int x, int y, float* returnArray);
	void FloatToPixel(float x, float y, int* returnArray);
};

#endif
