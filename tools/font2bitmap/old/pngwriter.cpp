#include "pngwriter.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


PNGWriter::PNGWriter(){
    ilInit();
    ilGenImages(1, &imageID);
    ilBindImage(imageID);
    ilEnable(IL_FILE_OVERWRITE);
}

PNGWriter::~PNGWriter(){
    ilDeleteImages(1,&imageID);
    ilShutDown();
}

PNGWriter* PNGWriter::getSingleton(){
    static PNGWriter mPNGWriter;
    return &mPNGWriter;
}

void PNGWriter::writeRGBA8888(char* buffer, int w, int h, const char* filename){

    ilTexImage( w, h,
                   0,
                   4,//4 bytes per pixel
                    IL_RGBA,
                    IL_UNSIGNED_BYTE,
                    buffer
                    );
    ilSave(IL_PNG, filename);
}


