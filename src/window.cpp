#include "window.h"
#include <iostream>
using namespace std;

#define WINDOW_DISCRETE 0
#define WINDOW_SCALING 1

/* Standard constructor, taking arguments in terms of pixels
A window instantiated with this constructor will remain the same size and in the same
position regardless of the main window resizing.  */
Window::Window(int x, int y, int width, int height, float parentX, float parentY) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];
	
	int parentPixel[2];
	FloatToPixel(parentX, parentY, parentPixel);
	this->xPosition = parentPixel[0] + x;
	this->yPosition = parentPixel[1] + y;
	this->width = width;
	this->height = height;
	this->windowType = WINDOW_DISCRETE;
	this->childCount = 0; 	//Storing the childcount rather than checking it
				//every time a child needs to be removed.

	//Set the colour to a nice, slightly transparent blue
	this->colour[0] = 80;
	this->colour[1] = 84;
	this->colour[2] = 248;
	this->colour[3] = 222;

	cout << "Created New Discrete Window" << endl;
}

/* Float constructor taking arguments as values between 0 and 1
A window instantiated with this constructor will remain at a relative size and position
to the main window, according to the float values.
*/
Window::Window(float x, float y, float width, float height, float parentX, float parentY) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];
	this->xPosition = parentX + x;
	this->yPosition = parentY + y;
	this->width = width;
	this->height = height;
	this->windowType = WINDOW_SCALING;
	this->childCount = 0;
	
	this->colour[0] = 80;
	this->colour[1] = 84;
	this->colour[2] = 248;
	this->colour[3] = 222;

	cout << "Created New Scaling Window" << endl;
}

//Destructor
Window::~Window() {

}

//Called whenever the window needs to be drawn
void Window::Draw(float parentX, float parentY) {
	float windowVectors[2][2];	//2D array containing the vector of each point of the window
	float tempArray[2];	//Stores the return value until it is inserted into the 2D array

	if (windowType == WINDOW_DISCRETE) {
		//We need to convert the pixel co-ordinates to world co-ordinates
		PixelToFloat(this->xPosition, this->yPosition, tempArray);
		//The bottom left corner of the window
		windowVectors[0][0] = parentX + tempArray[0];
		windowVectors[0][1] = parentY + tempArray[1];

		PixelToFloat(this->xPosition + this->width, this->yPosition + this->height, tempArray);
		//The top right corner of the window
		windowVectors[1][0] = parentX + tempArray[0];
		windowVectors[1][1] = parentY + tempArray[1];

	} else {
		//The bottom left corner of the window
		windowVectors[0][0] = parentX + this->xPosition;
		windowVectors[0][1] = parentY + this->yPosition;
		//The top right corner of the window
		windowVectors[1][0] = parentX + this->xPosition + this->width;
		windowVectors[1][1] = parentY + this->yPosition + this->height;
	}

	//Set the colour
	glColor4ub(this->colour[0], this->colour[1], this->colour[2], this->colour[3]);
	
	glBegin(GL_QUADS);
	glVertex2f(windowVectors[0][0], windowVectors[0][1]);
	glVertex2f(windowVectors[1][0], windowVectors[0][1]);
	glVertex2f(windowVectors[1][0], windowVectors[1][1]);
	glVertex2f(windowVectors[0][0], windowVectors[1][1]);
	glEnd();

	for (int i = 0; i < childCount; ++i){
		if (windowType == WINDOW_DISCRETE) {
			PixelToFloat(this->xPosition, this->yPosition, tempArray);
			this->children[i]->Draw(parentX + tempArray[0], parentY + tempArray[1]);
		} else {
			this->children[i]->Draw(parentX + xPosition, parentY + yPosition);
		}
	}
}

//Called whenever the window needs to be resized
void Window::Resize(int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;
}

//Converts a pixel co-ordinate into a float co-ordinate
void Window::PixelToFloat(int x, int y, float* returnArray) {
	returnArray[0] = 2 * ((float)x / this->screenWidth) - 1;
	returnArray[1] = 2 * ((float)y / this->screenHeight) - 1;

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
	returnArray[0] = ((x + 1) / 2.0) * this->screenWidth;
	returnArray[1] = ((y + 1) / 2.0) * this->screenHeight;
}

//Checks to see if the mouse falls within the bounds of the window. If it does, the
//function will return true. In addition, it will set the clickLocation array to the
//x and y values of the mouse relative to the top left corner of the window.
int Window::Click(int x, int y, int* clickLocation, float parentX, float parentY) {
	if (windowType == WINDOW_DISCRETE) {
		int parentPixel[2];
		FloatToPixel(parentX, parentY, parentPixel);
		if (x < parentPixel[0] + this->xPosition + this->width
			&& x > parentPixel[0] + this->xPosition
			&& y < parentPixel[1] + this->yPosition + this->height
			&& y > parentPixel[1] + this->yPosition) {

			clickLocation[0] = x - (parentPixel[0] + this->xPosition);
			clickLocation[1] = y - (parentPixel[1] + this->yPosition);

			return 1;
		}
	} else {
		int tempArray[2];
		int tempSizeArray[2];
		FloatToPixel(parentX + this->xPosition, parentY + this->yPosition, tempArray);
		FloatToPixel(parentX + this->xPosition + this->width,
				parentY + this->yPosition + this->height,
				tempSizeArray);
		if (x < tempSizeArray[0]
			&& x > tempArray[0]
			&& y < tempSizeArray[1]
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
void Window::Move(int x, int y, float parentX, float parentY) {
	if (windowType == WINDOW_DISCRETE) {
		int parentPixel[2];
		FloatToPixel(parentX, parentY, parentPixel);
		this->xPosition = parentPixel[0] + x;
		this->yPosition = parentPixel[1] + y;
	} else {
		float tempArray[2];
		PixelToFloat(x, y, tempArray);
		this->xPosition = parentX + tempArray[0];
		this->yPosition = parentY + tempArray[1];
	}
}

void Window::AddChild(Element* newChild) {
	this->children.push_back(newChild);
	this->childCount++;
}

void Window::RemoveChild(int childIndex) {
	if (childIndex < this->childCount)
		this->children.erase(this->children.begin() + childIndex);
}
