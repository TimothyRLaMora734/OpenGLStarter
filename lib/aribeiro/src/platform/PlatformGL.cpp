#include "PlatformGL.h"

#include <glew/glew.h>
#include <stdio.h>
#include <stdlib.h>

namespace aRibeiro {

	void PlatformGL::glCheckError(const char* file, unsigned int line, const char* expression)
	{
		// Get the last error
		GLenum errorCode = glGetError();

		if (errorCode != GL_NO_ERROR)
		{
			const char* fileString = file;
			const char* error = "Unknown error";
			const char* description = "No description";

			// Decode the error code
			switch (errorCode)
			{
			case GL_INVALID_ENUM:
			{
				error = "GL_INVALID_ENUM";
				description = "An unacceptable value has been specified for an enumerated argument.";
				break;
			}

			case GL_INVALID_VALUE:
			{
				error = "GL_INVALID_VALUE";
				description = "A numeric argument is out of range.";
				break;
			}

			case GL_INVALID_OPERATION:
			{
				error = "GL_INVALID_OPERATION";
				description = "The specified operation is not allowed in the current state.";
				break;
			}
#ifndef ARIBEIRO_RPI
			case GL_STACK_OVERFLOW:
			{
				error = "GL_STACK_OVERFLOW";
				description = "This command would cause a stack overflow.";
				break;
			}

			case GL_STACK_UNDERFLOW:
			{
				error = "GL_STACK_UNDERFLOW";
				description = "This command would cause a stack underflow.";
				break;
			}
#endif
			case GL_OUT_OF_MEMORY:
			{
				error = "GL_OUT_OF_MEMORY";
				description = "There is not enough memory left to execute the command.";
				break;
			}

			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				error = "GL_INVALID_FRAMEBUFFER_OPERATION";
				description = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
				break;
			}
			}

			// Log the error
			printf("An internal OpenGL call failed in %s(%i). \n"
				"Expression:%s\n"
				"Error description:%s\n"
				"%s\n\n", fileString, line, expression, error, description);

			exit(-1);
		}
	}

	void PlatformGL::checkShaderStatus(int shader) {
		GLint success;
		#ifdef ARIBEIRO_RPI
		OPENGL_CMD(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
		#else
		OPENGL_CMD(glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
		#endif
		if (success == GL_FALSE)
		{
			char log[1024];
			#ifdef ARIBEIRO_RPI
			OPENGL_CMD(glGetShaderInfoLog(shader, sizeof(log), 0, log));
			#else
			OPENGL_CMD(glGetInfoLogARB(shader, sizeof(log), 0, log));
			#endif
			printf("Failed to compile shader: %s\n", log);
			exit(-1);
		}
	}

	void PlatformGL::checkProgramStatus(int program) {
		GLint success;
		#ifdef ARIBEIRO_RPI
		OPENGL_CMD(glGetProgramiv(program, GL_LINK_STATUS, &success));
		#else
		OPENGL_CMD(glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &success));
		#endif
		if (success == GL_FALSE)
		{
			char log[1024];
			#ifdef ARIBEIRO_RPI
			OPENGL_CMD(glGetProgramInfoLog(program, sizeof(log), 0, log));
			#else
			OPENGL_CMD(glGetInfoLogARB(program, sizeof(log), 0, log));
			#endif
			printf("Failed to link shader: %s\n", log);
			exit(-1);
		}
	}

}
