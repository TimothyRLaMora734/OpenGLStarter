#ifndef PIXELFONTRENDERER_H
#define PIXELFONTRENDERER_H

class PixelFont;

class PixelFontRenderer{
    PixelFont *mPixelFont;
public:
    PixelFontRenderer(PixelFont *pixelFont);

    void print(int x, int y,const wchar_t* UTF_text);
};

#endif // PIXELFONTRENDERER_H
