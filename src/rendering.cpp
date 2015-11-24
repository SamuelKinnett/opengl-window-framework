#include "rendering.h"

Rendering::Rendering(int screenWidth, int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

void Rendering::Resize(int width, int height) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

void Rendering::PixelToFloat(
