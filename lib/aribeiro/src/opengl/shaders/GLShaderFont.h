#ifndef GL_SHADER_FONT_h_
#define GL_SHADER_FONT_h_

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderFont : public GLShader {

	public:
		//
		// attrib locations
		//
		int vPosition;
		int vColor;
		int vUV;

		//
		// uniform locations
		//
		int texture;
		int matrix;

		GLShaderFont();

		void setTexture(int activeTextureUnit);
		void setMatrix(const mat4 & m);
	};

}

#endif