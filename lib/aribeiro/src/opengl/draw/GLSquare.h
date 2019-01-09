#ifndef GL_SQUARE_COLOR_h
#define GL_SQUARE_COLOR_h

#include <aribeiro/GLShaderColor.h>
#include <aribeiro/GLShaderTextureColor.h>

namespace aRibeiro {

	class GLSquare {

	public:

		GLSquare();

		void draw(GLShaderColor *shader);
		void draw(GLShaderTextureColor *shader);

	};

}

#endif
