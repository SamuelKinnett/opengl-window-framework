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
	Button(float x, float y, float width, float height, int index,
			Element* parent, Rendering* rendering,
			std::function<void(int)>* function);
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

private:
	std::function<void(int)>* function; //A pointer to a function with
	//return type void and taking one integer argument, the ID of the
	//button that was clicked.
	Rendering* rendering;
};

#endif
