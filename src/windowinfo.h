//This structure provides a way to pass information about the parent element
//to its children. It stores the x and y positions, the width and the height
//of the element, along with the index of the window, in worldspace.

#ifndef WINDOW_INFO_H
#define WINDOW_INFO_H
#include "element.h"

//forward declaration
class Element;

struct window_t {
public:
	float x;	//The x and Y co-ordinates of the lower left corner
	float y;

	float width;	//The width and height of the window
	float height;

	int index;//The index of the parent window in its parent's child
	//vector.
	
	Element* parent;//A pointer to the parent element
};

#endif
