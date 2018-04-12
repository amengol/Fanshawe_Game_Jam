#include "cFBO_Shadow.h"

// Calls shutdown(), then init()
bool cFBO_Shadow::reset(int width, int height, std::string &error)
{
	if (!this->shutdown())
	{
		error = "Could not shutdown";
		return false;
	}

	return this->init(width, height, error);
}

bool cFBO_Shadow::shutdown(void)
{
	glDeleteTextures(1, &( this->depthTexture_ID ));

	glDeleteFramebuffers(1, &( this->ID ));

	return true;
}


bool cFBO_Shadow::init(int width, int height, std::string &error)
{
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &( this->ID ));
	glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

	glGenTextures(1, &( this->depthTexture_ID ));
	glBindTexture(GL_TEXTURE_2D, this->depthTexture_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT,
				   this->width,		
				   this->height);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture_ID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bool bFrameBufferIsGoodToGo = true;

	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
	case GL_FRAMEBUFFER_COMPLETE:
		bFrameBufferIsGoodToGo = true;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		bFrameBufferIsGoodToGo = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
	case GL_FRAMEBUFFER_UNSUPPORTED:
	default:
		bFrameBufferIsGoodToGo = false;
		break;
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bFrameBufferIsGoodToGo;
}

void cFBO_Shadow::clearBuffer(void)
{
	glViewport(0, 0, this->width, this->height);

    glClear(GL_DEPTH_BUFFER_BIT);

	return;
}

int cFBO_Shadow::getMaxColourAttachments(void)
{
	//  void glGetIntegerv(GLenum pname,
	// 				       GLint * data);

	int maxColourAttach = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColourAttach);

	return maxColourAttach;
}

int cFBO_Shadow::getMaxDrawBuffers(void)
{
	int maxDrawBuffers = 0;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

	return maxDrawBuffers;
}

// Takes width and height and return ratio
float cFBO_Shadow::CalculateAspectRatio(void)
{
	return this->width / (float) this->height;
}
