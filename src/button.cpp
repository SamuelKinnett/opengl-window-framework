//*****************************************************************************
//The Button class
//*****************************************************************************

#include "button.h"
#include <iostream>

#define BUTTON_DISCRETE 0
#define BUTTON_SCALING 1
#define BUTTON_FIXED_H 2
#define BUTTON_FIXED_W 3

//Constructor creating a scaling button
Button::Button(float x, float y, float width, float height,
		Element* parent, Rendering* rendering,
		int buttonType, Container* GUI,
		originPoints origin) {

	this->windowType = BUTTON_SCALING;
	this->Initialise(x, y, width, height, parent, rendering, buttonType, GUI, origin);
}

//Discrete constuctor
Button::Button(int x, int y, int width, int height, 
		Element * parent, Rendering * rendering, 
		int buttonType, Container * GUI,
		originPoints origin) {

	this->windowType = BUTTON_DISCRETE;
	this->Initialise(x, y, width, height, parent, rendering, buttonType, GUI, origin);
}

//Fixed width constructor
Button::Button(float x, float y, int width, float height, Element * parent, Rendering * rendering, int buttonType, Container * GUI,
				originPoints origin) {

	this->windowType = BUTTON_FIXED_W;
	this->Initialise(x, y, (float)width, height, parent, rendering, buttonType, GUI, origin);
}

//Fixed height constructor
Button::Button(float x, float y, float width, int height, Element * parent, Rendering * rendering, int buttonType, Container * GUI,
				originPoints origin) {

	this->windowType = BUTTON_FIXED_H;
	this->Initialise(x, y, width, (float)height, parent, rendering, buttonType, GUI, origin);
}

void Button::Initialise(float x, float y, float width, float height, Element *parent, Rendering * rendering, int buttonType, Container * GUI, originPoints origin) {
	this->elementInfo = new window_t;

	this->origin = origin;

	//Based on the new origin, we may need to convert the user's input
	//to make things easier for them. 
	switch (this->origin) {

	case bottomRight:
		this->elementInfo->x = x * -1.0f;
		this->elementInfo->y = y;
		break;

	case topLeft:
		this->elementInfo->x = x;
		this->elementInfo->y = y * -1.0f;
		break;

	case topRight:
		this->elementInfo->x = x * -1.0f;
		this->elementInfo->y = y * -1.0f;

	default:
		this->elementInfo->x = x;
		this->elementInfo->y = y;
		break;
	}

	this->elementInfo->width = width;
	this->elementInfo->height = height;
	this->elementInfo->index = parent->childCount;
	this->elementInfo->parent = parent;

	this->rendering = rendering;
	this->buttonType = buttonType;
	this->border = true;

	this->colour[0] = this->defaultColour[0];
	this->colour[1] = this->defaultColour[1];
	this->colour[2] = this->defaultColour[2];
	this->colour[3] = this->defaultColour[3];

	//In order to use the button callback function, we need to bind it
	//so that the compiler knows the location of the object as well as
	//its method. Using std::bind also allows us to remove the need to
	//provide 'this' as an argument every time we want to call the 
	//callback.
	this->buttonCallback = std::bind(&Container::ButtonCallback,
		GUI,
		this);
}

Button::~Button() {
	delete this->elementInfo;
}

//Draws the button to the screen
void Button::Draw() {
	rendering->DrawWindow(this);
}

bool Button::Create() {
	//TODO: Implement
	return false;
}

bool Button::Close() {
	//TODO: Implement
	return false;
}

void Button::Resize(int screenWidth, int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

void Button::AddChild(Element* newChild) {
	//Does nothing for now
}

void Button::RemoveChild(int index) {
	//Does nothing for now
}

int Button::Click(int x, int y, int* clickLocation) {
	
	int windowBounds[4];

	this->rendering->GetWindowBounds(this, windowBounds);

	if (x < windowBounds[1]
		&& x > windowBounds[0]
		&& y < windowBounds[3]
		&& y > windowBounds[2]) {

		clickLocation[0] = x - windowBounds[0];
		clickLocation[1] = y - windowBounds[2];

		std::cout << "I was clicked!" << std::endl;

		buttonCallback();	
		
		return 1;
	}
	return 0;
}

void Button::Move(int x, int y) {
	window_t* parentInfo = this->elementInfo->parent->elementInfo;
	
	float tempArray[2];	//Stores the results of converting the move
	//destination into a float co-ordinate
	float relativePosition[2];	//Stores the position of the bottom
	//left corner of the button

	this->rendering->PixelToFloat(x, y, tempArray);
	this->rendering->GetRelativeFloat(tempArray[0], tempArray[1],
				relativePosition, parentInfo);
	this->elementInfo->x = relativePosition[0];
	this->elementInfo->y = relativePosition[1];
}

void Button::SetColour(int r, int g, int b, int a) {
	this->colour[0] = r;
	this->colour[1] = g;
	this->colour[2] = b;
	this->colour[3] = a;
}

void Button::SetBorder(bool enabled, int* colour) {
	this->border = enabled;
	if (colour != 0) {
		this->borderColour[0] = colour[0];
		this->borderColour[1] = colour[1];
		this->borderColour[2] = colour[2];
		this->borderColour[3] = colour[3];
	}
}

//PassData allows the function called by the button to be changed.
void Button::PassData(void * newButtonType) {
	this->buttonType = *(int*)newButtonType;
}
