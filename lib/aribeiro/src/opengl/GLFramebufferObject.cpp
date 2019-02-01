#include "GLFramebufferObject.h"
#include <aribeiro/PlatformGL.h>

#include <stdio.h>
#include <stdlib.h>

namespace aRibeiro {

	//uses the following extensions:
	//   GLEW_ARB_framebuffer_object,
	//   GLEW_ARB_draw_buffers,
	//   GLEW_ARB_framebuffer_no_attachments,
	//   GLEW_ARB_depth_texture
	//   GLEW_ARB_texture_non_power_of_two

	GLuint DrawBuffersUnit[16] = {
		GL_COLOR_ATTACHMENT0_EXT,
		GL_COLOR_ATTACHMENT1_EXT,
		GL_COLOR_ATTACHMENT2_EXT,
		GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT,
		GL_COLOR_ATTACHMENT5_EXT,
		GL_COLOR_ATTACHMENT6_EXT,
		GL_COLOR_ATTACHMENT7_EXT,
		GL_COLOR_ATTACHMENT8_EXT,
		GL_COLOR_ATTACHMENT9_EXT,
		GL_COLOR_ATTACHMENT10_EXT,
		GL_COLOR_ATTACHMENT11_EXT,
		GL_COLOR_ATTACHMENT12_EXT,
		GL_COLOR_ATTACHMENT13_EXT,
		GL_COLOR_ATTACHMENT14_EXT,
		GL_COLOR_ATTACHMENT15_EXT
	};

    GLFramebufferObject::GLFramebufferObject(const GLFramebufferObject& v) {}
    void GLFramebufferObject::operator=(const GLFramebufferObject& v) {}

	GLFramebufferObject::GLFramebufferObject() {
		initialized = false;
		depth = NULL;
		stencil = NULL;
		mFbo = 0;
		width = 0;
		height = 0;

		OPENGL_CMD(glGenFramebuffersEXT(1, &mFbo));
	}

	GLFramebufferObject::~GLFramebufferObject() {
		if (glIsFramebufferEXT(mFbo))
			glDeleteFramebuffersEXT(1, &mFbo);
		mFbo = 0;
	}

	//
	// attach the textures set and depth
	//
	void GLFramebufferObject::attachTextures() {
		if (initialized)
			return;

		initialized = true;

		if ((int)color.size() > maxDrawBuffers()) {
			fprintf(stderr,
				"[GLFramebufferObject] Trying to use more draw buffers than the current hardware support. \n"
				"                      (color buffer size: %lu, max draw buffers: %i)\n", color.size(), maxDrawBuffers());
			exit(-1);
		}

		if (width == 0 || height == 0) {
			fprintf(stderr, "[GLFramebufferObject] You need to call setSize before attachTextures. \n" );
			exit(-1);
		}

		if (depth == NULL && color.size() == 0) {
			fprintf(stderr, "[GLFramebufferObject] Cannot create a framebuffer object without any texture. \n");
			exit(-1);
		}

		//force create 32x32 texture, if texture is not initialized
		//setSize(32,32);

		OPENGL_CMD(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFbo));

		for (int i = 0; i < (int)color.size(); i++)
			OPENGL_CMD(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, DrawBuffersUnit[i], GL_TEXTURE_2D, color[i]->mTexture, 0));

		if (depth != NULL) {
			OPENGL_CMD(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depth->mTexture, 0));
		}

		if (stencil != NULL) {
			OPENGL_CMD(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencil->mTexture, 0));
		}

		//check the fbo attachment status...
		GLenum status = (GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch (status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			//printf("Framebuffer OK!!\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			fprintf(stderr, "Unsupported framebuffer format\n");
			exit(-1);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			fprintf(stderr, "Framebuffer incomplete, missing attachment\n");
			exit(-1);
			break;
		//case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
		case 0x8CD8:
			fprintf(stderr, "Framebuffer incomplete, duplicate attachment\n");
			exit(-1);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			fprintf(stderr, "Framebuffer incomplete, attached images must have same dimensions\n");
			exit(-1);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			fprintf(stderr, "Framebuffer incomplete, attached images must have same format\n");
			exit(-1);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			fprintf(stderr, "Framebuffer incomplete, missing draw buffer\n");
			exit(-1);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			fprintf(stderr, "Framebuffer incomplete, missing read buffer\n");
			exit(-1);
			break;
		default:
			fprintf(stderr, "Opengl Call ERROR!!!");
			exit(-1);
			break;
		}
		
		//set the draw buffers to this FBO
		OPENGL_CMD(glDrawBuffers(color.size(), DrawBuffersUnit));

		OPENGL_CMD(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));
	}

	// GL_RGBA, etc...
	// GL_RGBA32F_ARB, GL_RGB32F_ARB, GL_RGBA16F_ARB, GL_RGB16F_ARB
	//
	void GLFramebufferObject::setSize(int w, int h, GLuint colorFormat, GLuint depthFormat, GLuint stencilFormat) {
        if (this->width == w && this->height == h)
            return;
		this->width = w;
		this->height = h;
		if (depth != NULL)
			depth->setSize(w, h, depthFormat);
		if (stencil != NULL)
			stencil->setSize(w, h, stencilFormat);
		for (int i = 0; i < (int)color.size(); i++)
			color[i]->setSize(w, h, colorFormat);
	}

	void GLFramebufferObject::enable() {
		if (!initialized)
			return;
		OPENGL_CMD(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFbo));
		//OPENGL_CMD(glDrawBuffers(color.size(), DrawBuffersUnit));
	}

	void GLFramebufferObject::disable() {
		// error to setup glDrawBuffers without any fbo in the current state set
		//OPENGL_CMD(glDrawBuffers(1, DrawBuffersUnit));
		//
		OPENGL_CMD(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));
	}

	int GLFramebufferObject::currentDepthBits() {
		int depthBits;
		glGetIntegerv(GL_DEPTH_BITS, &depthBits);
		return depthBits;
	}
	int GLFramebufferObject::maxColorAttachments() {
		int v;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &v);
		return v;
	}
	int GLFramebufferObject::maxDrawBuffers() {
		int v;
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &v);
		return v;
	}

}
