//*****************************************************************************
//This class implements a button for the windows framework.
//
//Written by Samuel Kinnett, 2015
//*****************************************************************************

#ifndef BUTTON_H
#define BUTTON_H
#include "element.h"
#include "rendering.h"
#include <functional>

class Button : public Element {
	
public:
	//The constructor takes standard float arguments but also a pointer
	//to a function to be executed when the button is clicked.
	Button(float x, float y, float width, float height,
			Rendering* rendering, std::function<void()>* function);
	~Button();
	
	virtual void Draw(window_t);	
	virtual bool Create();	//TODO: Implement
	virtual bool Close();	//TODO: Implement
	virtual void Resize(int, int);		
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);	
	virtual int Click(int, int, int*, window_t);	
	virtual void Move(int, int, window_t);		 
	virtual void SetColour(int, int, int, int);
	virtual void SetBorder(bool, int* = 0);
	virtual void PassData(void *);

private:
	std::function<void()>* function;
	Rendering* rendering;
};

#endif
