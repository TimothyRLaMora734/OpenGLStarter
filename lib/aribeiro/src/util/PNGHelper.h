#ifndef PNGHelper_h
#define PNGHelper_h

namespace aRibeiro {

	class PNGHelper {

	public:
		static char* readPNG(const char *file_name, int *w, int *h, int *chann, int *pixel_depth, bool invertY = false);
		static void writePNG(const char *file_name, int w, int h, int chann, char*buffer, bool invertY = false);
		static void closePNG(char*&buff);
	};

}

#endif
