//*****************************************************************************
//The Button class
//*****************************************************************************

#include "button.h"
#include "rendering.h"
#include <GL/gl.h>

Button::Button(float x, float y, float width, float height,
		Rendering* rendering, std::function<void()>* function) {

	this->elementInfo.x = x;
	this->elementInfo.y = y;
	this->elementInfo.width = width;
	this->elementInfo.height = height;
	this->rendering = rendering;
	this->function = function;

	this->colour[0] = this->defaultColour[0];
	this->colour[1] = this->defaultColour[1];
	this->colour[2] = this->defaultColour[2];
	this->colour[3] = this->defaultColour[3];
}

//Draws the button to the screen
void Button::Draw(window_t parentInfo) {
	float buttonPositions[2][2];	//Stores the co-ordinates of the
	//bottom left and top right corners of the button.
	
	float tempArray[2];	//Stores values before they are inserted into
	//the buttonPositions array
	
	//The bottom left corner of the window
	this->rendering->GetRelativeFloat(this->elementInfo.x, 
					this->elementInfo.y, 
					tempArray, 
					parentInfo);
	buttonPositions[0][0] = tempArray[0];
	buttonPositions[0][1] = tempArray[1];

	//The top right corner of the window
	this->rendering->GetRelativeFloat(this->elementInfo.x + 
						this->elementInfo.width,
					this->elementInfo.y + 
						this->elementInfo.height,
					tempArray, 
					parentInfo);
	buttonPositions[1][0] = tempArray[0];
	buttonPositions[1][1] = tempArray[1];
	
	//Set the colour
	glColor4ub(this->colour[0], 
			this->colour[1], 
			this->colour[2], 
			this->colour[3]);
	
	glBegin(GL_QUADS);
	glVertex2f(buttonPositions[0][0], buttonPositions[0][1]);
	glVertex2f(buttonPositions[1][0], buttonPositions[0][1]);
	glVertex2f(buttonPositions[1][0], buttonPositions[1][1]);
	glVertex2f(buttonPositions[0][0], buttonPositions[1][1]);
	glEnd();
	
	//Draw the border
	if (this->border == true) {	
		glColor4ub(this->borderColour[0],
				this->borderColour[1],
				this->borderColour[2],
				this->borderColour[3]);

		glBegin(GL_LINE_LOOP);
		glVertex2f(buttonPositions[0][0], buttonPositions[0][1]);
		glVertex2f(buttonPositions[1][0], buttonPositions[0][1]);
		glVertex2f(buttonPositions[1][0], buttonPositions[1][1]);
		glVertex2f(buttonPositions[0][0], buttonPositions[1][1]);
		glEnd();
	}
}

void Button::Resize(int screenWidth, int screenHeight) {
	//Does nothing for now
}

void Button::AddChild(Element* newChild) {
	//Does nothing for now
}

void Button::RemoveChild(int index) {
	//Does nothing for now
}

int Button::Click(int x, int y, int* clickLocation, window_t parentInfo) {

	float relativePosition[2];	//float storing the postion of the 
	//bottom left corner of the button in the world
	float relativeSize[2];		//float storing the position of the
	//top right corner of the button in the world
	int tempArray[2];	//used to store the position of the bottom
	//left corner of the button as a pixel co-ordinate
	int tempSizeArray[2];	//used to store the position of the top right
	//corner of the button as a pixel co-ordinate

	this->rendering->GetRelativeFloat(this->elementInfo.x,
			this->elementInfo.y,
			relativePosition,
			parentInfo);

	this->rendering->GetRelativeFloat(this->elementInfo.x +
				this->elementInfo.width,
			this->elementInfo.y + 
				this->elementInfo.height,
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
	
		std::function<void()> tempFunction = *function;	
		tempFunction();

		return 1;
	}
	return 0;
}

void Button::Move(int x, int y, window_t parentInfo) {
	float tempArray[2];	//Stores the results of converting the move
	//destination into a float co-ordinate
	float relativePosition[2];	//Stores the position of the bottom
	//left corner of the button

	this->rendering->PixelToFloat(x, y, tempArray);
	this->rendering->GetRelativeFloat(tempArray[0], tempArray[1],
				relativePosition, parentInfo);
	this->elementInfo.x = relativePosition[0];
	this->elementInfo.y = relativePosition[1];
}

void Button::SetColour(int r, int g, int b, int a) {
	this->colour[0] = r;
	this->colour[1] = g;
	this->colour[2] = b;
	this->colour[3] = a;
}

void Button::SetBorder(bool enabled, int* colour) {
	this->border = enabled;
	if (colour != 0) {
		this->borderColour[0] = colour[0];
		this->borderColour[1] = colour[1];
		this->borderColour[2] = colour[2];
		this->borderColour[3] = colour[3];
	}
}

//PassData allows the function called by the button to be changed.
void Button::PassData(void * newFunction) {
	this->function = (std::function<void()>*)newFunction;
}
