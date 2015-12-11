#include "textbox.h"
#include "glfont.h"
#include "rendering.h"
#include "windowinfo.h"
#include <string>

#define TEXTBOX_DISCRETE 0
#define TEXTBOX_SCALING 1
#define TEXTBOX_FIXED_H 2
#define TEXTBOX_FIXED_W 3

using namespace std;

Textbox::Textbox(int x, int y, int width, int height, Element * parent, 
			string text, Rendering * rendering) {
	this->windowType = TEXTBOX_DISCRETE;
	Initialise((float)x, (float)y, (float)width, (float)height, parent, text, rendering);
}

//Floating point constructor. Places the textbox at a position and with a 
//size always relative to the parent window.
Textbox::Textbox(float x, float y, float width, float height, Element* parent,
			string text, Rendering* rendering) {
	this->windowType = TEXTBOX_SCALING;
	Initialise(x, y, width, height, parent, text, rendering);
}

Textbox::Textbox(float x, float y, int width, float height, Element * parent, 
			string text, Rendering * rendering) {
	this->windowType = TEXTBOX_FIXED_W;
	Initialise(x, y, (float)width, height, parent, text, rendering);
}

Textbox::Textbox(float x, float y, float width, int height, Element * parent, 
			string text, Rendering * rendering) {
	this->windowType = TEXTBOX_FIXED_H;
	Initialise(x, y, width, (float)height, parent, text, rendering);
}

void Textbox::Initialise(float x, float y, float width, float height, Element * parent, std::string text, Rendering * rendering)
{
	this->elementInfo = new window_t;

	this->elementInfo->x = x;
	this->elementInfo->y = y;
	this->elementInfo->width = width;
	this->elementInfo->height = height;
	this->elementInfo->index = parent->childCount;
	this->elementInfo->parent = parent;

	this->colour[0] = this->defaultColour[0];
	this->colour[0] = this->defaultColour[0];
	this->colour[0] = this->defaultColour[0];
	this->colour[0] = this->defaultColour[0];

	this->text = text;
	this->rendering = rendering;

	this->glFont = new GLFont();
	glFont->Create("sf_square_head.glf");
}


Textbox::~Textbox() {
	delete this->elementInfo;
}

bool Textbox::Create() {
	//TODO: Implement
	return false;
}

bool Textbox::Close() {
	//TODO: Implement
	return false;
}

void Textbox::Resize(int screenWidth, int screenHeight) {
	//Nothing doing here at the moment. To shut GCC up, I've added a
	//completely useless body.
	this->screenHeight = screenHeight;
	this->screenWidth = screenWidth;
}

void Textbox::AddChild(Element* newChild) {
	//Not currently used
	this->children.push_back(newChild);
}

void Textbox::RemoveChild(int index) {
	//Not currently used
	this->children.erase(this->children.begin() + index);
}

int Textbox::Click(int x, int y, int* clickLocation) {

	int windowBounds[4];

	this->rendering->GetWindowBounds(this, windowBounds);

	if (x < windowBounds[1]
		&& x > windowBounds[0]
		&& y < windowBounds[3]
		&& y > windowBounds[2]) {

		clickLocation[0] = x - windowBounds[0];
		clickLocation[1] = y - windowBounds[2];

		//If this textbox can be dragged, we return a 1
		if (this->draggable)
			return 1;
		else
			return 0;
	}
	return 0;
}

void Textbox::Move(int x, int y) {
	int placeHolder[2];
	window_t* placeHolder2;

	placeHolder[0] = x;
	placeHolder[1] = y;
	placeHolder2 = this->elementInfo->parent->elementInfo;
}

void Textbox::Draw() {

	float tempArray[2];

	this->rendering->UpdateOriginPoint(this);
	//this->rendering->DrawWindow(this);
	
	//TODO: Account for different origin points/window types.
	//Currently just uses the bottom left corner.

	glFont->Begin();
	glFont->RenderText(this->text.c_str(), 0.025, this);
	glFont->End();
}

void Textbox::SetColour(int r, int g, int b, int a) {
	this->colour[0] = r;
	this->colour[1] = g;
	this->colour[2] = b;
	this->colour[3] = a;
}

void Textbox::SetBorder(bool enabled, int* colour) {
	this->border = enabled;
	if (enabled) {
		this->borderColour[0] = colour[0];
		this->borderColour[1] = colour[1];
		this->borderColour[2] = colour[2];
		this->borderColour[3] = colour[3];
	}
}

void Textbox::PassData(void* data) {
	this->text = *(string*)data;
}