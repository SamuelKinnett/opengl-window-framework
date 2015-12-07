#include "container.h"

Container::Container(float x, float y, float width, float height) {

	this->elementInfo = new window_t;

	this->elementInfo->x = x;
	this->elementInfo->y = y;
	this->elementInfo->width = width;
	this->elementInfo->height = height;

	childCount = 0;
	activeWindow = -1;
	movingWindow = false;
}

void Container::Draw() {
	for (int curChild = 0; curChild < childCount; ++curChild)
	       this->children[curChild]->Draw();	
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

void Container::RemoveChild(int index) {
	if (index < childCount) {
		this->children.erase(this->children.begin() + index);
		this->childCount -= 1;
		//Update the indicies of the child items
		for (int i = 0; i < childCount; ++i)
			this->children[i]->elementInfo->index = i;
	}
}

int Container::Click(int x, int y, int* clickLocation) {
	this->activeWindow = -1;
	for (int curWindow = 0; curWindow < this->childCount; ++curWindow) {
		//Loop through and check each window to see if it's been
		//clicked.
		if (children[curWindow]->Click(x, y, clickLocation))
			this->activeWindow = curWindow;
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