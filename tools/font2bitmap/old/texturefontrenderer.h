#ifndef TEXTUREFONTRENDERER_H
#define TEXTUREFONTRENDERER_H

class PixelFont;

#include <map>

class TextureFontRenderer{

public:
    struct GlyphInfo{
        unsigned int texX,texY;
        unsigned int x,y,w,h,advX,advY;
    };

private:
    char* textureBuffer;
    int mW;
    int mH;

    //glyph index in the texture
    std::map<wchar_t, GlyphInfo> glyphs;
    int glyph_w;
    int glyph_h;



public:
    //tenta criar a textura da fonte com as dimensoes especificadas
    //  se nao couber, ele aumenta a dimensao em potencia de 2 até caber
    //Tem o espaçamento entre os caracteres, para evitar problemas devido a filtros de texturas
    //   Se colocar em 0, deve utilizaro filtro nearest
    TextureFontRenderer(PixelFont *pixelFont, int w_h=16, int space_between_chars = 5);
    //a posicao depende da posicao atual do ogl (3D), o desenho é gerado em relacao aos pixels
    void print(const wchar_t* UTF_text);

    void drawPixelBuffer(int x,int y);

    ~TextureFontRenderer();

    TextureFontRenderer(const TextureFontRenderer& param);
    void operator=(const TextureFontRenderer& param);


    void exportLibSceFont(const char* file_meta, const char* file_png);

    GlyphInfo* getGlyph(wchar_t c);
};

#endif // TEXTUREFONTRENDERER_H
