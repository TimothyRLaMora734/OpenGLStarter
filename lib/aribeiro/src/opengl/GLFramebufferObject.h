#ifndef GLFramebufferObject_H
#define GLFramebufferObject_H

#include <aribeiro/GLTexture.h>
#include <glew/glew.h>
#include <vector>
#include <aribeiro/SSE2.h>

namespace aRibeiro {

	//uses the following extensions:
	//   GLEW_ARB_framebuffer_object,
	//   GLEW_ARB_draw_buffers,
	//   GLEW_ARB_depth_texture
	//   GLEW_ARB_texture_non_power_of_two

	class GLFramebufferObject {

		GLuint mFbo;
		bool initialized;

		GLuint mDepthRenderbuffer;

        //private copy constructores, to avoid copy...
        GLFramebufferObject(const GLFramebufferObject& v);
        void operator=(const GLFramebufferObject& v);

	public:

		std::vector<GLTexture *> color;
		GLTexture *depth;
		bool useRenderbufferDepth;//allow use framebuffer object without depth textures
		GLTexture *stencil;

		int width, height;

		GLFramebufferObject();
		~GLFramebufferObject();

		//
		// attach the textures set and depth
		//
		void attachTextures();

		// GL_RGBA, etc...
		// GL_RGBA32F_ARB, GL_RGB32F_ARB, GL_RGBA16F_ARB, GL_RGB16F_ARB
		//
		void setSize(int w, int h, GLuint colorFormat = GL_RGBA,
			GLuint depthFormat = GL_DEPTH_COMPONENT24,
			GLuint stencilFormat = GL_STENCIL_INDEX8
			);

		void enable();

		static void disable();

		static int currentDepthBits();
		static int maxColorAttachments();
		static int maxDrawBuffers();

		SSE2_CLASS_NEW_OPERATOR

	};

}

/*bool isPowerOf2(int num){
  if (num == 0) return false;
  while((num&0x01)!=1)
    num = num >> 1;
  return ((num&(~0x01))==0) ;
}*/

#endif
