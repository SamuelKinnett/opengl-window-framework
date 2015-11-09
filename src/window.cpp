#include "window.h"

#define WINDOW_DISCRETE 0
#define WINDOW_SCALING 1

/* Standard constructor, taking arguments in terms of pixels
A window instantiated with this constructor will remain the same size and in the same
position regardless of the main window resizing.  */
Window::Window(int x, int y, int width, int height) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];
	this->xPosition = x;
	this->yPosition = y;
	this->width = width;
	this->height = height;
	this->windowType = WINDOW_DISCRETE;

	this->colour[0] = 255;
	this->colour[1] = 255;
	this->colour[2] = 255;
	this->colour[3] = 255;
}

/* Float constructor taking arguments as values between 0 and 1
A window instantiated with this constructor will remain at a relative size and position
to the main window, according to the float values.
*/
Window::Window(float x, float y, float width, float height) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];
	this->xPosition = x;
	this->yPosition = y;
	this->width = width;
	this->height = height;
	this->windowType = WINDOW_SCALING;

	this->colour[0] = 255;
	this->colour[1] = 255;
	this->colour[2] = 255;
	this->colour[3] = 255;
}

//Destructor
Window::~Window() {

}

//Called whenever the window needs to be drawn
void Window::Draw() {
	float windowVectors[2][2];	//2D array containing the vector of each point of the window
	float tempArray[2];	//Stores the return value until it is inserted into the 2D array

	if (windowType == WINDOW_DISCRETE) {
		//We need to convert the pixel co-ordinates to world co-ordinates
		PixelToFloat(this->xPosition, this->yPosition, tempArray);
		windowVectors[0][0] = tempArray[0];
		windowVectors[0][1] = tempArray[1];

		PixelToFloat(this->xPosition + this->width, this->yPosition + this->height, tempArray);
		windowVectors[1][0] = tempArray[0];
		windowVectors[1][1] = tempArray[1];
	} else {
		windowVectors[0][0] = this->xPosition;
		windowVectors[0][1] = this->yPosition;
		windowVectors[1][0] = this->xPosition + this->width;
		windowVectors[1][1] = this->yPosition + this->height;
	}
	glBegin(GL_QUADS);
	/*
	glColor4i(this->colour[0],
			this->colour[1],
			this->colour[2],
			this->colour[3]);
	*/
	glVertex2f(windowVectors[0][0], windowVectors[1][1]);
	glVertex2f(windowVectors[1][0], windowVectors[1][1]);
	glVertex2f(windowVectors[1][0], windowVectors[0][1]);
	glVertex2f(windowVectors[0][0], windowVectors[0][1]);
	glEnd();
}

//Called whenever the window needs to be resized
void Window::Resize() {

}

//Converts a pixel co-ordinate into a float co-ordinate
void Window::PixelToFloat(int x, int y, float* returnArray) {
	returnArray[0] = 2 * (x / this->screenWidth) - 1;
	returnArray[1] = 2 * (y / this->screenHeight) - 1;

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
