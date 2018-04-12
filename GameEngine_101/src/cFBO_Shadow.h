#ifndef _cFBO_Shadow_HG_
#define _cFBO_Shadow_HG_


#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class cFBO_Shadow
{
public:
	cFBO_Shadow() :
		ID(0),
		depthTexture_ID(0),
		width(-1), height(-1) {};

	GLuint ID;
	GLuint depthTexture_ID;
	GLint width;
	GLint height;

	// Takes width and height and return ratio
	float CalculateAspectRatio(void);

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);

	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);

    void clearBuffer();

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
};

#endif