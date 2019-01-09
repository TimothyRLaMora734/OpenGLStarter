#ifndef PNGWRITER_H
#define PNGWRITER_H

#include <IL/il.h>

class PNGWriter{
    ILuint imageID;
    PNGWriter();
public:
    ~PNGWriter();
    static PNGWriter* getSingleton();
    void writeRGBA8888(char* buffer, int w, int h, const char* filename);
};

#endif // PNGWRITER_H
