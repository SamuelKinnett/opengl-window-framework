#include "rendering.h"

#define WINDOW_DISCRETE 0
#define WINDOW_SCALING 1
#define WINDOW_FIXED_H 2
#define WINDOW_FIXED_W 3

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
	returnArray[0] = parentInfo->x + GetRelativeFloat1D(x, parentInfo->width);
	returnArray[1] = parentInfo->y + GetRelativeFloat1D(y, parentInfo->height);
}

//Returns a float value representing the original value relative
//to the axis size.
float Rendering::GetRelativeFloat1D(float value, float axisSize) {
	return ((value + 1.0f) / 2.0f) * axisSize;  
}

//Updates the origin point of the element.
void Rendering::UpdateOriginPoint(Element * element)
{
	//Calculate the origin point and modifiers
	switch (element->origin) {

	case bottomLeft:
		this->GetRelativeFloat(
			-1.0f,
			-1.0f,
			element->originPosition,
			element->elementInfo->
			parent->elementInfo);
		element->xModifier = 1;
		element->yModifier = 1;
		break;

	case bottomRight:
		this->GetRelativeFloat(
			1.0f,
			-1.0f,
			element->originPosition,
			element->elementInfo->
			parent->elementInfo);
		element->xModifier = -1;
		element->yModifier = 1;
		break;

	case topLeft:
		this->GetRelativeFloat(
			-1.0f,
			1.0f,
			element->originPosition,
			element->elementInfo->
			parent->elementInfo);
		element->xModifier = 1;
		element->yModifier = -1;
		break;

	case topRight:
		this->GetRelativeFloat(
			1.0f,
			1.0f,
			element->originPosition,
			element->elementInfo->
			parent->elementInfo);
		element->xModifier = -1;
		element->yModifier = -1;
		break;
	}
}

void Rendering::DrawWindow(Element* element) {
	//Update the origin point
	UpdateOriginPoint(element);

	float windowVectors[2][2];
	//2D array containing the vector of each point of the window

	float tempArray[2];
	//Stores the bottom left co-ordinates until they are inserted into
	//the 2D array

	float tempSecondArray[2];
	//Stores the top right co-ordinates until they are inserted into the
	//2D array

	//Initialise variables here to prevent scope issues in the 
	//switch statement
	float floatWidth = 0;
	float floatHeight = 0;

	//Some pointers to make the code a little more compact!
	window_t* parentInfo = element->elementInfo->parent->elementInfo;
	window_t* window = element->elementInfo;
	Rendering* rendering = element->rendering;

	switch (element->windowType) {
	case WINDOW_DISCRETE:

		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			rendering->PixelToFloat1D(window->x, element->screenWidth)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			rendering->PixelToFloat1D(window->y, element->screenHeight)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = element->originPosition[0] +
			rendering->PixelToFloat1D(window->x + window->width,
				element->screenWidth)
			* element->xModifier;
		tempSecondArray[1] = element->originPosition[1] +
			rendering->PixelToFloat1D(window->y + window->height,
				element->screenHeight)
			* element->yModifier;
		break;

	case WINDOW_SCALING:
		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			rendering->GetRelativeFloat1D(window->x, parentInfo->width)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			rendering->GetRelativeFloat1D(window->y, parentInfo->height)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = element->originPosition[0] +
			rendering->GetRelativeFloat1D(window->x + window->width, parentInfo->width)
			* element->xModifier;
		tempSecondArray[1] = element->originPosition[1] +
			rendering->GetRelativeFloat1D(window->y + window->height, parentInfo->height)
			* element->yModifier;

		break;

	case WINDOW_FIXED_H:

		//Convert the height into a float value
		floatHeight = rendering->PixelToFloat1D(window->height,
			element->screenHeight);
		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			rendering->GetRelativeFloat1D(window->x, parentInfo->width)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			rendering->GetRelativeFloat1D(window->y, parentInfo->height)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = element->originPosition[0] +
			rendering->GetRelativeFloat1D(window->x + window->width, parentInfo->width)
			* element->xModifier;
		tempSecondArray[1] = tempArray[1] + floatHeight
			* element->yModifier;
		break;

	case WINDOW_FIXED_W:

		//Convert the width into a float value
		floatWidth = rendering->PixelToFloat1D(window->width,
			this->screenWidth)
			* element->xModifier;

		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			rendering->GetRelativeFloat1D(window->x, parentInfo->width)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			rendering->GetRelativeFloat1D(window->y, parentInfo->height)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = tempArray[0] + floatWidth
			* element->xModifier;
		tempSecondArray[1] = element->originPosition[1] +
			rendering->GetRelativeFloat1D(window->y + window->height, parentInfo->height)
			* element->yModifier;
		break;
	}

	//Store the point values in the 2D array
	windowVectors[0][0] = tempArray[0];
	windowVectors[0][1] = tempArray[1];
	windowVectors[1][0] = tempSecondArray[0];
	windowVectors[1][1] = tempSecondArray[1];


	//Set the colour
	glColor4ub(element->colour[0],
		element->colour[1],
		element->colour[2],
		element->colour[3]);

	//Draw the window. Based on the origin point we may need to adjust the order

	switch (element->origin) {

	case bottomLeft:
		//The default origin. Here, out two points are the bottom left and
		// top right respectively.
		glBegin(GL_QUADS);
		glVertex2f(windowVectors[0][0], windowVectors[0][1]);
		glVertex2f(windowVectors[1][0], windowVectors[0][1]);
		glVertex2f(windowVectors[1][0], windowVectors[1][1]);
		glVertex2f(windowVectors[0][0], windowVectors[1][1]);
		glEnd();
		break;

	case bottomRight:
		//In this case, our two points are the bottom right and the top left
		glBegin(GL_QUADS);
		glVertex2f(windowVectors[1][0], windowVectors[0][1]);
		glVertex2f(windowVectors[0][0], windowVectors[0][1]);
		glVertex2f(windowVectors[0][0], windowVectors[1][1]);
		glVertex2f(windowVectors[1][0], windowVectors[1][1]);
		glEnd();
		break;

	case topLeft:
		//Here, the points are the top left and the bottom right
		glBegin(GL_QUADS);
		glVertex2f(windowVectors[0][0], windowVectors[1][1]);
		glVertex2f(windowVectors[1][0], windowVectors[1][1]);
		glVertex2f(windowVectors[1][0], windowVectors[0][1]);
		glVertex2f(windowVectors[0][0], windowVectors[0][1]);
		glEnd();
		break;

	case topRight:
		//And finally, the points here are the top right and the bottom left.
		glBegin(GL_QUADS);
		glVertex2f(windowVectors[1][0], windowVectors[1][1]);
		glVertex2f(windowVectors[0][0], windowVectors[1][1]);
		glVertex2f(windowVectors[0][0], windowVectors[0][1]);
		glVertex2f(windowVectors[1][0], windowVectors[0][1]);
		glEnd();
		break;
	}

	//Draw the border
	if (element->border == true) {
		glColor4ub(element->borderColour[0],
			element->borderColour[1],
			element->borderColour[2],
			element->borderColour[3]);

		switch (element->origin) {

		case bottomLeft:
			//The default origin. Here, out two points are the bottom left and
			// top right respectively.
			glBegin(GL_LINE_LOOP);
			glVertex2f(windowVectors[0][0], windowVectors[0][1]);
			glVertex2f(windowVectors[1][0], windowVectors[0][1]);
			glVertex2f(windowVectors[1][0], windowVectors[1][1]);
			glVertex2f(windowVectors[0][0], windowVectors[1][1]);
			glEnd();
			break;

		case bottomRight:
			//In this case, our two points are the bottom right and the top left
			glBegin(GL_LINE_LOOP);
			glVertex2f(windowVectors[1][0], windowVectors[0][1]);
			glVertex2f(windowVectors[0][0], windowVectors[0][1]);
			glVertex2f(windowVectors[0][0], windowVectors[1][1]);
			glVertex2f(windowVectors[1][0], windowVectors[1][1]);
			glEnd();
			break;

		case topLeft:
			//Here, the points are the top left and the bottom right
			glBegin(GL_LINE_LOOP);
			glVertex2f(windowVectors[0][0], windowVectors[1][1]);
			glVertex2f(windowVectors[1][0], windowVectors[1][1]);
			glVertex2f(windowVectors[1][0], windowVectors[0][1]);
			glVertex2f(windowVectors[0][0], windowVectors[0][1]);
			glEnd();
			break;

		case topRight:
			//And finally, the points here are the top right and the bottom left.
			glBegin(GL_LINE_LOOP);
			glVertex2f(windowVectors[1][0], windowVectors[1][1]);
			glVertex2f(windowVectors[0][0], windowVectors[1][1]);
			glVertex2f(windowVectors[0][0], windowVectors[0][1]);
			glVertex2f(windowVectors[1][0], windowVectors[0][1]);
			glEnd();
			break;
		}
	}
}

void Rendering::GetWindowBounds(Element * element, int * returnArray) {
	float floatWidth, floatHeight;
	float relativeSize[2];
	float relativePosition[2];
	int tempArray[2];
	int tempSizeArray[2];


	switch (element->windowType) {

	case WINDOW_DISCRETE:

		//The first corner of the window
		relativePosition[0] = element->originPosition[0] +
			PixelToFloat1D(element->elementInfo->x,
				element->screenWidth)
			* element->xModifier;

		relativePosition[1] = element->originPosition[1] +
			PixelToFloat1D(element->elementInfo->y,
				element->screenHeight)
			* element->yModifier;

		//The second corner of the window
		relativeSize[0] = element->originPosition[0] +
			PixelToFloat1D(element->elementInfo->x +
				element->elementInfo->width,
				element->screenWidth)
			* element->xModifier;
		relativeSize[1] = element->originPosition[1] +
			PixelToFloat1D(element->elementInfo->y +
				element->elementInfo->height,
				element->screenHeight)
			* element->yModifier;

		FloatToPixel(relativePosition[0],
			relativePosition[1],
			tempArray);

		FloatToPixel(relativeSize[0],
			relativeSize[1],
			tempSizeArray);

		break;

	case WINDOW_SCALING:
		//The first corner of the window
		relativePosition[0] = element->originPosition[0] +
			GetRelativeFloat1D(element->elementInfo->x,
				element->elementInfo->parent->elementInfo->width)
			* element->xModifier;
		relativePosition[1] = element->originPosition[1] +
			GetRelativeFloat1D(element->elementInfo->y,
				element->elementInfo->parent->elementInfo->height)
			* element->yModifier;

		//The second corner of the window
		relativeSize[0] = element->originPosition[0] +
			GetRelativeFloat1D(element->elementInfo->x +
				element->elementInfo->width,
				element->elementInfo->parent->elementInfo->width)
			* element->xModifier;
		relativeSize[1] = element->originPosition[1] +
			GetRelativeFloat1D(element->elementInfo->y +
				element->elementInfo->height,
				element->elementInfo->parent->elementInfo->height)
			* element->yModifier;

		FloatToPixel(relativePosition[0],
			relativePosition[1],
			tempArray);

		FloatToPixel(relativeSize[0],
			relativeSize[1],
			tempSizeArray);
		break;

	case WINDOW_FIXED_W:

		//Convert the width into a float value
		floatWidth = element->rendering->PixelToFloat1D(element->elementInfo->
			width,
			element->screenWidth);

		//The first corner of the window
		relativePosition[0] = element->originPosition[0] +
			GetRelativeFloat1D(element->elementInfo->x,
				element->elementInfo->parent->elementInfo->width)
			* element->xModifier;
		relativePosition[1] = element->originPosition[1] +
			GetRelativeFloat1D(element->elementInfo->y,
				element->elementInfo->parent->elementInfo->height)
			* element->yModifier;

		//The second corner of the window
		relativeSize[0] = relativePosition[0] + floatWidth
			* element->xModifier;
		relativeSize[1] = element->originPosition[1] +
			GetRelativeFloat1D(element->elementInfo->y +
				element->elementInfo->height, element->elementInfo->
				parent->elementInfo->height)
			* element->yModifier;

		FloatToPixel(relativePosition[0],
			relativePosition[1],
			tempArray);

		FloatToPixel(relativeSize[0],
			relativeSize[1],
			tempSizeArray);
		break;

	case WINDOW_FIXED_H:

		//Convert the height into a float value
		floatHeight = PixelToFloat1D(element->elementInfo->height,
			element->screenHeight);

		//The first corner of the window
		relativePosition[0] = element->originPosition[0] +
			GetRelativeFloat1D(element->elementInfo->x,
				element->elementInfo->parent->elementInfo->width)
			* element->xModifier;
		relativePosition[1] = element->originPosition[1] +
			GetRelativeFloat1D(element->elementInfo->y,
				element->elementInfo->parent->elementInfo->height)
			* element->yModifier;

		//The second corner of the window
		relativeSize[0] = element->originPosition[0] +
			GetRelativeFloat1D(element->elementInfo->x +
				element->elementInfo->width, element->elementInfo->
				parent->elementInfo->width)
			* element->xModifier;
		relativeSize[1] = relativePosition[1] + floatHeight
			* element->yModifier;

		FloatToPixel(relativePosition[0],
			relativePosition[1],
			tempArray);

		FloatToPixel(relativeSize[0],
			relativeSize[1],
			tempSizeArray);
		break;
	}

	//Work out the boundaries of the box, accounting for situations
	//where the anchor point isn't the bottom left.
	if (tempArray[0] > tempSizeArray[0]) {
		returnArray[1] = tempArray[0];
		returnArray[0] = tempSizeArray[0];
	}
	else if (tempArray[0] < tempSizeArray[0]) {
		returnArray[1] = tempSizeArray[0];
		returnArray[0] = tempArray[0];
	}
	else {
		returnArray[1] = tempArray[0];
		returnArray[0] = tempSizeArray[0];
	}

	if (tempArray[1] > tempSizeArray[1]) {
		returnArray[3] = tempArray[1];
		returnArray[2] = tempSizeArray[1];
	}
	else if (tempArray[1] < tempSizeArray[1]) {
		returnArray[3] = tempSizeArray[1];
		returnArray[2] = tempArray[1];
	}
	else {
		returnArray[3] = tempArray[1];
		returnArray[2] = tempSizeArray[1];
	}
}