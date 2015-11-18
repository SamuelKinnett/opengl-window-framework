//A base abstract class that all interface elements must inherit from
#include <vector>
using namespace std;

#ifndef ELEMENT_H
#define ELEMENT_H

class Element {

public:
	int _xPosition, _yPosition, childCount;
	vector <Element*> children;

	virtual void Draw(float, float) = 0;	//Called to draw the element to the screen
	virtual void Resize(int, int) = 0;	//Called to resize the element
	virtual void AddChild(Element*) = 0;	//Called to add a child element
	virtual void RemoveChild(int) = 0;	//Called to remove the child at the index
	virtual int Click(int, int, int*, float, float) = 0;	//Called to check if the element has been clicked
	virtual void Move(int, int, float, float) = 0;		//Called to move the element 
};

#endif
