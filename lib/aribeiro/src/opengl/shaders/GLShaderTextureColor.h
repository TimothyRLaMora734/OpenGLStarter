#ifndef GL_SHADER_TEXTURE_COLOR_h_
#define GL_SHADER_TEXTURE_COLOR_h_

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderTextureColor : public GLShader {

    protected:
        
        void setupAttribLocation();
        
	public:
		//
		// attrib locations
		//
		static const int vPosition = 0;
		static const int vUV = 1;

		//
		// uniform locations
		//
		int color;
		int matrix;
		int texture;

		GLShaderTextureColor();

		void setColor(const vec4 &c);
		void setMatrix(const mat4 & m);
		void setTexture(int activeTextureUnit);

	};

}

#endif
