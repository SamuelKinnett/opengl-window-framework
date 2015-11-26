//*****************************************************************************
//This class describes a container of elements. It is never drawn, and as a
//result is ideal to use as a "main window" of sorts, containing all of the GUI
//elements of the application.
//
//Written by Samuel Kinnett, 2015
//*****************************************************************************

#ifndef CONTAINER_H
#define CONTAINER_H
#include "element.h"

class Container : public Element {
	
public:
	Container(float x, float y, float width, float height);

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

private:
	int childCount;
	int activeWindow;
	bool movingWindow;
};

#endif
