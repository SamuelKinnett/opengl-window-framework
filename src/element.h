//A base abstract class that all interface elements must inherit from
#include <vector>
#include "windowinfo.h"
#include "rendering.h"
#include "originpoints.h"

#ifndef ELEMENT_H
#define ELEMENT_H

//forward declaration
struct window_t;
class Rendering;

class Element {

public:
	window_t* elementInfo;
	std::vector <Element*> children;
	Rendering* rendering;
	
	int colour[4];
	int screenWidth, screenHeight;	//Used to locally store the dimensions of the screen for resizing and suchlike
	bool border;
	int windowType;	//The type of window (e.g. scaling, discrete etc.)
	bool draggable;
	bool inAnimation;	//Is the element currently in an animation?
	int defaultColour[4]{ 26, 35, 34, 202 };
	int borderColour[4]{ 157, 242, 201, 255 };
	int childCount; //How many children does the element have?
	
	originPoints origin;
	
	//This variable stores the x and y position of the origin point of the
	//element.
	float originPosition[2];
	//These modifiers are used to make the width and/or height of the
	//element positive or negative as required by the origin.
	int xModifier, yModifier;
	


	//Called to draw the element to the screen
	virtual void Draw() = 0;	
	
	//Allows for opening animations and effects
	//Returns true when the object and all of its children
	//are finished.
	virtual bool Create() = 0;

	//Allows for closing animations and effects
	//Returns true when the object and all of its children
	//are finished.
	virtual bool Close() = 0;
	
	//Called to resize the element
	virtual void Resize(int, int) = 0;		
	
	//Called to add a child element
	virtual void AddChild(Element*) = 0;
	
	//Called to remove the child element at the index
	virtual void RemoveChild(int) = 0;	
	
	//Called to check if the element has been clicked
	virtual int Click(int, int, int*) = 0;
	
	//Called to move the element	
	virtual void Move(int, int) = 0;		 
	
	//Set the colour of the element
	virtual void SetColour(int, int, int, int) = 0;
	
	//Enable or disable borders for the element. Passing in an RGBA
	//array will also allow you to cange the colour
	virtual void SetBorder(bool, int* = 0) = 0;

	//Used to pass data to the element, e.g. sending a text box a string
	//It is up to the method of the class inheriting from this interface
	//to determine what type of data has been passed!
	virtual void PassData(void *) = 0;
};

#endif
