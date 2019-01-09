#ifndef texture_h___
#define texture_h___


#include <aribeiro/aribeiro.h>
using namespace aRibeiro;


class ShaderTwoTextures : public GLShader {

protected:
	//
	// uniform
	//
	int uSampler2DTextureA;
	int uSampler2DTextureB;

	int uMat4ModelViewProjection;

	int uFloatBlend;

public:
	//
	// vertex attrib
	//
	int aVec3Position;
	int aVec2UV;

	ShaderTwoTextures() :GLShader() {

		const char vertexShaderCode[] = {
			//attributes
			"attribute vec3 aVec3Position;"
			"attribute vec2 aVec2UV;"
			//uniforms
			"uniform mat4 uMat4ModelViewProjection;"
			//varying
			"varying vec2 vVec2UV;"
			//vertex program
			"void main() {"
				//pega as coordenadas de textura vindas da aplicacao
				"vVec2UV = aVec2UV;"
				//calcula a coordenada homogenea da projecao configurada
				"gl_Position = uMat4ModelViewProjection * vec4(aVec3Position, 1.0);"
			"}"
		};

		const char fragmentShaderCode[] = {
			//"precision mediump float;"
			//uniforms
			"uniform sampler2D uSampler2DTextureA;"
			"uniform sampler2D uSampler2DTextureB;"
			"uniform float uFloatBlend;"

			//varying
			"varying vec2 vVec2UV;"

			"void main() {"
				"vec4 textelA = texture2D(uSampler2DTextureA, vVec2UV);"
				"vec4 textelB = texture2D(uSampler2DTextureB, vVec2UV);"
				"gl_FragColor = lerp(textelA, textelB, uFloatBlend);"
			"}"
		};

		LoadShaderProgram(vertexShaderCode, fragmentShaderCode);

		aVec3Position = getAttribLocation("aVec3Position");
		aVec2UV = getAttribLocation("aVec2UV");

		uSampler2DTextureA = getUniformLocation("uSampler2DTextureA");
		uSampler2DTextureB = getUniformLocation("uSampler2DTextureB");

		uMat4ModelViewProjection = getUniformLocation("uMat4ModelViewProjection");

		uFloatBlend = getUniformLocation("uFloatBlend");
	}


	void setTextureA(int textureUnit) {
		setUniform(uSampler2DTextureA, textureUnit);
	}

	void setTextureB(int textureUnit) {
		setUniform(uSampler2DTextureB, textureUnit);
	}

	void setBlend(float blend) {
		setUniform(uFloatBlend, blend);
	}

	void setModelViewProjection(const mat4 &matrix) {
		setUniform(uMat4ModelViewProjection, matrix);
	}

};

class ShaderProceduralTexture : public GLShader {

protected:
	//
	// uniform
	//
	int uMat4ModelViewProjection;

	int uFloatFrequency;

public:
	//
	// vertex attrib
	//
	int aVec3Position;
	int aVec2UV;

	ShaderProceduralTexture() :GLShader() {

		const char vertexShaderCode[] = {
			//attributes
			"attribute vec3 aVec3Position;"
			"attribute vec2 aVec2UV;"
			//uniforms
			"uniform mat4 uMat4ModelViewProjection;"
			//varying
			"varying vec2 vVec2UV;"
			//vertex program
			"void main() {"
				//pega as coordenadas de textura vindas da aplicacao
				"vVec2UV = aVec2UV;"
				//calcula a coordenada homogenea da projecao configurada
				"gl_Position = uMat4ModelViewProjection * vec4(aVec3Position, 1.0);"
			"}"
		};

		const char fragmentShaderCode[] = {
			//"precision mediump float;"
			//uniforms
			"uniform float uFloatFrequency;"

			//varying
			"varying vec2 vVec2UV;"

			"vec3 proceduralChecker(vec2 coord) {"
				"float freq = uFloatFrequency;"
				"vec3 retorno = 1;"
				"if (freq > 0)"
					"retorno = lerp( vec3(0.5,0.5,0.5), vec3(1.0,1.0,1.0), saturate(sign(fmod(coord.x,freq) - freq * 0.5)) );"
				"return retorno;"
			"}"

			"void main() {"
				"vec3 color = proceduralChecker( vVec2UV );"
				"gl_FragColor = vec4( color, 1.0 );"
			"}"
		};

		LoadShaderProgram(vertexShaderCode, fragmentShaderCode);

		aVec3Position = getAttribLocation("aVec3Position");
		aVec2UV = getAttribLocation("aVec2UV");

		uMat4ModelViewProjection = getUniformLocation("uMat4ModelViewProjection");

		uFloatFrequency = getUniformLocation("uFloatFrequency");
	}

	void setFrequency(float v) {
		setUniform(uFloatFrequency, v);
	}

	void setModelViewProjection(const mat4 &matrix) {
		setUniform(uMat4ModelViewProjection, matrix);
	}
};

#endif
