//This structure provides a way to pass information about the parent element
//to its children. It stores the x and y positions, the width and the height
//of the element in worldspace.

#ifndef WINDOW_INFO_H
#define WINDOW_INFO_H

struct window_t {
public:
	float x;	//The x and Y co-ordinates of the lower left corner
	float y;

	float width;	//The width and height of the window
	float height;
};

#endif
