#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#define FPS 60
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

int windowHandle = 0;

void Initialise(int, char*[]);
void InitWindow(int, char*[]);
bool InitOpenGL();
void Update();
void Render();
void MainLoop(int);

int main(int argc, char* argv[]) {
	Initialise(argc, argv);
}

//Initialise the window and OpenGL
void Initialise(int argc, char* argv[]) {
	InitWindow(argc, argv);
	InitOpenGL();

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
}

//Initialises OpenGL
bool InitOpenGL() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Initialise the OpenGL clear colour, in this case black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
	//Draw a triangle to test that the code works
	glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.5f, 0.0f);
		glVertex2f(0.0f, 0.5f);
	glEnd();

	//Update the screen by swapping the buffers
	glutSwapBuffers();
}

//The main program loop
void MainLoop(int val) {

	Update();
	Render();

	//Set another timed function call. In this way, the main loop is
	//looped at the framerate speicifed by the FPS constant
	glutTimerFunc(1000 / FPS, MainLoop, val);
}
