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
#include "textbox.h"
#include "rendering.h"
#include "originpoints.h"
#include <string>
#include <vector>

//foward declaration
class Button;
class Container : public Element {
	
public:
	Container(float x, float y, float width, float height,
		       	Rendering* rendering, GLFont* titleFont);
	~Container();

	virtual void Draw();	
	virtual bool Create();
	virtual bool Close();
	virtual void Resize(int, int);		
	virtual void AddChild(Element*);
	virtual void RemoveChild(int);
	virtual void RemoveChild(Element*);	
	virtual int Click(int, int, int*);
	virtual void Move(int, int);		 
	virtual void SetColour(int, int, int, int);
	virtual void SetBorder(bool, int* = 0);
	virtual void PassData(void *);

	void ButtonCallback(Button*);
	void Update();
	Window* InstantiateWindow(float x, float y, float width, float height, std::string windowText = ""); 
private:
	int activeWindow;

	//Stores the indexes of elements to delete on the next update.
	std::vector<Element*> flaggedForDeletion;
	bool movingWindow;
	Rendering* rendering;
	GLFont* titleFont;
};

#endif
