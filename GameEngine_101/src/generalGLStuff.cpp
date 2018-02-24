#include "globalOpenGL_GLFW.h"

std::string decodeGLErrorFromEnum(GLenum error, std::string &details)
{
	std::string errText = "";
	switch (error)
	{
	case GL_NO_ERROR:
		details = "No error has been recorded.The value of this symbolic constant is guaranteed to be 0.";
		return "GL_NO_ERROR";
		break;
	case GL_INVALID_ENUM:
		details = "An unacceptable value is specified for an enumerated argument.The offending command is ignored and has no other side effect than to set the error flag.";
		return "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		details = "A numeric argument is out of range.The offending command is ignored and has no other side effect than to set the error flag.";
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		details = "The specified operation is not allowed in the current state.The offending command is ignored and has no other side effect than to set the error flag.";
		return "GL_INVALID_OPERATION";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		details = "The framebuffer object is not complete.The offending command is ignored and has no other side effect than to set the error flag.";
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		details = "There is not enough memory left to execute the command.The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		return "GL_OUT_OF_MEMORY";
		break;
	case GL_STACK_UNDERFLOW:
		details = "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
		return "GL_STACK_UNDERFLOW";
		break;
	case GL_STACK_OVERFLOW:
		details = "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
		return "GL_STACK_OVERFLOW";
		break;
	}
	// Huh?
	details = "Unknown GLError enum";
	return "UNKNOWN";
}

