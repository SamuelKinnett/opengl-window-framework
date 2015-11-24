//This class provides a resizable box containing a user-specified string

#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "element.h"
#include "GLFont.h"
#include "rendering.h"
#include <string>

class Textbox : Element {

public:
	Textbox(float, float, float, float, std::string, Rendering* rendering);

	virtual void Draw(window_t parentInfo);
	virtual void Resize(int width, int height);
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);
	virtual int Click(int x, int y, int* clickLocation, 
				window_t parentInfo);
	virtual void Move(int x, int y, window_t parentInfo);
	virtual void SetColour(int r, int g, int b, int a);
	virtual void SetBorder(bool enabled, int* colour);
	virtual void PassData(void * textToWrite);

private:
	std::string text;
	GLFont* glFont;
};

#endif
