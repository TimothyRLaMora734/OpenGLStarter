#ifndef GL_SHADER_VERTEX_COLOR_h_
#define GL_SHADER_VERTEX_COLOR_h_

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderVertexColor : public GLShader {

	public:
		//
		// attrib locations
		//
		int vPosition;
		int vColor;

		//
		// uniform locations
		//
		int matrix;

		GLShaderVertexColor();

		void setMatrix(const mat4 & m);
	};

}

#endif