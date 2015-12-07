#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <vector>
#include <iostream>

//Custom classes
#include "container.h"
#include "window.h"
#include "windowinfo.h"
#include "rendering.h"
#include "textbox.h"
#include "button.h"

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
void HandleButtonClick(Button*);

