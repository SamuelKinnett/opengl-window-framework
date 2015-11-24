//This class provides functions for rendering to the screen.

#ifndef RENDERING_H
#define RENDERING_H
#include "windowinfo.h"
class Rendering {

public:
	//Constructor, taking the screen width and height
	Rendering(int, int);

	//Called when the window is resized, updates the stored screen
	//dimensions
	void Resize(int, int);
	
	//This method converts a pixel co-ordinate to a float co-ordinate
	void PixelToFloat(int x, int y, float* returnArray);
	
	//Converts an integer value, usually a pixel, into a float value
	//constrained by the size of the provided axis
	float PixelToFloat1D(int value, int axisSize);
	
	//This method converts a float co-ordinate into a pixel co-ordinate.
	void FloatToPixel(float x, float y, int* returnArray);

	//Converts a float value into an integer value constrained by the size
	//of the provided axis
	int FloatToPixel1D(float value, int axisSize);
	
	//Returns a float co-ordinate within a parent element, given the
	//position of the element relative to said parent.
	void GetRelativeFloat(float x, float y, float* returnArray, 
				window_t parentInfo);

private:
	int screenWidth, screenHeight;

};

#endif
