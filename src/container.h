//*****************************************************************************
//This class describes a container of elements. This class forms the foundation
// of the framework; every element of the gui must be contained within a main
// container object.
//
//Written by Samuel Kinnett, 2015
//*****************************************************************************

#ifndef CONTAINER_H
#define CONTAINER_H
#include "element.h"
#include "button.h"
#include "window.h"
#include "rendering.h"
#include "originpoints.h"

//foward declaration
class Button;
class Container : public Element {
	
public:
	Container(float x, float y, float width, float height,
		       	Rendering* rendering);

	virtual void Draw();	
	virtual bool Create();
	virtual bool Close();
	virtual void Resize(int, int);		
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);	
	virtual int Click(int, int, int*);
	virtual void Move(int, int);		 
	virtual void SetColour(int, int, int, int);
	virtual void SetBorder(bool, int* = 0);
	virtual void PassData(void *);

	void ButtonCallback(Button*);
	Window* CreateWindow(float x, float y, float width, float height); 
private:
	int activeWindow;
	bool movingWindow;
	Rendering* rendering;
};

#endif
