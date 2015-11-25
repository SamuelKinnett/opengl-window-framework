#ifndef WINDOW_H
#define WINDOW_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include "element.h"
#include "windowinfo.h"
#include "rendering.h"
#include "animationstate.h"

//A class describing a window
class Window : public Element {

public:
	//WINDOW_DISCRETE
	Window(int x, int y, int width, int height, Rendering* rendering);		
	
	//WINDOW_SCALING
	Window(float x, float y, float width, float height, 
			Rendering* rendering);

	//WINDOW_FIXED_H
	Window(float x, float y, float width, int height, 
			Rendering* rendering);

	//WINDOW_FIXED_W
	Window(float x, float y, int width, float height, 
			Rendering* rendering);
	
	~Window();

	virtual void SetColour(int R, int G, int B, int A);
	virtual void SetBorder(bool enabled, int* colour = 0);

	virtual void Draw(window_t);
	virtual bool Create();
	virtual bool Close();

	virtual void Resize(int, int);
	virtual void Move(int x, int y, window_t);
	virtual int Click(int x, int y, int* clickLocation, window_t);
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);
	virtual void PassData(void * input);
private:
	int windowType;	//WINDOW_DISCRETE, WINDOW_SCALING, WINDOW_FIXED_H
	//or WINDOW_FIXED_W

	char *windowTitle;

	//animation related variables
	animationState animState;
	window_t  targetDimensions;

	int screenWidth, screenHeight, childCount;
	void Initialise(float x, float y, float width, float height,
			Rendering* rendering);
};

#endif
