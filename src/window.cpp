#include "window.h"
#include <iostream>
#include "windowinfo.h"
using namespace std;

#define WINDOW_DISCRETE 0
#define WINDOW_SCALING 1
#define WINDOW_FIXED_H 2
#define WINDOW_FIXED_W 3

//used to enable and disable movement along certain axes
#define AXIS_X 0
#define AXIS_Y 1

/* Standard constructor, taking arguments in terms of pixels
A window instantiated with this constructor will remain the same size and in the same
position regardless of the main window resizing.  */
Window::Window(int x, int y, int width, int height) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];
	
	this->elementInfo.x = x;
	//cout << "Relative X: " << this->xPosition << endl;
	this->elementInfo.y = y;
	//cout << "Relative Y:" << this->yPosition << endl;
	this->elementInfo.width = width;
	this->elementInfo.height = height;
	this->windowType = WINDOW_DISCRETE;
	this->childCount = 0; 	//Storing the childcount rather than checking it
				//every time a child needs to be removed.

	//Set the colour to a nice, slightly transparent blue
	this->colour[0] = 80;
	this->colour[1] = 84;
	this->colour[2] = 248;
	this->colour[3] = 222;
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

	this->elementInfo.x = x;
	this->elementInfo.y = y;
	this->elementInfo.width = width;
	this->elementInfo.height = height;
	this->windowType = WINDOW_SCALING;
	this->childCount = 0;

	this->colour[0] = 80;
	this->colour[1] = 84;
	this->colour[2] = 248;
	this->colour[3] = 222;
}

//Composite constructor accepting both float and iteger values.
//This constructor creates a window that remains at the same relative space when
//its parent window is resized, however only the width will scale with the window.
Window::Window(float x, float y, float width, int height) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];	

	this->elementInfo.x = x;
	this->elementInfo.y = y;
	this->elementInfo.width = width;
	this->elementInfo.height = height;
	this->windowType = WINDOW_FIXED_H;
	this->childCount = 0;

	this->colour[0] = 80;
	this->colour[1] = 84;
	this->colour[2] = 248;
	this->colour[3] = 222;

}

//Composite constructor accepting both float and iteger values.
//This constructor creates a window that remains at the same relative space when
//its parent window is resized, however only the height will scale with the window.
Window::Window(float x, float y, int width, float height) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];
	
	this->elementInfo.x = x;
	this->elementInfo.y = y;
	this->elementInfo.width = width;
	this->elementInfo.height = height;
	this->windowType = WINDOW_FIXED_W;
	this->childCount = 0;

	this->colour[0] = 80;
	this->colour[1] = 84;
	this->colour[2] = 248;
	this->colour[3] = 222;

}

//Destructor
Window::~Window() {

}

//Called whenever the window needs to be drawn
void Window::Draw(window_t parentInfo) {
	float windowVectors[2][2];	//2D array containing the vector of each point of the window
	float tempArray[2];		//Stores the return value until it is inserted into the 2D array
	float relativeSize[2];		//The x and y position of the element added to its parent
	float relativePosition[2];	//The top right co-ordinates of the element added to its parent
	
	//Initialise variables here to prevent scope issues in the switch statement
	float floatWidth = 0;
	float floatHeight = 0;

	switch (windowType) {
		case WINDOW_DISCRETE:
			//We need to convert the pixel co-ordinates to world co-ordinates
			int parentPixel[2];
			FloatToPixel(parentInfo.x, parentInfo.y, parentPixel);
			PixelToFloat(parentPixel[0] + this->elementInfo.x, parentPixel[1] + this->elementInfo.y, tempArray);
			//The bottom left corner of the window
			windowVectors[0][0] = tempArray[0];
			windowVectors[0][1] = tempArray[1];
	
			PixelToFloat(parentPixel[0] + this->elementInfo.x + this->elementInfo.width, 
					parentPixel[1] +  this->elementInfo.y + this->elementInfo.height, 
					tempArray);
			//The top right corner of the window
			windowVectors[1][0] = tempArray[0];
			windowVectors[1][1] = tempArray[1];
			break;

		case WINDOW_SCALING:
			//The bottom left corner of the window
			GetRelativeFloat(this->elementInfo.x, this->elementInfo.y, relativePosition, parentInfo);
			GetRelativeFloat(this->elementInfo.x + this->elementInfo.width,
						this->elementInfo.y + this->elementInfo.height,
						relativeSize, parentInfo);
			windowVectors[0][0] = relativePosition[0];
			//parentInfo.x + this->elementInfo.x;
			windowVectors[0][1] = relativePosition[1];
			//The top right corner of the window
			windowVectors[1][0] = relativeSize[0];
			windowVectors[1][1] = relativeSize[1];
			break;

		case WINDOW_FIXED_H:
			//The bottom left corner of the window
			
			//Convert the height into a float value
			floatHeight = PixelToFloat1D(FloatToPixel1D(this->elementInfo.y, this->screenHeight)
				       				+ this->elementInfo.height,
			      					this->screenHeight);
			GetRelativeFloat(this->elementInfo.x, this->elementInfo.y, relativePosition, parentInfo);
			GetRelativeFloat(this->elementInfo.x + this->elementInfo.width,
						floatHeight,
						relativeSize, parentInfo);
			windowVectors[0][0] = relativePosition[0];
			//parentInfo.x + this->elementInfo.x;
			windowVectors[0][1] = relativePosition[1];
			//The top right corner of the window
			windowVectors[1][0] = relativeSize[0];
			windowVectors[1][1] = relativeSize[1];
			break;
		
		case WINDOW_FIXED_W:
			//The bottom left corner of the window
			
			//Convert the width into a float value
			floatWidth = PixelToFloat1D(FloatToPixel1D(this->elementInfo.x, this->screenWidth)
				       				+ this->elementInfo.width,
			      					this->screenWidth);
			GetRelativeFloat(this->elementInfo.x, this->elementInfo.y, relativePosition, parentInfo);
			GetRelativeFloat(floatWidth,
						this->elementInfo.y + this->elementInfo.height,
						relativeSize, parentInfo);
			windowVectors[0][0] = relativePosition[0];
			//parentInfo.x + this->elementInfo.x;
			windowVectors[0][1] = relativePosition[1];
			//The top right corner of the window
			windowVectors[1][0] = relativeSize[0];
			windowVectors[1][1] = relativeSize[1];
			break;	
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
		this->children[i]->Draw(this->elementInfo);
	}
}

//Called whenever the window needs to be resized
void Window::Resize(int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;
}

//Converts a pixel co-ordinate into a float co-ordinate
void Window::PixelToFloat(int x, int y, float* returnArray) {
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
float Window::PixelToFloat1D(int value, int axisSize) {
	return (2 * ((float)value / axisSize) - 1);
}

//Converts a float co-ordinate into a pixel co-ordinate
void Window::FloatToPixel(float x, float y, int* returnArray) {
	returnArray[0] = FloatToPixel1D(x, this->screenWidth);
	returnArray[1] = FloatToPixel1D(y, this->screenHeight);
}

//Converts a float into an integer value within an axis
int Window::FloatToPixel1D(float value, int axisSize) {
	return (((value + 1) / 2.0) * axisSize);
}

//Checks to see if the mouse falls within the bounds of the window. If it does, the
//function will return true. In addition, it will set the clickLocation array to the
//x and y values of the mouse relative to the top left corner of the window.
int Window::Click(int x, int y, int* clickLocation, window_t parentInfo) {
	int tempArray[2];
	int tempSizeArray[2];
	float relativePosition[2];
	float relativeSize[2];
	float floatHeight = 0;
	float floatWidth = 0;

	switch (windowType) {
		
		case WINDOW_DISCRETE:
			int parentPixel[2];
			FloatToPixel(parentInfo.x, parentInfo.y, parentPixel);
			if (x < parentPixel[0] + this->elementInfo.x + this->elementInfo.width
				&& x > parentPixel[0] + this->elementInfo.x
				&& y < parentPixel[1] + this->elementInfo.y + this->elementInfo.height
				&& y > parentPixel[1] + this->elementInfo.y) {

				clickLocation[0] = x - (parentPixel[0] + this->elementInfo.x);
				clickLocation[1] = y - (parentPixel[1] + this->elementInfo.y);
				
				//Check to see if any of the child items have been clicked.
				//If they have, we return a 0 to prevent the window from
				//being moved.
				for (int i = 0; i < childCount; ++i) 
					if (children[i]->Click(x, y, clickLocation, this->elementInfo) == 1)
						return 0;
				//Otherwise, we return 1 so the window can be dragged
				return 1;
			}
			break;

	 	case WINDOW_SCALING:
			GetRelativeFloat(this->elementInfo.x, this->elementInfo.y, relativePosition, parentInfo);
			GetRelativeFloat(this->elementInfo.x + this->elementInfo.width,
						this->elementInfo.y + this->elementInfo.height,
						relativeSize, parentInfo);
			FloatToPixel(relativePosition[0], relativePosition[1], tempArray);
			FloatToPixel(relativeSize[0], relativeSize[1], tempSizeArray);
			if (x < tempSizeArray[0]
				&& x > tempArray[0]
				&& y < tempSizeArray[1]
				&& y > tempArray[1]) {

				clickLocation[0] = x - tempArray[0];
				clickLocation[1] = y - tempArray[1];

				cout << "Collision Detected At: " << tempArray[0] << "," << tempArray[1];
			
				//Once again, check to see if any of the child items have been clicked
				for(int i = 0; i < childCount; ++i)
					if (children[i]->Click(x, y, clickLocation, this->elementInfo) == 1)
						return 0;
				return 1;
			}
			break;

	 	case WINDOW_FIXED_H:
			floatHeight = PixelToFloat1D(FloatToPixel1D(this->elementInfo.y, this->screenHeight)
							+ this->elementInfo.height,
							this->screenHeight);
			GetRelativeFloat(this->elementInfo.x, this->elementInfo.y, relativePosition, parentInfo);
			GetRelativeFloat(this->elementInfo.x + this->elementInfo.width,
						floatHeight,
						relativeSize, parentInfo);
			FloatToPixel(relativePosition[0], relativePosition[1], tempArray);
			FloatToPixel(relativeSize[0], relativeSize[1], tempSizeArray);
			if (x < tempSizeArray[0]
				&& x > tempArray[0]
				&& y < tempSizeArray[1]
				&& y > tempArray[1]) {

				clickLocation[0] = x - tempArray[0];
				clickLocation[1] = y - tempArray[1];

				cout << "Collision Detected At: " << tempArray[0] << "," << tempArray[1];
			
				//Once again, check to see if any of the child items have been clicked
				for(int i = 0; i < childCount; ++i)
					if (children[i]->Click(x, y, clickLocation, this->elementInfo) == 1)
						return 0;
				return 1;
			}
			break;

	 	case WINDOW_FIXED_W:
			floatWidth = PixelToFloat1D(FloatToPixel1D(this->elementInfo.x, this->screenWidth)
							+ this->elementInfo.width,
							this->screenWidth);
			GetRelativeFloat(this->elementInfo.x, this->elementInfo.y, relativePosition, parentInfo);
			GetRelativeFloat(floatWidth,
						this->elementInfo.y + this->elementInfo.height,
						relativeSize, parentInfo);
			FloatToPixel(relativePosition[0], relativePosition[1], tempArray);
			FloatToPixel(relativeSize[0], relativeSize[1], tempSizeArray);
			if (x < tempSizeArray[0]
				&& x > tempArray[0]
				&& y < tempSizeArray[1]
				&& y > tempArray[1]) {

				clickLocation[0] = x - tempArray[0];
				clickLocation[1] = y - tempArray[1];

				cout << "Collision Detected At: " << tempArray[0] << "," << tempArray[1];
			
				//Once again, check to see if any of the child items have been clicked
				for(int i = 0; i < childCount; ++i)
					if (children[i]->Click(x, y, clickLocation, elementInfo) == 1)
						return 0;
				return 1;
			}
			break;
	}
	cout << "No Collision Detected" << endl;
	return 0;
}

//Moves the window to the specified position. Eventually, this could be extended to allow
//for moving animations and the like. X and Y are screen position values given in pixels
//and so need to be converted if the window is a scaling type.
void Window::Move(int x, int y, window_t parentInfo) {
	if (windowType == WINDOW_DISCRETE) {
		int parentPixel[2];
		FloatToPixel(parentInfo.x, parentInfo.y, parentPixel);
		this->elementInfo.x = parentPixel[0] + x;
		this->elementInfo.y = parentPixel[1] + y;
	} else {
		//All other window types use float values to store their position.
		float tempArray[2];
		float relativePosition[2];
		PixelToFloat(x, y, tempArray);
		GetRelativeFloat(tempArray[0], tempArray[1], relativePosition, parentInfo);
		this->elementInfo.x = relativePosition[0];
		this->elementInfo.y = relativePosition[1];
	}
}

//Add a new child element
void Window::AddChild(Element* newChild) {
	this->children.push_back(newChild);
	this->childCount++;
}

//Remove a child element at the specified index
void Window::RemoveChild(int childIndex) {
	if (childIndex < this->childCount) {
		this->children.erase(this->children.begin() + childIndex);
		childCount--;
	}
}

//Returns a float value relative to the parent element
void Window::GetRelativeFloat(float x, float y, float* returnArray, window_t parentInfo) {
	returnArray[0] = parentInfo.x + (((x + 1.0)/2.0) * parentInfo.width);
	returnArray[1] = parentInfo.y + (((y + 1.0)/2.0) * parentInfo.height);
}
