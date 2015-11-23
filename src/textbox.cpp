#include "textbox.h"
#include "GLFont.h"
#include <string>
using namespace std;

//Floating point constructor. Places the textbox at a position and with a size
//always relative to the parent window.
Textbox::Textbox(float x, float y, float width, float height, string text) {
	this->elementInfo.x = x;
	this->elementInfo.y = y;
	this->elementInfo.width = width;
	this->elementInfo.height = height;
	this->text = text;

	this->glFont = new GLFont();
	glFont->Create("franklin_gothic.glf");
}

void Textbox::Resize(int screenWidth, int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}


