#ifndef GL_SHADER_TEXTURE_COLOR_h_
#define GL_SHADER_TEXTURE_COLOR_h_

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderTextureColor : public GLShader {

	public:
		//
		// attrib locations
		//
		int vPosition;
		int vUV;

		//
		// uniform locations
		//
		int color;
		int matrix;
		int texture;

		GLShaderTextureColor();

		void setColor(vec4 c);
		void setMatrix(const mat4 & m);
		void setTexture(int activeTextureUnit);

	};

}

#endif