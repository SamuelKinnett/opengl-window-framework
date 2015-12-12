#include "container.h"
#include <iostream>

Container::Container(float x, float y, float width, float height,
		Rendering* rendering, GLFont* titleFont) {

	this->elementInfo = new window_t;

	this->elementInfo->x = x;
	this->elementInfo->y = y;
	this->elementInfo->width = width;
	this->elementInfo->height = height;
	
	this->rendering = rendering;
	this->titleFont = titleFont;

	childCount = 0;
	activeWindow = -1;
	movingWindow = false;
}

Container::~Container() {

	delete this->elementInfo;

	for (int curChild = 0; curChild < childCount; ++curChild) {
		delete this->children[curChild];
	}
}

void Container::Draw() {
	for (int curChild = 0; curChild < childCount; ++curChild) {
		this->children[curChild]->Draw();
	}
}

bool Container::Create() {
	return false;
}

bool Container::Close() {
	return false;
}

void Container::Resize(int screenWidth, int screenHeight) {
	for (int curChild = 0; curChild < childCount; ++curChild)
		this->children[curChild]->Resize(screenWidth, screenHeight);
}

void Container::AddChild(Element* newChild) {
	this->children.push_back(newChild);
	this->childCount += 1;
}

//No longer used
void Container::RemoveChild(int){}

void Container::RemoveChild(Element* child) {
	int index = child->elementInfo->index;
	if (index < childCount) {
		for (int i = 0; i < childCount; ++i) {
			if (this->children[i]->elementInfo->index == index) {
				delete this->children[i];
				this->children.erase(this->children.begin() + i);
				this->childCount -= 1;
				//Update the indicies of the child items
				for (int curChild = 0; curChild < childCount; ++curChild)
					this->children[curChild]->elementInfo->index = curChild;
			}
		}
	}
}

int Container::Click(int x, int y, int* clickLocation) {
	
	int clickResult = 0;
	this->activeWindow = -1;
	for (int curWindow = this->childCount - 1; curWindow > -1; --curWindow) {
		//Loop through and check each window to see if it's been
		//clicked.
		clickResult = children[curWindow]->Click(x, y, clickLocation);

		if (clickResult > 0) {
			this->activeWindow = curWindow;
			break;
		}
	}
	if (this->activeWindow > -1) {
		//If the user has clicked on a window, move it to the
		//back of the vector, causing it to be drawn last and
		//therefore at the "front".

		this->children.push_back(children[this->activeWindow]);
		this->children.erase(this->children.begin() +
			this->activeWindow);

		//The active window will now be incorrect, so let's
		//update it.
		this->activeWindow = children.size() - 1;

		if (clickResult == 2)
			return 2;
		else if (clickResult == 1)
			return 1;
	} else {
		//Nothing was clicked, return 0
		return 0;
	}
}

void Container::Move(int x, int y) {
	this->children[this->activeWindow]->Move(x, y);
}

void Container::SetColour(int r, int g, int b, int a) {
	//Doesn't do anything at the moment
	this->colour[0] = r;
	this->colour[1] = g;
	this->colour[2] = b;
	this->colour[3] = a;
}

void Container::SetBorder(bool enabled, int* colour) {
	//Doesn't do anything at the moment
	this->border = enabled;
	this->colour[0] = colour[0];
	this->colour[1] = colour[1];
	this->colour[2] = colour[2];
	this->colour[3] = colour[3];
}

void Container::PassData(void * newData) {
	//Doesn't do anything at the moment
	int toShutUpYCM = *(int*)newData;
	++toShutUpYCM;
}

void Container::ButtonCallback(Button* clickedButton) {
	std::cout << "A button was clicked!" << std::endl;
	switch (clickedButton->buttonType) {
		//This is where the user can specify the functions to be executed for
		//each button type.
	case 1:
		//Close button
		flaggedForDeletion.push_back(clickedButton->elementInfo->parent);
		break;
	}
}

//Called every time to glut loop function is ran. This is where the user can
//add any window-relevant code.
void Container::Update()
{
	for (int i = 0; i < flaggedForDeletion.size(); ++i) {
		this->RemoveChild(flaggedForDeletion[i]);
		flaggedForDeletion.erase(flaggedForDeletion.begin() + i);
	}
}

//This method allows the user to quickly and easily create new windows. It
// returns a pointer to the newly added window, to allow for easy manipulation.
Window* Container::InstantiateWindow(float x, float y, float width, float height, std::string windowText) {
	
	//Create the base window
	this->AddChild(new Window(x, y, width, height, this, rendering, bottomLeft));
	Element* currentWindow = this->children[childCount - 1];
	currentWindow->draggable = false;

	//Add a close button
	currentWindow->AddChild(new Button(0, 0, 30, 30,
		currentWindow,
		rendering, 1, this, topRight));
	currentWindow->children[0]->draggable = false;

	//Add a title bar
	if (windowText != "") {
		currentWindow->AddChild(new Textbox(-1.0f, -1.0f, 2.0f, 30, 
											currentWindow, 
											windowText, 
											rendering,
											titleFont,
											topLeft));
		currentWindow->children[1]->draggable = true;
	}
	else
	{
		currentWindow->AddChild(new Window(-1.0f, 1.0f, 2.0f, 30, currentWindow,
			rendering, topLeft));
		currentWindow->children[1]->draggable = true;
	}

	return (Window*)currentWindow;
}
