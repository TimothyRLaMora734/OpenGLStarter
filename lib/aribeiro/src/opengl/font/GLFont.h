#ifndef GLFont___H
#define GLFont___H

#include <glew/glew.h>
#include <map>
#include <vector>
#include <convertutf/ConvertUTF.h>

#include <aribeiro/all_math.h>
//#include <aribeiro/GLStripText.h>
#include <aribeiro/Font.h>
#include <aribeiro/GLTexture.h>
#include <aribeiro/GLShaderFont.h>

#include <wchar.h> // to define swprintf(...);

namespace aRibeiro {

	class GLStripText;

	struct GLFontGlyphInfo {
		//base calculations
		vec2 texBegin, texEnd;
		vec2 dimension;
		vec2 beforeStart, advance;
		//used for render
		vec2 TexcoordVertex2fStrip[6 * 2];
	};

	class GLFont {
		FontHeader mHeader;
		std::map<UTF32, GLFontGlyphInfo> mGlyphs;
		//GLuint mTextureID;

		GLTexture texture;
		static GLShaderFont *globalShader;

	private:

		//private copy constructores, to avoid copy...
		GLFont(const GLFont& v);
		void operator=(const GLFont&v);

	public:

		static void releaseSharedResources();
		
		GLFont(const FontHeader& aHeader,
			std::map<UTF32, FontGlyphInfo>& aGlyphs,
			const char* luminancePointer);
		virtual ~GLFont();

		GLTexture *getTexture();
		GLShaderFont *getShader();

		void print(const vec4& color, const vec2& pos, const wchar_t* string);

		void print(const vec4& color, const vec2& pos, const char* string);

		//void print(const wchar_t* v,...);
		void print(const vec4& color, const wchar_t* string);
		void print(const vec3& color, const wchar_t* string);

		collision::AABB computeBounds(const wchar_t* string);
		collision::AABB computeBounds(const char* string);

		collision::AABB computeBoundsJustBox(const wchar_t* string);
		collision::AABB computeBoundsJustBox(const char* string);

		GLStripText* createGLStripText(const wchar_t* string);

		float getLineHeight();
	private:
		struct VertexAttrib {
			vec2 uv;
			vec4 color;
			vec2 pos;
			VertexAttrib() {}
			VertexAttrib(const vec2& uv,
				const vec4& color,
				const vec2& pos) {
				this->uv = uv;
				this->color = color;
				this->pos = pos;
			}
		};
		std::vector<VertexAttrib, ssealign<VertexAttrib, 16> > mModelBuffer;
		vec2 putc(wchar_t c, const vec2& p, bool repeatFirstPtn, bool repeatLastPtn, const vec4& color, float basex);
	};

}

#endif
