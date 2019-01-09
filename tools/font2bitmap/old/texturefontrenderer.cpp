#include "texturefontrenderer.h"
#include "ft2_font_generator.h"

#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>

void bufferWrite(char*dstRGBA, char*originLum,
                 int dstx, int dsty,
                 int w,int h, int limdstW, int limorgW ){
    for (int y=0;y<h;y++){
        for (int x=0;x<w;x++){
            //aqui pode escrever branco com alpha, ao invez de tom de cinza + alpha
            dstRGBA[(x+dstx+(y+dsty)*limdstW)*4+0] = 0;//originLum[(x+y*limorgW)];
            dstRGBA[(x+dstx+(y+dsty)*limdstW)*4+1] = 0;//originLum[(x+y*limorgW)];
            dstRGBA[(x+dstx+(y+dsty)*limdstW)*4+2] = 0;//originLum[(x+y*limorgW)];
            dstRGBA[(x+dstx+(y+dsty)*limdstW)*4+3] = originLum[(x+y*limorgW)];
        }
    }
}

void fillBuffer_RGBA(void* targetBuffer, int w, int h, int spaces, PixelFont *pixelFont,
                     std::map<wchar_t, TextureFontRenderer::GlyphInfo> * newGlyphs){
    std::map<wchar_t, TextureFontRenderer::GlyphInfo>& newGlyphsRef = *newGlyphs;
    memset(targetBuffer,0,w*h*4);
    int x = spaces;
    int y = spaces;

    //increment the line
    int maxy = 0;

    const std::vector<wchar_t>& charset = pixelFont->getCharset();
    for (size_t i=0;i<charset.size();i++){
        wchar_t c = charset[i];
        PixelFont::GlyphInfo *info;
        void* data;
        pixelFont->getGlyph(c,&info,&data);



        int newx = x + info->w + spaces;
        if (newx > w){
            newx = spaces;

            y += maxy + spaces;
            maxy = 0;

            TextureFontRenderer::GlyphInfo glyph;

            glyph.texX = newx;
            glyph.texY = y;
            glyph.x = info->x;
            glyph.y = info->y;
            glyph.w = info->w;
            glyph.h = info->h;
            glyph.advX = info->advX;
            glyph.advY = info->advY;

            newGlyphsRef[c] = glyph;

            //write font to texture
            bufferWrite((char*)targetBuffer, (char*)data,
                        newx,y,
                        info->w,info->h, w, info->w );

            //increment newx
            newx += info->w + spaces;

        }else{

            TextureFontRenderer::GlyphInfo glyph;

            glyph.texX = x;
            glyph.texY = y;
            glyph.x = info->x;
            glyph.y = info->y;
            glyph.w = info->w;
            glyph.h = info->h;
            glyph.advX = info->advX;
            glyph.advY = info->advY;

            newGlyphsRef[c] = glyph;

            //write font to texture
            bufferWrite((char*)targetBuffer, (char*)data,
                        x,y,
                        info->w,info->h, w, info->w );
        }
        if (int(info->h) > maxy)
            maxy = info->h;
        x = newx;
    }
}

//verifica se os caracteres cabem nesse w/h
bool checkSizeForThisResolution(int w,int h, int spaces, PixelFont *pixelFont){
    int x = spaces;
    int y = spaces;

    //increment the line
    int maxy = 0;

    const std::vector<wchar_t>& charset = pixelFont->getCharset();
    for (size_t i=0;i<charset.size();i++){
        wchar_t c = charset[i];
        PixelFont::GlyphInfo *info;
        void* data;
        pixelFont->getGlyph(c,&info,&data);
        int newx = x + info->w + spaces;
        if (i==0 && newx > w){
            return false;
        }else if (newx > w){
            newx = spaces;
            y += maxy + spaces;
            maxy = 0;
            newx += info->w + spaces;
            if (newx > w){
                return false;
            }
        }
        if (int(info->h) > maxy)
            maxy = info->h;
        x = newx;
    }
    if (y+maxy+spaces > h)
        return false;
    return true;
}

#include <stdio.h>

void computeTextureMinSize(int spaces, int iw,int ih,PixelFont *pixelFont, int *wr, int *hr){
    int w = iw;
    int h = ih;
    //space between fonts
    //int spaces = 1;

    //faz incrementar um eixo, depois outro quando aumentar o tamanho da area da textura

    int double_turn = 0;
    printf("Double turn OFF!!!\n");
    printf("w: %d h: %d\n",w,h);
    while (!checkSizeForThisResolution(w,h,spaces,pixelFont)){
        if (double_turn == 0)
            w = w << 1;
        else
            h = h << 1;
        double_turn = (double_turn+1)%2;
        printf("w: %d h: %d\n",w,h);
    }
    *wr = w;
    *hr = h;

    //test with double_turn on
    double_turn = 1;
    w = iw;
    h = ih;
    printf("Double turn ON!!!\n");
    printf("w: %d h: %d\n",w,h);
    while (!checkSizeForThisResolution(w,h,spaces,pixelFont)){
        if (double_turn == 0)
            w = w << 1;
        else
            h = h << 1;
        double_turn = (double_turn+1)%2;
        printf("w: %d h: %d\n",w,h);
    }

    printf("Check fouble turn result...\n");
    if ((*wr)*(*hr) > w*h){
        *wr = w;
        *hr = h;
    }

    printf("Best resolution: %dx%d !!!\n",*wr,*hr);

}

void invertBuffer(int w,int h,char*buffer){
    char*aux = new char[w*h*4];
    for (int i=0;i<h/2;i++){
        memcpy(aux,&buffer[i*w*4],w*4);
        memcpy(&buffer[i*w*4],&buffer[(h-1-i)*w*4],w*4);
        memcpy(&buffer[(h-1-i)*w*4],aux,w*4);
    }
    delete[] aux;
}

TextureFontRenderer::TextureFontRenderer(PixelFont *pixelFont, int w_h, int space_between_chars){
    int finalW,finalH;
    //check the w/h param
    computeTextureMinSize(space_between_chars,w_h,w_h,pixelFont, &finalW,&finalH);
    //this->mWH = finalW;
    mW = finalW;
    mH = finalH;
    printf("Final texture size: %dx%d\n",mW,mH);
    fflush(stdout);
    //alloc the buffer
    textureBuffer = (char*)malloc(mW*mH*4);//RGBA - 8bpp

    glyph_w = pixelFont->width();
    glyph_h = pixelFont->height();


    //generate texture
    fillBuffer_RGBA(textureBuffer, mW, mH, space_between_chars, pixelFont, &glyphs);
    invertBuffer(mW,mH,textureBuffer);
}

TextureFontRenderer::~TextureFontRenderer(){
    if (textureBuffer)
        free(textureBuffer);
}

void TextureFontRenderer::print(const wchar_t* UTF_text){

}

void TextureFontRenderer::drawPixelBuffer(int x,int y){
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(1,1,0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);//1byte unpacked data
    glRasterPos2i(x,y);
    glBitmap(0,0,0,0,0,-mH,NULL);

    //char* textureBuffer_inv = new char[mW*mH*4];
    //for (int y=0;y<mH;y++){
      //  memcpy(&textureBuffer_inv[(mH-1-y)*mW*4],&textureBuffer[y*mW*4],mW*4);
    //}

    glDrawPixels(mW,mH,GL_RGBA,GL_UNSIGNED_BYTE,textureBuffer);

    glColor3f(1,1,1);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x,y);
    glVertex2f(x+mW,y);
    glVertex2f(x+mW,y+mH);
    glVertex2f(x,y+mH);
    glVertex2f(x,y);
    glEnd();

    //delete[] textureBuffer_inv;


}

TextureFontRenderer::TextureFontRenderer(const TextureFontRenderer& param){
    //memcpy(this,&param,sizeof(TextureFontRenderer));

    mW = param.mW;
    mH = param.mH;
    glyphs = param.glyphs;
    glyph_w = param.glyph_w;
    glyph_h = param.glyph_h;

    if (this->textureBuffer){
        textureBuffer = (char*)malloc(mW*mH*4);
        memcpy(textureBuffer,param.textureBuffer,sizeof(char)*mW*mH*4);
    }
}

void TextureFontRenderer::operator=(const TextureFontRenderer& param){
    if (textureBuffer)
        free(textureBuffer);
    //memcpy(this,&param,sizeof(TextureFontRenderer));

    mW = param.mW;
    mH = param.mH;
    glyphs = param.glyphs;
    glyph_w = param.glyph_w;
    glyph_h = param.glyph_h;

    if (this->textureBuffer){
        textureBuffer = (char*)malloc(mW*mH*4);
        memcpy(textureBuffer,param.textureBuffer,sizeof(char)*mW*mH*4);
    }
}


#include "pngwriter.h"


TextureFontRenderer::GlyphInfo* TextureFontRenderer::getGlyph(wchar_t c){
    std::map<wchar_t, GlyphInfo>::iterator it = glyphs.find(c);
    if (it != glyphs.end()){
        return &it->second;
    }
    return NULL;
}


void TextureFontRenderer::exportLibSceFont(const char* file_meta, const char* file_png){
    PNGWriter::getSingleton()->writeRGBA8888(textureBuffer,mW,mH,file_png);

    FILE* out = fopen(file_meta,"w");
    if (out){
        int space_width = glyph_w;
        //get space glyph
        GlyphInfo* space = getGlyph(L' ');
        if (space)
            space_width = space->advX;
        //write header
        fprintf(out,"FONT_TOOL_FILE %u %u %u %u %u %u\n",
               mW,//<TexW>
               mH,//<TexH>
               space_width,//<Space_size_x>
               glyph_h,//<new_line_size_y>
               1,//<pixel_format=1(8888)>
               glyphs.size()//<GlyphsCount>
               );
        //iterate over all glyphs
        std::map<wchar_t, TextureFontRenderer::GlyphInfo>::iterator it;
        for (it = glyphs.begin();it!=glyphs.end();it++){
            wchar_t c = it->first;
            TextureFontRenderer::GlyphInfo& glyph = it->second;
            fprintf(out,"%u %d %u %u %u %u %u\n",
                   (unsigned int)c,//<unicode char code>
                   glyph.y,//<ascent_y>
                   glyph.texX,//<tex_left_x>
                   glyph.texY,//<tex_top_y>
                   glyph.w,//<glyph_w>
                   glyph.h,//<glyph_h>
                   glyph.advX//<increment_x>
                   );

        }

        fclose(out);
    }

}
