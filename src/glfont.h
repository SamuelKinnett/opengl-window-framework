#ifndef _glfonth_
#define _glfonth_

//*********************************************************
//GLFONT.H -- Header for GLFONT.CPP
//Copyright (c) 1998 Brad Fish
//Copyright (c) 2002 Henri Kyrki
//See glFont.txt for terms of use
//10.5 2002
//*********************************************************

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

class Element;

namespace GLFontError {
	struct InvalidFile{};
	struct InvalidFont{};
}

class GLFontBase {
public:
	GLFontBase();
	void Begin();
	virtual ~GLFontBase();

	virtual void Create(const char* Filename) {;}

	//Removed z arguments, unnecessary in this situation
	virtual void RenderText(const char* String, float x, float y, float size, Element* element) {;}
	virtual void RenderText(const char* String, int x, int y) {;}
	void End();

protected:

	void CreateImpl(const char *FileName, bool PixelPerfect = FALSE);

	typedef struct
	{
	union {
		float dx;
		int width;
	};
	union {
		float dy;
		int height;
	};
	float tx1, ty1;
	float tx2, ty2;
	} GLFONTCHAR;

	typedef struct
	{
	unsigned int Tex;
	int TexWidth, TexHeight;
	int IntStart, IntEnd;
	GLFONTCHAR *Char;
	} GLFONT;
	
	GLFONT Font;
	bool ok;
private:
	void FreeResources();
};

class GLFont : public GLFontBase {
public:
	GLFont();
	virtual ~GLFont(){;}
	virtual void Create(const char* FileName);
	//Removed z argument, unnecessary in this situation
	virtual void RenderText (const char* String, float size, Element* element);
};
#endif
//End of file


