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
	
	virtual void Draw(window_t) = 0;	//Called to draw the element to the screen
	virtual void Resize(int, int) = 0;	//Called to resize the element
	virtual void AddChild(Element*) = 0;	//Called to add a child element
	virtual void RemoveChild(int) = 0;	//Called to remove the child at the index
	virtual int Click(int, int, int*, window_t) = 0;	//Called to check if the element has been clicked
	virtual void Move(int, int, window_t) = 0;		//Called to move the element 
};

#endif
