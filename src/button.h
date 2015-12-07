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
	//The constructor takes standard float arguments for the size and an
	//integer value that allows for user defined button callbacks.
	Button(float x, float y, float width, float height, int index,
			Element* parent, Rendering* rendering,
			int buttonType);
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
};

#endif
