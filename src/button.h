//*****************************************************************************
//This class implements a button for the windows framework.
//
//Written by Samuel Kinnett, 2015
//*****************************************************************************

#ifndef BUTTON_H
#define BUTTON_H

#include "element.h"
#include "rendering.h"
#include "container.h"

#ifdef _WIN32
//We're on winblows, include windows.h
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <functional>

//forward declaration
class Container;

class Button : public Element {
	
public:
	//The constructor takes standard float arguments for the size and an
	//integer value that allows for user defined button callbacks. It also
	//takes a pointer to the buttonclick callback in the main function.
	Button(float x, float y, float width, float height,
			Element* parent, Rendering* rendering,
			int buttonType, Container* GUI);
	//TODO: Add other constructors, similar to the window class.	
	~Button();
	
	virtual void Draw();	
	virtual bool Create();	//TODO: Implement
	virtual bool Close();	//TODO: Implement
	virtual void Resize(int, int);		
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);	
	virtual int Click(int, int, int*);	
	virtual void Move(int, int);		 
	virtual void SetColour(int, int, int, int);
	virtual void SetBorder(bool, int* = 0);
	virtual void PassData(void *);

	int buttonType;	//An integer value that allows the user to determine
	// what action should be taken in the HandleButtonClicked function. 

private:
	Rendering* rendering;
	std::function<void()> buttonCallback;
};

#endif
