#include "GLShaderTextureColor.h"

namespace aRibeiro {

	GLShaderTextureColor::GLShaderTextureColor() :GLShader() {
		const char vertexShaderCode[] = {
			"attribute vec3 vPosition;"
			"attribute vec2 vUV;"
			"uniform mat4 matrix;"
			"varying vec2 uv;"
			"void main() {"
			"  uv = vUV;"
			"  gl_Position = matrix * vec4(vPosition,1.0);"
			//"  gl_Position = gl_ModelViewProjectionMatrix * vPosition;"
			"}" };

		const char fragmentShaderCode[] = {
			//"precision mediump float;"
			"uniform vec4 color;"
			"varying vec2 uv;"
			"uniform sampler2D texture;"
			"void main() {"
			"  vec4 texel = texture2D(texture, uv);"
			"  vec4 result = texel * color;"
			"  gl_FragColor = result;"
			"}" };

		LoadShaderProgram(vertexShaderCode, fragmentShaderCode);

		vPosition = getAttribLocation("vPosition");
		vUV = getAttribLocation("vUV");
		
		color = getUniformLocation("color");
		matrix = getUniformLocation("matrix");
		texture = getUniformLocation("texture");
	}


	void GLShaderTextureColor::setColor(vec4 c) {
		setUniform(color, c);
	}

	void GLShaderTextureColor::setMatrix(const mat4 & m) {
		setUniform(matrix, m);
	}

	void GLShaderTextureColor::setTexture(int activeTextureUnit) {
		setUniform(texture, activeTextureUnit);
	}

}

