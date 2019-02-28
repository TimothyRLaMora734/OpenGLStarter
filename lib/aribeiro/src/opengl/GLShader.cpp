
#include "GLShader.h"

#include <aribeiro/PlatformGL.h>

namespace aRibeiro {

	//
	// Static
	//
	GLint GLShader::loadShader(int type, const char* shaderCode) {
		GLint shader = glCreateShader(type);
		const char* shaderArray[] = { shaderCode };
		OPENGL_CMD(glShaderSource(shader, 1, shaderArray, NULL));
		OPENGL_CMD(glCompileShader(shader));
		return shader;
	}

	GLint GLShader::loadShaderStrings(GLShader *shaderObj, const char* vertexShaderCode, const  char* fragmentShaderCode) {
		GLint vertexShader;
		OPENGL_CMD(vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderCode));
		PlatformGL::checkShaderStatus(vertexShader);
		
		GLint fragmentShader;
		OPENGL_CMD(fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCode));
		PlatformGL::checkShaderStatus(fragmentShader);

		int mProgram;
		OPENGL_CMD(mProgram = glCreateProgram());
		
		//
		// Attach shaders
		//
		OPENGL_CMD(glAttachShader(mProgram, vertexShader));
		OPENGL_CMD(glAttachShader(mProgram, fragmentShader));

        shaderObj->mProgram = mProgram;
        shaderObj->setupAttribLocation();
        
		//
		// Perform shader link
		//
		OPENGL_CMD(glLinkProgram(mProgram));

		PlatformGL::checkProgramStatus(mProgram);

		//
		// Shader information is not needed any more... can be detached and deleted
		//
		OPENGL_CMD(glDetachShader(mProgram, vertexShader));
		OPENGL_CMD(glDetachShader(mProgram, fragmentShader));

		OPENGL_CMD(glDeleteShader(vertexShader));
		OPENGL_CMD(glDeleteShader(fragmentShader));

		return mProgram;
	}

	void GLShader::disable() {
		OPENGL_CMD(glUseProgram(0));
	}

	GLint GLShader::getCurrentShader() {
		GLint result;
		OPENGL_CMD(glGetIntegerv(GL_CURRENT_PROGRAM, &result));
		return result;
	}

	//
	// Non-Static
	//
	void GLShader::LoadShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode) {
		mProgram = loadShaderStrings(this, vertexShaderCode, fragmentShaderCode);
	}
	GLint GLShader::getAttribLocation(const char* name) {
		GLint result;
		OPENGL_CMD(result = glGetAttribLocation(mProgram, name));
		return result;
	}
	GLint GLShader::getUniformLocation(const char* name) {
		GLint result;
		OPENGL_CMD(result = glGetUniformLocation(mProgram, name));
		return result;
	}
	
	void GLShader::setUniform(int location, int v) {
		OPENGL_CMD(glUniform1i(location, v));
	}

	void GLShader::setUniform(int location, float v) {
		OPENGL_CMD(glUniform1f(location, v));
	}
	void GLShader::setUniform(int location, const vec2 &v) {
		OPENGL_CMD(glUniform2fv(location, 1, v.array));
	}
	void GLShader::setUniform(int location, const vec3 &v) {
		OPENGL_CMD(glUniform3fv(location, 1, v.array));
	}
	void GLShader::setUniform(int location, const vec4 &v) {
		OPENGL_CMD(glUniform4fv(location, 1, v.array));
	}
	void GLShader::setUniform(int location, const mat4 &m) {
		OPENGL_CMD(glUniformMatrix4fv(location, 1, GL_FALSE , m.array));
	}

	int GLShader::getUniformInt(int location) {
		int v;
		OPENGL_CMD(glGetUniformiv(mProgram, location, &v));
		return v;
	}

	float GLShader::getUniformFloat(int location) {
		float v;
		OPENGL_CMD(glGetUniformfv(mProgram, location, &v));
		return v;
	}

	vec2 GLShader::getUniformVec2(int location) {
		vec2 v;
		OPENGL_CMD(glGetUniformfv(mProgram, location, v.array));
		return v;
	}

	vec3 GLShader::getUniformVec3(int location) {
		vec3 v;
		OPENGL_CMD(glGetUniformfv(mProgram, location, v.array));
		return v;
	}

	vec4 GLShader::getUniformVec4(int location) {
		vec4 v;
		OPENGL_CMD(glGetUniformfv(mProgram, location, v.array));
		return v;
	}

	mat4 GLShader::getUniformMat4(int location) {
		mat4 v;
		OPENGL_CMD(glGetUniformfv(mProgram, location, v.array));
		return v;
	}
    
    void GLShader::bindAttribLocation(int location, const char* attribName) {
        OPENGL_CMD(glBindAttribLocation(mProgram, location, attribName));
    }

	void GLShader::enable() {
		if (GLShader::getCurrentShader() != mProgram)
			OPENGL_CMD(glUseProgram(mProgram));
	}

	GLShader::GLShader() {
		mProgram = -1;
	}

	GLShader::~GLShader() {
		if (mProgram != -1) {
			if (GLShader::getCurrentShader() == mProgram)
				GLShader::disable();
			glDeleteProgram(mProgram);
			mProgram = -1;
		}
	}
}
