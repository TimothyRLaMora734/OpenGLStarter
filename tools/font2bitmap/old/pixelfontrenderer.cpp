#include "pixelfontrenderer.h"

#include "ft2_font_generator.h"

#include <GL/gl.h>
#include <wchar.h>

PixelFontRenderer::PixelFontRenderer(PixelFont *pixelFont){
    mPixelFont = pixelFont;
}

void PixelFontRenderer::print(int x, int y,const wchar_t* UTF_text){
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(1,0,0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_COLOR);

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);//1byte unpacked data

    size_t charsCount = wcslen(UTF_text);
    int currx = x;
    int curry = y;
    int lastx = x;
    bool first = true;
    for(size_t i=0;i<charsCount;i++){
        wchar_t c = UTF_text[i];
        if (c == L'\n'){
            glBegin(GL_LINES);
            glVertex2i(x,y);
            glVertex2i(lastx,y);
            glEnd();

            y += mPixelFont->height();
            currx = x;
            curry = y;
            first = true;
            continue;
        }
        PixelFont::GlyphInfo *info;
        void* buffer;
        if (mPixelFont->getGlyph(c,&info,&buffer)){
            //if (i==0)
            //    glRasterPos2i(currx,curry + info->y);
            //else
            if(first){//line first char processing
                currx -= info->x;
                first = false;
            }
            glRasterPos2i(currx + info->x,curry + info->y);
            glDrawPixels(info->w,info->h,GL_LUMINANCE,GL_UNSIGNED_BYTE,buffer);
            //glBitmap(0,0,0,0,info->advX,info->advY,NULL);
            lastx = currx + info->w + info->x;
            currx += info->advX;
            curry += info->advY;
        }
    }

    glBegin(GL_LINES);
    glVertex2i(x,y);
    glVertex2i(lastx,y);
    glEnd();

}
