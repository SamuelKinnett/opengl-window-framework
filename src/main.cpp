#include "main.h"
using namespace std;

#define FPS 60
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define SCREEN_ORIGIN_X -1
#define SCREEN_ORIGIN_Y -1

int windowHandle = 0;
bool draggingWindow = false;	//Is the user currently dragging a window?
int activeWindow = -1;		//The window currently being interacted with
int mouseRelativePosition[2];	//The position of the mouse relative to the
				// window being dragged
int screenSize[2] = {DEFAULT_WIDTH, DEFAULT_HEIGHT};

Container* GUI;		//The main GUI, holding all of the windows.
Rendering* rendering;	//Rendering class, used for world to screenspace
			// conversion
std::function<void(Button*)> buttonCallback;

int main(int argc, char* argv[]) {
	Initialise(argc, argv);
}

//Initialise the window and OpenGL
void Initialise(int argc, char* argv[]) {
	InitWindow(argc, argv);
	InitOpenGL();

	//Create a new rendering class that will be used by all child elements
	rendering = new Rendering(screenSize[0], screenSize[1]); 
	
	//Create the GUI
	GUI = new Container(-1.0f, -1.0f, 2.0f, 2.0f, rendering);

	//TESTING
	//Add two windows
	Window* window1 = GUI->InstantiateWindow(0.0f, 0.0f, 0.5f, 0.5f, "Main Window");
	Window* window2 = GUI->InstantiateWindow(-1.0f, -0.3f, 0.6f, 0.3f, "Another Window");
	
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
	//Sets the HandleButtonPress function to be called whenever a button is
	//pressed.
	glutKeyboardFunc(HandleButtonPress);
	//Sets the HandleSpecialButtonPress function to be called whenever a 
	//special key such as the arrow keys or the escape key is pressed
	glutSpecialFunc(HandleSpecialButtonPress);
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

	//Initalise texturing
	glEnable(GL_TEXTURE_2D);	
	
	//Code goes here to check for errors

	return true;
}

//Handles any input and/or updates that need to be made
void Update() {
	GUI->Update();
}

//Handles any rendering to be done
void Render() {
	//Clear the colour buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Drawing code goes here
	GUI->Draw();

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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (GUI->Click(x, screenSize[1] - y, mouseRelativePosition) == 2) {
			draggingWindow = true;
		}
		else {
			draggingWindow = false;
		}
	}

	//cout << "Mouse X: " << x << endl;
	//cout << "Mouse Y: " << y << endl;
	//cout << "Active Window: " << activeWindow << endl;
}

//Handles the mouse moving while a mouse button is pressed
void HandleMouseMoving(int x, int y) {
	if (draggingWindow) {
		GUI->Move(x - mouseRelativePosition[0],
			(screenSize[1] - y) - mouseRelativePosition[1]);
		//windows[activeWindow]->Move(x - mouseRelativePosition[0], 
		//	(screenSize[1] - y) - mouseRelativePosition[1]);
	}
}

void HandleButtonPress(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		//Escape key, close the program
		delete GUI;
		exit(EXIT_SUCCESS);
	case 'w':
		//Create a new window
		Window* tempWindow = GUI->InstantiateWindow(0, 0, 0.5f, 0.5f, "Window");
		tempWindow->Resize(screenSize[0], screenSize[1]);
	}
}

void HandleSpecialButtonPress(int key, int x, int y) {

}

//Handles the freeglut window being resized
void Resize(int width, int height) {
	//Resize the rendering class
	rendering->Resize(width, height);
	
	GUI->Resize(width, height);
	/*for (uint i = 0; i < windows.size(); ++i)
		windows[i]->Resize(width, height);*/

	screenSize[0] = width;
	screenSize[1] = height;
	//Change the viewport size so that discrete/scaling windows work
	//properly
	glViewport(0, 0, width, height);
}
