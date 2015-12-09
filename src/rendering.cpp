#include "rendering.h"
#include "windowinfo.h"

Rendering::Rendering(int screenWidth, int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

void Rendering::Resize(int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;
}

//Converts a pixel co-ordinate into a float co-ordinate
void Rendering::PixelToFloat(int x, int y, float* returnArray) {
	returnArray[0] = PixelToFloat1D(x, this->screenWidth);
	returnArray[1] = PixelToFloat1D(y, this->screenHeight);

	//Clamp the float values between -1 and 1

	if (returnArray[0] > 1)
		returnArray[0] = 1;
	else if (returnArray[0] < -1)
		returnArray[0] = -1;

	if (returnArray[1] > 1)
		returnArray[1] = 1;
	else if (returnArray[1] < -1)
		returnArray[1] = -1;
}

//Converts an integer into a fraction of an axis
float Rendering::PixelToFloat1D(int value, int axisSize) {
	return ((float)value / axisSize);
}

//Converts a float co-ordinate into a pixel co-ordinate
void Rendering::FloatToPixel(float x, float y, int* returnArray) {
	returnArray[0] = FloatToPixel1D(x, this->screenWidth);
	returnArray[1] = FloatToPixel1D(y, this->screenHeight);
}

//Converts a float into an integer value within an axis
int Rendering::FloatToPixel1D(float value, int axisSize) {
	return (((value + 1) / 2.0) * axisSize);
}

//Returns a float value relative to the size of the parent element
void Rendering::GetRelativeFloat(float x, float y, float* returnArray,
		window_t* parentInfo) {
	returnArray[0] = GetRelativeFloat1D(x, parentInfo->width);
	returnArray[1] = GetRelativeFloat1D(y, parentInfo->height);
}

float Rendering::GetRelativeFloat1D(float value, float axisSize) {
	return ((value + 1.0f) / 2.0f) * axisSize;  
}
