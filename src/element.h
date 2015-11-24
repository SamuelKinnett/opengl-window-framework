//A base abstract class that all interface elements must inherit from
#include <vector>
#include "windowinfo.h"

#ifndef ELEMENT_H
#define ELEMENT_H

class Element {

public:
	window_t elementInfo;
	std::vector <Element*> children;
	int colour[4];
	int borderColour[4];
	bool border;

	//Called to draw the element to the screen
	virtual void Draw(window_t) = 0;	
	
	//Called to resize the element
	virtual void Resize(int, int) = 0;		
	
	//Called to add a child element
	virtual void AddChild(Element*) = 0;
	
	//Called to remove the child element at the index
	virtual void RemoveChild(int) = 0;	
	
	//Called to check if the element has been clicked
	virtual int Click(int, int, int*, window_t) = 0;
	
	//Called to move the element	
	virtual void Move(int, int, window_t) = 0;		 
	
	//Set the colour of the element
	virtual void SetColour(int, int, int, int) = 0;
	
	//Enable or disable borders for the element. Passing in an RGBA
	//array will also allow you to cange the colour
	virtual void SetBorder(bool, int* = 0);

	//Used to pass data to the element, e.g. sending a text box a string
	//It is up to the method of the class inheriting from this interface
	//to determine what type of data has been passed!
	virtual void PassData(void *);
};

#endif
