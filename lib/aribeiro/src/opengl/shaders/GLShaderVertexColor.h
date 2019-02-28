#ifndef GL_SHADER_VERTEX_COLOR_h_
#define GL_SHADER_VERTEX_COLOR_h_

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderVertexColor : public GLShader {

    protected:
        
        void setupAttribLocation();
        
	public:
		//
		// attrib locations
		//
		static const int vPosition = 0;
		static const int vColor = 1;

		//
		// uniform locations
		//
		int matrix;

		GLShaderVertexColor();

		void setMatrix(const mat4 & m);
	};

}

#endif
