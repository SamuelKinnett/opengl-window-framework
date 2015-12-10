//This class provides a resizable box containing a user-specified string

#ifndef TEXTBOX_H
#define TEXTBOX_H

#ifdef _WIN32
//We're on winblows, include windows.h
#include <Windows.h>
#endif

#include <GL/freeglut.h>
#include <GL/gl.h>
#include "element.h"
#include "windowinfo.h"
#include "rendering.h"
#include "glfont.h"
#include <string>

class Textbox : public Element {

public:
	Textbox(float, float, float, float, int, Element*,
			std::string, Rendering*);
	~Textbox();

	virtual void Draw();
	virtual bool Create(); //TODO: Implement
	virtual bool Close(); //TODO: Implement
	virtual void Resize(int, int);
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);
	virtual int Click(int, int, int*);
	virtual void Move(int, int);
	virtual void SetColour(int, int, int, int);
	virtual void SetBorder(bool, int* = 0);
	virtual void PassData(void *);

private:
	std::string text;
	GLFont* glFont;
};

#endif
