#include "window.h"
#include <iostream>
using namespace std;

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

	cout << "Created New Discrete Window" << endl;
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
		//The bottom left corner of the window
		windowVectors[0][0] = tempArray[0];
		windowVectors[0][1] = tempArray[1];

		PixelToFloat(this->xPosition + this->width, this->yPosition + this->height, tempArray);
		//The top right corner of the window
		windowVectors[1][0] = tempArray[0];
		windowVectors[1][1] = tempArray[1];
	} else {
		//The bottom left corner of the window
		windowVectors[0][0] = this->xPosition;
		windowVectors[0][1] = this->yPosition;
		//The top right corner of the window
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
	glVertex2f(windowVectors[0][0], windowVectors[0][1]);
	glVertex2f(windowVectors[1][0], windowVectors[0][1]);
	glVertex2f(windowVectors[1][0], windowVectors[1][1]);
	glVertex2f(windowVectors[0][0], windowVectors[1][1]);
	glEnd();
}

//Called whenever the window needs to be resized
void Window::Resize(int width, int height) {
	//this->screenWidth = width;
	//this->screenHeight = height;
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

//Converts a float co-ordinate into a pixel co-ordinate
void Window::FloatToPixel(float x, float y, int* returnArray) {
	returnArray[0] = ((this->xPosition + 1) / 2.0) * this->screenWidth;
	returnArray[1] = ((this->yPosition + 1) / 2.0) * this->screenHeight;
}

//Checks to see if the mouse falls within the bounds of the window. If it does, the
//function will return true. In addition, it will set the clickLocation array to the
//x and y values of the mouse relative to the top left corner of the window.
int Window::CheckMouseCollision(int x, int y, int* clickLocation) {
	if (windowType == WINDOW_DISCRETE) {
		if (x < this->xPosition + this->width
			&& x > this->xPosition
			&& y < this->yPosition + this->height
			&& y > this->yPosition) {

			clickLocation[0] = x - this->xPosition;
			clickLocation[1] = y - this->yPosition;

			return 1;
		}
	} else {
		int tempArray[2];
		int tempSizeArray[2];
		FloatToPixel(this->xPosition, this->yPosition, tempArray);
		FloatToPixel(this->xPosition + this->width,
				this->yPosition + this->height,
				tempSizeArray);
		cout << "Window X, Y = " << tempArray[0] << "," << tempArray[1] << endl;
		cout << "Window Width, Height = " << tempSizeArray[0] << "," << tempSizeArray[1] << endl;
		if (x < tempArray[0] + tempSizeArray[0]
			&& x > tempArray[0]
			&& y < tempArray[1] + tempSizeArray[1]
			&& y > tempArray[1]) {

			clickLocation[0] = x - tempArray[0];
			clickLocation[1] = y - tempArray[1];

			cout << "Collision Detected At: " << tempArray[0] << "," << tempArray[1];
			return 1;
		}
	}
	cout << "No Collision Detected" << endl;
	return 0;
}

//Moves the window to the specified position. Eventually, this could be extended to allow
//for moving animations and the like. X and Y are screen position values given in pixels
//and so need to be converted if the window is a scaling type.
void Window::Move(int x, int y) {
	if (windowType == WINDOW_DISCRETE) {
		this->xPosition = x;
		this->yPosition = y;
	} else {
		float tempArray[2];
		PixelToFloat(x, y, tempArray);
		this->xPosition = tempArray[0];
		this->yPosition = tempArray[1];
	}
}
