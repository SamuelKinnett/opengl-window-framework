#ifndef WINDOW_H
#define WINDOW_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include "element.h"
#include "windowinfo.h"
//A class describing a window
class Window : public Element {

	int windowType;	//WINDOW_DISCRETE, WINDOW_SCALING, WINDOW_FIXED_H or WINDOW_FIXED_W
	char *windowTitle;
	int screenWidth, screenHeight, childCount;
public:
	Window(int x, int y, int width, int height);		//WINDOW_DISCRETE
	Window(float x, float y, float width, float height);	//WINDOW_SCALING
	Window(float x, float y, float width, int height);	//WINDOW_FIXED_H
	Window(float x, float y, int width, float height);	//WINDOW_FIXED_W
	~Window();

	virtual void Draw(window_t);
	virtual void Resize(int, int);
	virtual void Move(int x, int y, window_t);
	virtual int Click(int x, int y, int* clickLocation, window_t);
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);
private:
	void PixelToFloat(int x, int y, float* returnArray);
	float PixelToFloat1D(int value, int axisSize);
	void FloatToPixel(float x, float y, int* returnArray);
	int FloatToPixel1D(float value, int axisSize);
	void GetRelativeFloat(float x, float y, float* returnArray, window_t parentInfo);
};

#endif
