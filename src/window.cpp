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

//temporary, used to determine animation speed
#define ANIMATION_SPEED 10

//Standard constructor, taking arguments in terms of pixels A window
//instantiated with this constructor will remain the same size and in the same
//position regardless of the main window resizing.
Window::Window(int x, int y, int width, int height,
		Rendering* rendering, Element* parent) {
	int index = parent->childCount;
	this->windowType = WINDOW_DISCRETE;
	this->Initialise((float)x, (float)y, (float)width, (float)height,
			index, rendering, parent);
}

//Float constructor taking arguments as values between 0 and 1 A window 
//instantiated with this constructor will remain at a relative size and
//position to the main window, according to the float values.
Window::Window(float x, float y, float width, float height,
		Rendering* rendering, Element* parent) {
	int index = parent->childCount;
	this->windowType = WINDOW_SCALING;
	this->Initialise(x, y, width, height, index, rendering, parent);
}

//Composite constructor accepting both float and iteger values. This
//constructor creates a window that remains at the same relative space when
//its parent window is resized, however only the width will scale with the
//window.
Window::Window(float x, float y, float width, int height,
		Rendering* rendering, Element* parent) {
	int index = parent->childCount;
	this->windowType = WINDOW_FIXED_H;
	this->Initialise(x, y, width, (float)height, index, rendering,
			parent);
}

//Composite constructor accepting both float and iteger values. This 
//constructor creates a window that remains at the same relative space when
//its parent window is resized, however only the height will scale with the
//window.
Window::Window(float x, float y, int width, float height,
		Rendering* rendering, Element* parent) {
	int index = parent->childCount;
	this->windowType = WINDOW_FIXED_W;
	this->Initialise(x, y, (float)width, height, index, rendering,
			parent);
}

//The initialise function is used for instantiation that is common accross
//all window types.
void Window::Initialise(float x, float y, float width, float height, int index,
		Rendering* rendering, Element* parent) {
	float viewportData[4];

	//Get information about the size of the viewport
	glGetFloatv(GL_VIEWPORT, viewportData);

	this->screenWidth = viewportData[2];
	this->screenHeight = viewportData[3];

	this->border = true;
	this->elementInfo = new window_t;
	this->elementInfo->x = x;
	this->elementInfo->y = y;
	this->elementInfo->width = 0;
	this->elementInfo->height= 0;
	this->elementInfo->index = index;
	this->elementInfo->parent = parent;

	this->targetDimensions.x = x;
	this->targetDimensions.y = y;
	this->targetDimensions.width = width;
	this->targetDimensions.height = height;
	this->rendering = rendering;
	this->childCount = 0;

	this->colour[0] = defaultColour[0];
	this->colour[1] = defaultColour[1];
	this->colour[2] = defaultColour[2];
	this->colour[3] = defaultColour[3];

	//Calculate the origin point and modifiers
	switch(origin) {

		case bottomLeft:	
			rendering->GetRelativeFloat(-1.0f, -1.0f, originPosition, parent->elementInfo);
			xModifier = 1;
			yModifier = 1;
			break;

		case bottomRight:
			rendering->GetRelativeFloat(1.0f, -1.0f, originPosition, parent->elementInfo);
			xModifier = -1;
			yModifier = 1;
			break;

		case topLeft:
			rendering->GetRelativeFloat(-1.0f, 1.0f, originPosition, parent->elementInfo);
			xModifier = 1;
			yModifier = -1;
			break;

		case topRight:
			rendering->GetRelativeFloat(1.0f, 1.0f, originPosition, parent->elementInfo);
			xModifier = -1;
			yModifier = -1;
			break;
	}

	this->animState = opening;
}

//Destructor
Window::~Window() {
	for (int currentChild = 0; currentChild < childCount; ++currentChild) {
		this->RemoveChild(currentChild);
	}
	delete this->elementInfo;
}

//Called whenever the window needs to be drawn
void Window::Draw() {
	
	//Check to see if we're currently opening or closing the window
	//If we are, run the animation for a frame
	if (animState == opening) {
		this->inAnimation = this->Create();
	} else if (animState == closing) {
		this->inAnimation = this->Close();
	}
	
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

	window_t* parentInfo = this->elementInfo->parent->elementInfo;

	switch (windowType) {
		case WINDOW_DISCRETE:
			//We need to convert the pixel co-ordinates to 
			//world co-ordinates
			int parentPixel[2];
			/*this->rendering->FloatToPixel(parentInfo->x, 
					parentInfo->y, 
					parentPixel);*/
			this->rendering->FloatToPixel(
					this->originPosition[0],
					this->originPosition[1],
					parentPixel);

			//The bottom left corner of the window
			this->rendering->PixelToFloat(
					parentPixel[0] + 
					this->elementInfo->x * xModifier, 
					parentPixel[1] + 
					this->elementInfo->y * yModifier,
				       	tempArray);	
	
			//The top right corner of the window
			this->rendering->PixelToFloat(parentPixel[0] + 
					this->elementInfo->x + 
						this->elementInfo->width, 
					parentPixel[1] + 
						this->elementInfo->y + 
						this->elementInfo->height, 
					tempSecondArray);
			break;

		case WINDOW_SCALING:
			//The bottom left corner of the window
			this->rendering->GetRelativeFloat(this->elementInfo->x, 
					this->elementInfo->y, 
					tempArray, 
					parentInfo);

			//The top right corner of the window
			this->rendering->GetRelativeFloat(this->elementInfo->x + 
						this->elementInfo->width,
					this->elementInfo->y + 
						this->elementInfo->height,
					tempSecondArray, 
					parentInfo);
			break;

		case WINDOW_FIXED_H:
			
			//Convert the height into a float value
			floatHeight = this->rendering->PixelToFloat1D(
					this->rendering->FloatToPixel1D(this->elementInfo->y, 
						this->screenHeight) +
				      		this->elementInfo->height,
						this->screenHeight);
			
			//The bottom left corner of the window
			this->rendering->GetRelativeFloat(this->elementInfo->x, 
					this->elementInfo->y, 
					tempArray, 
					parentInfo);

			//The top right corner of the window
			this->rendering->GetRelativeFloat(this->elementInfo->x +
				    	this->elementInfo->width,
					floatHeight,
					tempSecondArray, 
					parentInfo);
			break;
		
		case WINDOW_FIXED_W:
			
			//Convert the width into a float value
			floatWidth = this->rendering->PixelToFloat1D(
					this->rendering->FloatToPixel1D(this->elementInfo->x,
						this->screenWidth) + 
						this->elementInfo->width,
			      			this->screenWidth);

			//The bottom left corner of the window	
			this->rendering->GetRelativeFloat(this->elementInfo->x,
					this->elementInfo->y,
					tempArray,
					parentInfo);

			//The top right corner of the window
			this->rendering->GetRelativeFloat(floatWidth,
					this->elementInfo->y + 
					this->elementInfo->height,
					tempSecondArray, 
					parentInfo);
			break;	
	}

	//Store the point values in the 2D array
	windowVectors[0][0] = tempArray[0];
	windowVectors[0][1] = tempArray[1];
	windowVectors[1][0] = tempSecondArray[0];
	windowVectors[1][1] = tempSecondArray[1];
	
	
	//Set the colour
	glColor4ub(this->colour[0], 
			this->colour[1], 
			this->colour[2], 
			this->colour[3]);
	
	glBegin(GL_QUADS);
	glVertex2f(windowVectors[0][0], windowVectors[0][1]);
	glVertex2f(windowVectors[1][0], windowVectors[0][1]);
	glVertex2f(windowVectors[1][0], windowVectors[1][1]);
	glVertex2f(windowVectors[0][0], windowVectors[1][1]);
	glEnd();

	//DEBUG
	/* cout << "********************************************" << endl
	<< "Window: " << this->elementInfo->index << endl
	<< "Position:" << endl
	<< windowVectors[0][0] << "," << windowVectors[0][1] << "," << windowVectors[1][0] << "," << windowVectors[1][1] << endl
	<< "Parent Data: " << endl
	<< this->elementInfo->parent->elementInfo->x << "," << this->elementInfo->parent->elementInfo->y << endl
	<< this->elementInfo->parent->elementInfo->width << "," << this->elementInfo->parent->elementInfo->height << endl
	<< "********************************************" << endl; */
	//Draw the border
	if (this->border == true) {	
		glColor4ub(this->borderColour[0],
				this->borderColour[1],
				this->borderColour[2],
				this->borderColour[3]);

		glBegin(GL_LINE_LOOP);
		glVertex2f(windowVectors[0][0], windowVectors[0][1]);
		glVertex2f(windowVectors[1][0], windowVectors[0][1]);
		glVertex2f(windowVectors[1][0], windowVectors[1][1]);
		glVertex2f(windowVectors[0][0], windowVectors[1][1]);
		glEnd();
	}

	//Draw each of the child elements
	for (int i = 0; i < childCount; ++i){
		this->children[i]->Draw();
	}
}

//Resizes the window overtime to achieve an animation
bool Window::Create() {
	//Firstly, update the x and y co-ordinates. That way, child elements
	//can be moved whilst still animating.
	this->targetDimensions.x = this->elementInfo->x;
	this->targetDimensions.y = this->elementInfo->y;
	
	float widthIncrement = (float)this->targetDimensions.width / 
					ANIMATION_SPEED;
	float heightIncrement = (float)this->targetDimensions.height /
					ANIMATION_SPEED;

	if (this->elementInfo->width != this->targetDimensions.width) {
		if (this->elementInfo->width + widthIncrement > this->targetDimensions.width)
			this->elementInfo->width = this->targetDimensions.width;
		else
			this->elementInfo->width += widthIncrement;

		return true;
	} else if (this->elementInfo->height != this->targetDimensions.height) {
		if (this->elementInfo->height + heightIncrement > this->targetDimensions.height)
			this->elementInfo->height = targetDimensions.height;
		else
			this->elementInfo->height += heightIncrement;

		return true;
	} else {
		this->inAnimation = false;
		this->animState = open;
		return false;
	}
}

//Resizes the window overtime to achieve an animation
bool Window::Close() {
	float widthDecrement = (float)this->targetDimensions.width /
					ANIMATION_SPEED;
	float heightDecrement = (float)this->targetDimensions.height /
					ANIMATION_SPEED;

	if (this->elementInfo->height != 0) {
		if (this->elementInfo->height - heightDecrement < 0)
			this->elementInfo->height = 0;
		else
			this->elementInfo->height -= heightDecrement;
		return true;
	} else if (this->elementInfo->width != 0) {
		if (this->elementInfo->width - widthDecrement < 0)
			this->elementInfo->width = 0;
		else
			this->elementInfo->width -= widthDecrement;
		return true;
	} else {
		this->inAnimation = false;
		this->animState = closed;
		return false;
	}
}

//Called whenever the window needs to be resized
void Window::Resize(int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;
}


//Checks to see if the mouse falls within the bounds of the window. If it does,
//the function will return true. In addition, it will set the clickLocation
//array to the x and y values of the mouse relative to the top left corner of
//the window.
int Window::Click(int x, int y, int* clickLocation) {

	//If we're in an animation, ignore everything and return false.
	if (this->inAnimation)
		return 0;
	
	int tempArray[2];
	int tempSizeArray[2];
	float relativePosition[2];
	float relativeSize[2];
	float floatHeight = 0;
	float floatWidth = 0;
	window_t* parentInfo = this->elementInfo->parent->elementInfo;

	switch (windowType) {
		
		case WINDOW_DISCRETE:
			int parentPixel[2];
			this->rendering->FloatToPixel(parentInfo->x, parentInfo->y, parentPixel);
			if (x < parentPixel[0] + this->elementInfo->x + 
					this->elementInfo->width
				&& x > parentPixel[0] + this->elementInfo->x
				&& y < parentPixel[1] + this->elementInfo->y +
					this->elementInfo->height
				&& y > parentPixel[1] + this->elementInfo->y) {

				clickLocation[0] = x - (parentPixel[0] +
						this->elementInfo->x);
				clickLocation[1] = y - (parentPixel[1] +
						this->elementInfo->y);
				
				//Check to see if any of the child items have
				//been clicked and are draggable.
				for (int i = 0; i < childCount; ++i) 
					if (children[i]->Click(
							x, 
							y, 
							clickLocation
							) == 1) {
						clickLocation[0] = x - (parentPixel[0] +
								this->elementInfo->x);
						clickLocation[1] = y - (parentPixel[1] +
								this->elementInfo->y);
						return 1;	
					}
				//Otherwise, If this window can be dragged, we return 1
				if (this->draggable)
					return 1;
				else 
					return 0;
			}
			break;

	 	case WINDOW_SCALING:
			this->rendering->GetRelativeFloat(this->elementInfo->x,
					this->elementInfo->y,
					relativePosition,
					parentInfo);

			this->rendering->GetRelativeFloat(this->elementInfo->x +
						this->elementInfo->width,
					this->elementInfo->y + 
						this->elementInfo->height,
					relativeSize,
					parentInfo);

			this->rendering->FloatToPixel(relativePosition[0],
					relativePosition[1],
					tempArray);

			this->rendering->FloatToPixel(relativeSize[0],
					relativeSize[1],
					tempSizeArray);

			if (x < tempSizeArray[0]
				&& x > tempArray[0]
				&& y < tempSizeArray[1]
				&& y > tempArray[1]) {

				clickLocation[0] = x - tempArray[0];
				clickLocation[1] = y - tempArray[1];
			
				//Check to see if any of the child items have
				//been clicked and are draggable.
				for (int i = 0; i < childCount; ++i) 
					if (children[i]->Click(
							x, 
							y, 
							clickLocation
							) == 1) {
						clickLocation[0] = x - tempArray[0];
						clickLocation[1] = y - tempArray[1];
						return 1;	
					}
				//Otherwise, If this window can be dragged, we return 1
				if (this->draggable)
					return 1;
				else 
					return 0;
				
			}
			break;

	 	case WINDOW_FIXED_H:
			floatHeight = this->rendering->PixelToFloat1D(
					this->rendering->FloatToPixel1D(
						this->elementInfo->y,
						this->screenHeight) + 
						this->elementInfo->height,
					this->screenHeight);

			this->rendering->GetRelativeFloat(this->elementInfo->x,
					this->elementInfo->y,
					relativePosition,
					parentInfo);

			this->rendering->GetRelativeFloat(this->elementInfo->x +
					this->elementInfo->width,
					floatHeight,
					relativeSize,
					parentInfo);
			
			this->rendering->FloatToPixel(relativePosition[0],
					relativePosition[1],
					tempArray);
			
			this->rendering->FloatToPixel(relativeSize[0],
					relativeSize[1],
					tempSizeArray);
			
			if (x < tempSizeArray[0]
				&& x > tempArray[0]
				&& y < tempSizeArray[1]
				&& y > tempArray[1]) {

				clickLocation[0] = x - tempArray[0];
				clickLocation[1] = y - tempArray[1];
			
				//Check to see if any of the child items have
				//been clicked and are draggable.
				for (int i = 0; i < childCount; ++i) 
					if (children[i]->Click(
							x, 
							y, 
							clickLocation
							) == 1) {
						clickLocation[0] = x - tempArray[0];
						clickLocation[1] = y - tempArray[1];
						return 1;	
					}
				//Otherwise, If this window can be dragged, we return 1
				if (this->draggable)
					return 1;
				else 
					return 0;
				
			}
			break;

	 	case WINDOW_FIXED_W:
			floatWidth = this->rendering->PixelToFloat1D(
					this->rendering->FloatToPixel1D(
						this->elementInfo->x,
						this->screenWidth) + 
						this->elementInfo->width,
					this->screenWidth);

			this->rendering->GetRelativeFloat(this->elementInfo->x,
					this->elementInfo->y,
					relativePosition,
					parentInfo);

			this->rendering->GetRelativeFloat(floatWidth,
					this->elementInfo->y +
					this->elementInfo->height,
					relativeSize,
					parentInfo);
			
			this->rendering->FloatToPixel(relativePosition[0],
					relativePosition[1],
					tempArray);

			this->rendering->FloatToPixel(relativeSize[0],
					relativeSize[1],
					tempSizeArray);
			
			if (x < tempSizeArray[0]
				&& x > tempArray[0]
				&& y < tempSizeArray[1]
				&& y > tempArray[1]) {

				clickLocation[0] = x - tempArray[0];
				clickLocation[1] = y - tempArray[1];
			
				//Check to see if any of the child items have
				//been clicked and are draggable.
				for (int i = 0; i < childCount; ++i) 
					if (children[i]->Click(
							x, 
							y, 
							clickLocation
							) == 1) {
						clickLocation[0] = x - tempArray[0];
						clickLocation[1] = y - tempArray[1];
						return 1;	
					}
				//Otherwise, If this window can be dragged, we return 1
				if (this->draggable)
					return 1;
				else 
					return 0;
				
			}
			break;
	}
	cout << "No Collision Detected" << endl;
	return 0;
}

//Moves the window to the specified position. Eventually, this could be
//extended to allow for moving animations and the like. X and Y are screen
//position values given in pixels and so need to be converted if the window
//is a scaling type.
void Window::Move(int x, int y) {
	window_t* parentInfo = this->elementInfo->parent->elementInfo;
	
	if (windowType == WINDOW_DISCRETE) {
		int parentPixel[2];
		this->rendering->FloatToPixel(parentInfo->x, parentInfo->y, parentPixel);
		this->elementInfo->x = parentPixel[0] + x;
		this->elementInfo->y = parentPixel[1] + y;
	} else {
		//All other window types use float values to store their
		//position.
		float tempArray[2];
		float relativePosition[2];
		this->rendering->PixelToFloat(x, y, tempArray);
		this->rendering->GetRelativeFloat(tempArray[0], tempArray[1],
					relativePosition, parentInfo);
		this->elementInfo->x = relativePosition[0];
		this->elementInfo->y = relativePosition[1];
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
		//update the child element's indices
		for (int i = 0; i < childCount; ++i)
			this->children[i]->elementInfo->index = i;
	}
}

//Sets the colour of the window
void Window::SetColour(int r, int g, int b, int a) {	
	this->colour[0] = r;
	this->colour[1] = g;
	this->colour[2] = b;
	this->colour[3] = a;

	for (int curChild = 0; curChild < childCount; ++curChild)
		this->children[curChild]->SetColour(r, g, b, a);
}

//Enables/Disables the border and optionally sets its colour
void Window::SetBorder(bool enabled, int* colour) {
	this->border = enabled;
	if (colour != 0) {
		this->borderColour[0] = colour[0];
		this->borderColour[1] = colour[1];
		this->borderColour[2] = colour[2];
		this->borderColour[3] = colour[3];
	}

	for (int curChild = 0; curChild < childCount; ++curChild)
		this->children[curChild]->SetBorder(enabled, colour);
}

//For the window class, this method does not do anything
void Window::PassData(void * input) {
	int placeholder = *(int*)input;
	placeholder++;	//Just to shut up YCM
}
