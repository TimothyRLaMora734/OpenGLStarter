#ifndef GLStripText___H
#define GLStripText___H

#include <map>
#include <vector>
#include <glew/glew.h>

#include <aribeiro/all_math.h>

#include <aribeiro/all_math.h>
#include <aribeiro/Font.h>
#include <aribeiro/GLTexture.h>
#include <aribeiro/GLShaderFont.h>


namespace aRibeiro {

	struct StripInfoVertexAttrib {
		vec4 mColor;
		vec2 mCoord;
		vec2 mPos;
	};

	class GLStripText {
		StripInfoVertexAttrib *mStripInfo;
		int mSize;
		
		GLTexture *texture;
		GLShaderFont *shader;

	private:

		// avoid copy
		GLStripText(const GLStripText& v);
		void operator=(const GLStripText&v);

	public:

		void setSize(int s);

		GLStripText(int buffSize, GLTexture *texture, GLShaderFont *shader);
		virtual ~GLStripText();
		StripInfoVertexAttrib& getVertex(int index);
		StripInfoVertexAttrib& getCharVertex(int CharIndex);

		void draw();

		void resetColor(vec4 c);

		int getCharacterCount();

		void saturateFromBegin(float increment);
	};

}

#endif