#include "textbox.h"
#include "GLFont.h"
#include "rendering.h"
#include "windowinfo.h"
#include <string>
using namespace std;

//Floating point constructor. Places the textbox at a position and with a size
//always relative to the parent window.
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

void Textbox::Resize(int screenWidth, int screenHeight) {
	//Nothing needs to be done AT THE MOMENT
}

void Textbox::AddChild(Element*) {

}

void Textbox::RemoveChild(int) {

}

int Textbox::Click(int x, int y, int* clickLocation, window_t parentInfo) {
	return 0;
}

void Textbox::Move(int x, int y, window_t parentInfo) {

}

void Textbox::SetColour(int r, int g, int b, int a) {

}

void Textbox::SetBorder(bool enabled, int* colour) {

}

void Textbox::PassData(void* data) {
	this->text = *(string*)data;
}
