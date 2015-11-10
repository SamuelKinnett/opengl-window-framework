#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <vector>
#include <iostream>

//Custom classes
#include "window.h"

using namespace std;

#define FPS 60
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define TRUE 1
#define FALSE 0

int windowHandle = 0;
int draggingWindow = FALSE;	//Is the user currently dragging a window?
int activeWindow = -1;		//The window currently being interacted with
int mouseRelativePosition[2];	//The position of the mouse relative to the window being dragged
int screenSize[2] = {DEFAULT_WIDTH, DEFAULT_HEIGHT};

void Initialise(int, char*[]);
void InitWindow(int, char*[]);
bool InitOpenGL();
void Update();
void Render();
void Resize(int, int);
void MainLoop(int);

//Input handling
void HandleMouseClick(int, int, int, int);
void HandleMouseMoving(int, int);

vector <Window*> windows; //Points to all of the current windows

int main(int argc, char* argv[]) {
	Initialise(argc, argv);
}

//Initialise the window and OpenGL
void Initialise(int argc, char* argv[]) {
	InitWindow(argc, argv);
	InitOpenGL();

	//TESTING
	//Add two windows to the beginning of the windows vector

	windows.push_back(new Window(0.0f, 0.0f, 0.5f, 0.5f));
	windows.push_back(new Window(10, 10, 50, 50));

	//Start running GLut's loop
	glutMainLoop();
}

//Create a new window using FreeGLut
void InitWindow(int argc, char* argv[]) {
	glutInit(&argc, argv);

	//Set the window size
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	//Set the display mode to be double buffered
	glutInitDisplayMode(GLUT_DOUBLE);
	//Create a window with the specified title
	windowHandle = glutCreateWindow("Test");
	//Check for errors
	if(windowHandle < 1) {
		fprintf(stderr, "ERROR: Could not create a new rendering window.\n");
		exit(EXIT_FAILURE);
	}

	//Set the function to be called when Glut needs to update the screen
	glutDisplayFunc(Render);
	//Sets the MainLoop function to be called after a specified number
	//of milliseconds. The time is calculated by 1000 / FPS so that
	//the main loop is run at the specified framerate.
	glutTimerFunc(1000 / FPS, MainLoop, 0);
	//Sets the HandleMouseClick function to be called whenever any mouse
	//button is clicked.
	glutMouseFunc(HandleMouseClick);
	//Sets the HandleMouseMoving function to be called whenever the mouse
	//is moved whilst a mouse button is held down
	glutMotionFunc(HandleMouseMoving);
	//Sets the Resize function to be called whenever the main window is
	//resized.
	glutReshapeFunc(Resize);
}

//Initialises OpenGL
bool InitOpenGL() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Initialise the OpenGL clear colour, in this case black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Initialise blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Code goes here to check for errors

	return true;
}

//Handles any input and/or updates that need to be made
void Update() {
	//Nothing needs doing at the moment!
}

//Handles any rendering to be done
void Render() {
	//Clear the colour buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Drawing code goes here
	for(vector<Window*>::iterator it = windows.begin(); it != windows.end(); ++it) {
		Window& currentWindow = **it;	//The current window being drawn
		currentWindow.Draw();
	}

	//Update the screen by swapping the buffers
	glutSwapBuffers();
}

//The main program loop
void MainLoop(int val) {

	Update();
	Render();

	//Set another timed function call. In this way, the main loop is
	//looped at the framerate specified by the FPS constant
	glutTimerFunc(1000 / FPS, MainLoop, val);
}

//Handles mouse clicks
void HandleMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		activeWindow = -1;	//Reset the active window
		for (uint i = 0; i < windows.size(); ++i) {
			//Loop through every window and see if the mouse collides with them.
			//If it does, make that the current window being dragged and move it
			if (windows[i]->CheckMouseCollision(x, screenSize[1] - y, mouseRelativePosition))
				activeWindow = i;
		}
		if (activeWindow > -1) {
			//If the user has clicked on a window, move that window to the back of
			//the vector, meaning it will be rendered last and therefore "at the
			//front"
			windows.push_back(windows[activeWindow]);
			windows.erase(windows.begin() + activeWindow);

			if (state == GLUT_DOWN) {
				draggingWindow = TRUE;
			} else {
				draggingWindow = FALSE;
			}
		} else {
			draggingWindow = FALSE;
		}
	}
	cout << "Mouse X: " << x << endl;
	cout << "Mouse Y: " << y << endl;
	cout << "Active Window: " << activeWindow << endl;
}

//Handles the mouse moving while a mouse button is pressed
void HandleMouseMoving(int x, int y) {
	cout << "Wew" << endl;
	if (draggingWindow) {
		windows[activeWindow]->Move(x - mouseRelativePosition[0], (screenSize[1] - y) - mouseRelativePosition[1]);
	}
}

//Handles the freeglut window being resized
void Resize(int width, int height) {
	for (uint i = 0; i < windows.size(); ++i)
		windows[i]->Resize(width, height);
	screenSize[0] = width;
	screenSize[1] = height;
}
