#include "GLTexture.h"

#include <aribeiro/PlatformGL.h>
#include <aribeiro/PNGHelper.h>
#include <aribeiro/SetNullAndDelete.h>
#include <stdlib.h>
#include <stdio.h>

namespace aRibeiro {

	int GLTexture::originalPackAlignment = -1;
	int GLTexture::originalUnpackAlignment = -1;

	GLTexture::GLTexture() {

		//initialize static pack and unpack alignment
		if (originalPackAlignment == -1) {
			OPENGL_CMD(glGetIntegerv(GL_PACK_ALIGNMENT, &originalPackAlignment));
			OPENGL_CMD(glGetIntegerv(GL_UNPACK_ALIGNMENT, &originalUnpackAlignment));
		}

		mTexture = 0;
		width = 0;
		height = 0;

		OPENGL_CMD(glGenTextures(1, &mTexture));
		OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, mTexture));

		//active(0);
		//OPENGL_CMD(glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));

		// Set filtering
		OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		//deactive(0);

		//GLES20.glBindTexture(GL.GL_TEXTURE_2D, 0);
	}

	bool GLTexture::isInitialized() {
		return width > 0 && height > 0;
	}

	void GLTexture::uploadBufferAlpha8(const void* buffer, int w, int h) {

		//active(0);

		OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, mTexture));

		//set pack data alignment to 1 byte read from memory and 1 byte to write to gpu memory...
		OPENGL_CMD(glPixelStorei(GL_PACK_ALIGNMENT, 1));
		OPENGL_CMD(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

		if (w != width || h != height) {
			OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer));
			width = w;
			height = h;
		}
		else
			OPENGL_CMD(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_ALPHA, GL_UNSIGNED_BYTE, buffer));

		//back the pack alignment for texture operations
		OPENGL_CMD(glPixelStorei(GL_PACK_ALIGNMENT, originalPackAlignment));
		OPENGL_CMD(glPixelStorei(GL_UNPACK_ALIGNMENT, originalUnpackAlignment));

		//deactive(0);
	}

	void GLTexture::uploadBufferRGB_888(const void* buffer, int w, int h) {

		OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, mTexture));

		//set pack data alignment to 1 byte read from memory and 1 byte to write to gpu memory...
		OPENGL_CMD(glPixelStorei(GL_PACK_ALIGNMENT, 1));
		OPENGL_CMD(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

		if (w != width || h != height) {
			OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer));
			width = w;
			height = h;
		}
		else
			OPENGL_CMD(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer));

		//back the pack alignment for texture operations
		OPENGL_CMD(glPixelStorei(GL_PACK_ALIGNMENT, originalPackAlignment));
		OPENGL_CMD(glPixelStorei(GL_UNPACK_ALIGNMENT, originalUnpackAlignment));

	}

	void GLTexture::uploadBufferRGBA_8888(const void* buffer, int w, int h) {

		OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, mTexture));

		//set pack data alignment to 1 byte read from memory and 1 byte to write to gpu memory...
		//OPENGL_CMD(glPixelStorei(GL_PACK_ALIGNMENT, 1));
		//OPENGL_CMD(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

		if (w != width || h != height) {
			OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
			width = w;
			height = h;
		}
		else
			OPENGL_CMD(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer));

		//back the pack alignment for texture operations
		//OPENGL_CMD(glPixelStorei(GL_PACK_ALIGNMENT, originalPackAlignment));
		//OPENGL_CMD(glPixelStorei(GL_UNPACK_ALIGNMENT, originalUnpackAlignment));

	}

	//
	// Old way to copy framebuffer to texture... consider to use FBOs to acomplish it better
	//
	// can read rgb(GL_RGB) from framebuffer, or the depth component (GL_DEPTH_COMPONENT24)
	void GLTexture::setSize(int w, int h, GLuint format) {
		OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, mTexture));

		if (format == GL_DEPTH_COMPONENT16 ||
			format == GL_DEPTH_COMPONENT24
			#ifndef ARIBEIRO_RPI
			|| format == GL_DEPTH_COMPONENT32
			#endif
			) {

			OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0));

			//depth buffer force to nearest filtering...
			OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		}
		else if (format == GL_STENCIL_INDEX8) {

			OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, 0));

			//depth buffer force to nearest filtering...
			OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		}
		else if (format == GL_DEPTH24_STENCIL8 ) {
			//
			// requires extension: GLEW_EXT_packed_depth_stencil or GLEW_NV_packed_depth_stencil
			//
			OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0));

			//depth buffer force to nearest filtering...
			OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			OPENGL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		}
		#ifndef ARIBEIRO_RPI
		else if (format == GL_RGBA16F ||
			format == GL_RGB16F ||
			format == GL_RGBA32F ||
			format == GL_RGB32F) {
			OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_RGBA, GL_FLOAT, 0));
		}
		#endif
		else
            OPENGL_CMD(glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, 0));



		width = w;
		height = h;
	}

	//copy the color buffer, or depth buffer, using the texture current size
	void GLTexture::copyFrameBuffer() {
		OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, mTexture));
		OPENGL_CMD(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height));
	}

	void GLTexture::active(int id) {
		OPENGL_CMD(glActiveTexture(GL_TEXTURE0 + id));
		#if !defined(ARIBEIRO_RPI)
            OPENGL_CMD(glEnable(GL_TEXTURE_2D));
		#endif
		OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, mTexture));
	}

	void GLTexture::deactive(int id) {
		OPENGL_CMD(glActiveTexture(GL_TEXTURE0 + id));
		#if !defined(ARIBEIRO_RPI)
            OPENGL_CMD(glDisable(GL_TEXTURE_2D));
        #else
            OPENGL_CMD(glBindTexture(GL_TEXTURE_2D, 0));
		#endif
	}

	GLTexture::~GLTexture()
	{
		if (mTexture != 0 && glIsTexture(mTexture))
			OPENGL_CMD(glDeleteTextures(1, &mTexture));
		mTexture = 0;
	}

	GLTexture *GLTexture::loadFromPNG(const char* filename, bool invertY) {
		int w, h, channels, depth;

		char* buffer = PNGHelper::readPNG(filename, &w, &h, &channels, &depth, invertY);

		if (buffer == NULL) {
			fprintf( stderr, "error to load: %s\n", filename);
			exit(-1);
		}

		if (channels == 1 && depth == 8) {
			GLTexture *result = new GLTexture();
			result->uploadBufferAlpha8(buffer, w, h);
			setNullAndDelete(buffer);
			return result;
		} else if (channels == 3 && depth == 8) {
			GLTexture *result = new GLTexture();
			result->uploadBufferRGB_888(buffer, w, h);
			setNullAndDelete(buffer);
			return result;
		} else if(channels == 4 && depth == 8) {
			GLTexture *result = new GLTexture();
			result->uploadBufferRGBA_8888(buffer, w, h);
			setNullAndDelete(buffer);
			return result;
		}

		PNGHelper::closePNG(buffer);
		//setNullAndDeleteArray(buffer);

		fprintf(stderr, "invalid image format: %d channels %d depth. Error to load: %s\n", channels, depth, filename);
		exit(-1);

		return NULL;
	}

	void GLTexture::writeToPNG(const char* filename, bool invertY) {

        #ifdef ARIBEIRO_RPI

        //
        // OpenGLES needs to create a framebuffer to copy the pixels
        //
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

        char *buff = new char[width*height*4];
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buff);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fbo);

        PNGHelper::writePNG(filename, width, height,4,buff, !invertY);
        delete[] buff;

        #else

            active(0);
            char *buff = new char[width*height*4];
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);
            PNGHelper::writePNG(filename, width, height,4,buff, !invertY);
            delete[] buff;
            deactive(0);
        #endif

	}

}
