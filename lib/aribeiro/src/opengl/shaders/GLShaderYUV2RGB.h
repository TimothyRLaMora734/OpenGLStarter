#ifndef GL_SHADER_YUV2RGB_h
#define GL_SHADER_YUV2RGB_h

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderYUV2RGB : public GLShader {
    
    protected:
        
        void setupAttribLocation();
        
	public:
		//
		//attrib
		//
		static const int vPosition = 0;
		static const int vUV = 1;

		//
		//uniform
		//
		int scale;
		int textureY;
		int textureU;
		int textureV;
		int fade;
		
		GLShaderYUV2RGB(bool useBlur);

		void setScale(float x, float y);

		void setTextureY(int textureUnit);
		void setTextureU(int textureUnit);
		void setTextureV(int textureUnit);

		void setFade(float v);
	};

}
#endif
