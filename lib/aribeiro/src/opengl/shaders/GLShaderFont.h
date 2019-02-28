#ifndef GL_SHADER_FONT_h_
#define GL_SHADER_FONT_h_

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderFont : public GLShader {

    protected:
        
        void setupAttribLocation();
        
	public:
		//
		// attrib locations
		//
		static const int vPosition = 0;
		static const int vColor = 1;
		static const int vUV = 2;

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
