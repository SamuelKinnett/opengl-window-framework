//A base abstract class that all interface elements must inherit from

#ifndef ELEMENT_H
#define ELEMENT_H

class Element {
	int _xPosition, _yPosition;
public:
	virtual void Draw() = 0;		//Called to draw the element to the screen
	virtual void Resize(int, int) = 0;	//Called to resize the element
};

#endif
