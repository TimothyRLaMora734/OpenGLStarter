#ifndef GL_TEXTURE_h
#define GL_TEXTURE_h

#include <glew/glew.h> // extensions here

namespace aRibeiro {

	class GLTexture {

	private:

		static int originalPackAlignment;
		static int originalUnpackAlignment;

	public:
		GLuint mTexture;
		int width;
		int height;

		GLTexture();

		bool isInitialized();

		void uploadBufferAlpha8(const void* buffer, int w, int h);

		void uploadBufferRGB_888(const void* buffer, int w, int h);

		void uploadBufferRGBA_8888(const void* buffer, int w, int h);

		//
		// Old way to copy framebuffer to texture... consider to use FBOs to acomplish it better
		//
		// can read rgb(GL_RGB) from framebuffer, or the depth component (GL_DEPTH_COMPONENT24)
		void setSize(int w, int h, GLuint format = GL_RGB);

		//copy the color buffer, or depth buffer, using the texture current size
		void copyFrameBuffer();

		void active(int id);
		void deactive(int id);

		virtual ~GLTexture();

		static GLTexture *loadFromPNG(const char* filename, bool invertY = false);
		void writeToPNG(const char* filename, bool invertY = false);

	};

}

#endif
