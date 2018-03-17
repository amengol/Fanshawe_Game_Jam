#ifndef _FBO_HG_
#define _FBO_HG_

#include <string>
#include "globalOpenGL_GLFW.h"

class cFBO
{
public:
	cFBO() : 
		ID(0), 
		colourTexture_0_ID(0),
		normalTexture_1_ID(1),
		depthTexture_ID(0), 
		width(-1), height(-1) {};

	GLuint ID;				// g_FBO	// = 0;
	GLuint colourTexture_0_ID;		// = 0;
	GLuint normalTexture_1_ID;
	GLuint vertexWorldPos_2_ID;
//	GLuint TBDTexture_2_ID;
//	GLuint TBDTexture_3_ID;

	GLuint depthTexture_ID;		// = 0;
	GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
	GLint height;

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);
	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);
	
	void clearBuffers(bool bClearColour = true, bool bClearDepth = true);

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
};

#endif