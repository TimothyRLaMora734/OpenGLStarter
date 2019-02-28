#include "GLShaderColor.h"

namespace aRibeiro {

	GLShaderColor::GLShaderColor() :GLShader() {
		const char vertexShaderCode[] = {
			"attribute vec3 vPosition;"
			"uniform mat4 matrix;"
			"void main() {"
			"  gl_Position = matrix * vec4(vPosition, 1.0);"
			//"  gl_Position = gl_ModelViewProjectionMatrix * vPosition;"
			"}" };

		const char fragmentShaderCode[] = {
			//"precision mediump float;"
			"uniform vec4 color;"
			"void main() {"
			"  vec4 result = color;"
			"  gl_FragColor = result;"
			"}" };

		LoadShaderProgram(vertexShaderCode, fragmentShaderCode);

		//vPosition = getAttribLocation("vPosition");
		
		color = getUniformLocation("color");
		matrix = getUniformLocation("matrix");
	}


	void GLShaderColor::setColor(vec4 c) {
		setUniform(color, c);
	}

	void GLShaderColor::setMatrix(const mat4 & m) {
		setUniform(matrix, m);
	}
    
    void GLShaderColor::setupAttribLocation() {
        bindAttribLocation(GLShaderColor::vPosition, "vPosition");
    }

}

