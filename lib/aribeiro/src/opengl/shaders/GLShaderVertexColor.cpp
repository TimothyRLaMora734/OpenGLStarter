#include "GLShaderVertexColor.h"

namespace aRibeiro {

	GLShaderVertexColor::GLShaderVertexColor() :GLShader() {
		const char vertexShaderCode[] = {
			"attribute vec3 vPosition;"
			"attribute vec4 vColor;"
			"uniform mat4 matrix;"
			"varying vec4 color;"
			"void main() {"
			"  color = vColor;"
			"  gl_Position = matrix * vec4(vPosition, 1.0);"
			//"  gl_Position = gl_ModelViewProjectionMatrix * vPosition;"
			"}" };

		const char fragmentShaderCode[] = {
			//"precision mediump float;"
			"varying vec4 color;"
			"void main() {"
			"  vec4 result = color;"
			"  gl_FragColor = result;"
			"}" };

		LoadShaderProgram(vertexShaderCode, fragmentShaderCode);

		vPosition = getAttribLocation("vPosition");
		vColor = getAttribLocation("vColor");

		matrix = getUniformLocation("matrix");
	}


	void GLShaderVertexColor::setMatrix(const mat4 & m) {
		setUniform(matrix, m);
	}

}

