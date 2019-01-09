#ifndef platform_gl__h__
#define platform_gl__h__

namespace aRibeiro {

	class PlatformGL {
	public:

#ifdef NDEBUG

#define OPENGL_CMD(expr) expr

#else

#define OPENGL_CMD(expr) do { \
		expr; \
		PlatformGL::glCheckError(__FILE__, __LINE__, #expr); \
	} while (false)

#endif

		static void glCheckError(const char* file, unsigned int line, const char* expression);
		static void checkShaderStatus(int shader);
		static void checkProgramStatus(int program);

	};



}

#endif