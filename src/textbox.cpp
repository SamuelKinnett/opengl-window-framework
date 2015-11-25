#include "textbox.h"
#include "glfont.h"
#include "rendering.h"
#include "windowinfo.h"
#include <string>
using namespace std;

//Floating point constructor. Places the textbox at a position and with a 
//size always relative to the parent window.
Textbox::Textbox(float x, float y, float width, float height, string text, Rendering* rendering) {
	this->elementInfo.x = x;
	this->elementInfo.y = y;
	this->elementInfo.width = width;
	this->elementInfo.height = height;
	this->text = text;
	this->rendering = rendering;

	this->glFont = new GLFont();
	glFont->Create("franklin_gothic.glf");
}

Textbox::~Textbox() {

}

void Textbox::Resize(int screenWidth, int screenHeight) {
	//Nothing doing here at the moment. To shut GCC up, I've added a
	//completely useless body.
	int placeHolder[2];
	placeHolder[0] = screenWidth;
	placeHolder[1] = screenHeight;
}

void Textbox::AddChild(Element* newChild) {
	//Not currently used
	this->children.push_back(newChild);
}

void Textbox::RemoveChild(int index) {
	//Not currently used
	this->children.erase(this->children.begin() + index);
}

int Textbox::Click(int x, int y, int* clickLocation, window_t parentInfo) {
	int placeHolder[3];
	window_t placeHolder2;

	placeHolder[0] = x;
	placeHolder[1] = y;
	placeHolder[2] = *clickLocation;
	placeHolder2 = parentInfo;
	return 0;
}

void Textbox::Move(int x, int y, window_t parentInfo) {
	int placeHolder[2];
	window_t placeHolder2;

	placeHolder[0] = x;
	placeHolder[1] = y;
	placeHolder2 = parentInfo;
}

void Textbox::Draw(window_t parentInfo) {
	float tempArray[2];
	
	rendering->GetRelativeFloat(this->elementInfo.x, this->elementInfo.y, tempArray, parentInfo);
	glFont->Begin();
	glFont->RenderText(this->text.c_str(), this->elementInfo.x, this->elementInfo.y, 0, 1);
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
