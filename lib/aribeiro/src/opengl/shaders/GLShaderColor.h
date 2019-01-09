#ifndef GL_SHADER_COLOR_h_
#define GL_SHADER_COLOR_h_

#include <aribeiro/GLShader.h>

namespace aRibeiro {

	class GLShaderColor : public GLShader {

	public:
		//
		// attrib locations
		//
		int vPosition;

		//
		// uniform locations
		//
		int color;
		int matrix;

		GLShaderColor();

		void setColor(vec4 c);
		void setMatrix(const mat4 & m);
	};

}

#endif