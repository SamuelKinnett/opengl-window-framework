//*********************************************************
//GLFONT.CPP -- glFont routines
//Copyright (c) 1998 Brad Fish
//Copyright (c) 2002 Henri Kyrki
//See glFont.txt for terms of use
//10.5 2002
//*********************************************************

//*********************************************************
//Slight modification has been made to the original implemenation
//1. Removal of the dependency on OpenGL extensions
//2. Change primitive from GL_QUADS to GL_TRIANGLE_STRIP
//3. Added an additional arguement to RenderText to allow font scaling when
//using GLFont
//Minsi Chen
//*********************************************************

//*********************************************************
//Modified rendering to work with my window framework, allowing
//anchoring and proper scaling/rendering.
//
//Sam Kinnett
//*********************************************************

#if _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#if _WIN32
#include <GL/gl.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#endif

#include "GLFont.h"
#include "element.h"
#include "windowinfo.h"
#include <cmath>

//*********************************************************
//GLFontBase
//*********************************************************

GLFontBase::GLFontBase() : ok(FALSE)
{
}

void GLFontBase::CreateImpl(const char *Filename, bool PixelPerfect)
{
	Font.Char = NULL;
	FreeResources();

	FILE *Input;

	//Open font file
	if (fopen_s(&Input, Filename, "rb") != 0)
		throw GLFontError::InvalidFile();

	//Read glFont structure
	fread(&Font, sizeof(GLFONT), 1, Input);
	
	unsigned int tex;
	//Save texture number
	glGenTextures( 1, &tex );
	Font.Tex = tex;

	//Get number of characters
	
	//endian_swap(Font.IntEnd);
	//endian_swap(Font.IntStart);
	int Num = Font.IntEnd - Font.IntStart + 1;
	
	//Allocate memory for characters
	Font.Char = new GLFONTCHAR[Num];

	//Read glFont characters
	fread(Font.Char, sizeof(GLFONTCHAR), Num, Input);

	//Get texture size
	Num = Font.TexWidth * Font.TexHeight * 2;

	//Allocate memory for texture data
	char *TexBytes = new char[Num];

	//Read texture data
	fread(TexBytes, sizeof(char), Num, Input);
	
	//Set texture attributes
	glBindTexture(GL_TEXTURE_2D, Font.Tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	if(PixelPerfect)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Create texture
	glTexImage2D(GL_TEXTURE_2D, 0, 2, Font.TexWidth, Font.TexHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, (void *)TexBytes);

	//Clean up
	delete []TexBytes;
	fclose(Input);

	ok = TRUE;
}
//*********************************************************
void GLFontBase::FreeResources ()
{
	//Free character memory
	glDeleteTextures(1, &Font.Tex);
	if (Font.Char != NULL) delete []Font.Char;
	ok = FALSE;
}
//*********************************************************
void GLFontBase::Begin ()
{
	if (!ok)
	{
		throw GLFontError::InvalidFont();
	}
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Font.Tex);
}

void GLFontBase::End ()
{
	//glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
//*********************************************************
GLFontBase::~GLFontBase ()
{
	FreeResources();
}

//*********************************************************
//GLFont
//*********************************************************

GLFont::GLFont()
{
}
//*********************************************************
void GLFont::Create(const char* Filename)
{
	GLFontBase::CreateImpl(Filename, FALSE);
}
//*********************************************************
void GLFont::RenderText (const char* String, float size, Element* element)
{
	//Return if we don't have a valid glFont
	if (!ok)
	{
		throw GLFontError::InvalidFont();
	}

	float floatWidth, floatHeight;
	float tempArray[2];
	float tempSecondArray[2];
	window_t* window = element->elementInfo;
	window_t* parentInfo = window->parent->elementInfo;

	//Work out the bounds of the textbox
	switch (element->windowType) {
	case 0:

		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			element->rendering->PixelToFloat1D(window->x, element->screenWidth)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			element->rendering->PixelToFloat1D(window->y, element->screenHeight)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = element->originPosition[0] +
			element->rendering->PixelToFloat1D(window->x + window->width,
				element->screenWidth)
			* element->xModifier;
		tempSecondArray[1] = element->originPosition[1] +
			element->rendering->PixelToFloat1D(window->y + window->height,
				element->screenHeight)
			* element->yModifier;
		break;

	case 1:
		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			element->rendering->GetRelativeFloat1D(window->x, parentInfo->width)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			element->rendering->GetRelativeFloat1D(window->y, parentInfo->height)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = element->originPosition[0] +
			element->rendering->GetRelativeFloat1D(window->x + window->width, parentInfo->width)
			* element->xModifier;
		tempSecondArray[1] = element->originPosition[1] +
			element->rendering->GetRelativeFloat1D(window->y + window->height, parentInfo->height)
			* element->yModifier;

		break;

	case 2:

		//Convert the height into a float value
		floatHeight = element->rendering->PixelToFloat1D(window->height,
			element->screenHeight);
		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			element->rendering->GetRelativeFloat1D(window->x, parentInfo->width)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			element->rendering->GetRelativeFloat1D(window->y, parentInfo->height)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = element->originPosition[0] +
			element->rendering->GetRelativeFloat1D(window->x + window->width, parentInfo->width)
			* element->xModifier;
		tempSecondArray[1] = tempArray[1] + floatHeight
			* element->yModifier;
		break;

	case 3:

		//Convert the width into a float value
		floatWidth = element->rendering->PixelToFloat1D(window->width,
			element->screenWidth)
			* element->xModifier;

		//The first corner of the window
		tempArray[0] = element->originPosition[0] +
			element->rendering->GetRelativeFloat1D(window->x, parentInfo->width)
			* element->xModifier;
		tempArray[1] = element->originPosition[1] +
			element->rendering->GetRelativeFloat1D(window->y, parentInfo->height)
			* element->yModifier;

		//The second corner of the window
		tempSecondArray[0] = tempArray[0] + floatWidth
			* element->xModifier;
		tempSecondArray[1] = element->originPosition[1] +
			element->rendering->GetRelativeFloat1D(window->y + window->height, parentInfo->height)
			* element->yModifier;
		break;
	}

	float topLeft[2];

	//Work out the co-ordinates of the bottom left and top right of the
	//textbox.

	if (tempArray[0] < tempSecondArray[0]) {
		topLeft[0] = tempArray[0];
	}
	else {
		topLeft[0] = tempSecondArray[0];
	}

	if (tempArray[1] < tempSecondArray[1]) {
		topLeft[1] = tempSecondArray[1];
	}
	else {
		topLeft[1] = tempArray[1];
	}

	float x = topLeft[0];
	float y = topLeft[1];

	int verticalLines;
	
	//Calculate how many vetical lines of text will fit in the box
	float charHeight = (this->Font.TexHeight * size);

	verticalLines = ceil(window->height / charHeight);

	//Get length of string
	int Length = strlen(String);

	//Calculate if we'll need to "squash" the text or not
	float squashMod;
	float charWidth = (this->Font.TexWidth * size);
	if (((charWidth * size) * Length) / verticalLines < window->width)
		squashMod = 1;
	else
		squashMod = window->width / (((charWidth * size) * Length) / verticalLines);

	//Get characters per line
	int charPerLine = floor(window->width / (charWidth * squashMod));

	//Begin rendering quads
	glBegin(GL_TRIANGLE_STRIP);

	glColor4ub(
		element->colour[0],
		element->colour[1],
		element->colour[2],
		element->colour[3]);

	int spaceLeft = charPerLine;

	//Loop through characters
	for (int i = 0; i < Length; i++)
	{
		//Get pointer to glFont character
		GLFONTCHAR *Char = &Font.Char[(int)String[i] - Font.IntStart];
		
		float dx = Char->dx*size * squashMod;
		float dy = Char->dy*size;



		//Specify vertices and texture coordinates
		glTexCoord2f(Char->tx1, Char->ty2);
		glVertex2f(x, y - dy);
		glTexCoord2f(Char->tx1, Char->ty1);
		glVertex2f(x, y);
		glTexCoord2f(Char->tx2, Char->ty2);
		glVertex2f(x + dx, y - dy);
		glTexCoord2f(Char->tx2, Char->ty1);
		glVertex2f(x + dx, y);
		/*glTexCoord2f(Char->tx1, Char->ty2);
		glVertex2f(x, y);
		glTexCoord2f(Char->tx2, Char->ty2);
		glVertex2f(x + dx, y);
		glTexCoord2f(Char->tx2, Char->ty1);
		glVertex2f(x + dx, y + dy);
		glTexCoord2f(Char->tx1, Char->ty1);
		glVertex2f(x, y + dy);*/

		//Move to next character
		spaceLeft--;
		if (spaceLeft == 0) {
			spaceLeft = charPerLine;
			x = topLeft[0] - dx;
			y += dy;
		}
		x += dx;
	}

	//Stop rendering quads
	glEnd();
}

//End of file



