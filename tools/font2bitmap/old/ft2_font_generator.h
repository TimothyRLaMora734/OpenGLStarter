#ifndef FT2_FONT_GENERATOR_H
#define FT2_FONT_GENERATOR_H

enum FT2_format{
    FORMAT_RGBA_8888,//better for AA color pixel fonts with alpha
    FORMAT_RGB_888,
    FORMAT_LUMINANCE_8,//better size in memory, but can have issues with alpha
    FORMAT_BIT_1 // better for bit fonts
};

enum FT2_size_type{
    SIZE_TYPE_PT,
    SIZE_TYPE_PIXEL
};

//known devices
//  windows default DPI = 96
//  mac default DPI = 72
// iphone4 - 960-by-640-pixel resolution at 326 ppi
// iphone - 480 x 320 163 pixels per inch
// ipod touch - 480 x 320 160 pixels per inch
//
struct FT2DeviceInfo{
    const char name[1024];
    int device_width;
    int device_height;
    int device_DPI;
};

class FT2Configuration{
public:
    FT2Configuration(const FT2Configuration& init);

    FT2Configuration(int align=4, // 4 bytes
                     FT2_format format=FORMAT_RGBA_8888, //rgba default
                     //if the size_type choosed is pixel then you must pass just the with and height
                     //   otherwise, you must specify all the others parameters
                     FT2_size_type size_type = SIZE_TYPE_PIXEL,
                     //the unity of the width and height depends on the size_type specified.
                     //    if size_type is SIZE_TYPE_PIXEL then it is pixels
                     //    if size_type is SIZE_TYPE_PT then it is pt
                     int char_max_width = 14,
                     int char_max_height = 14,
                     int device_width = 640,
                     int device_height = 480,
                     int device_DPI = 64, // each pixel size will multiply by 1/64 of points
                     const wchar_t *unicode_character_set =
                     L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz:”\"“'áãàéêíóúñôõâü.,1234567890¡!@#$%&*();-¿?ÂÁÉÍÔÚ\\=+_®ª©/°◦º™ "
                     );
    ~FT2Configuration();
    void operator=(const FT2Configuration& param);
//private:
    int m_alignment;
    FT2_format m_format;
    FT2_size_type m_size_type;
    int m_char_max_width;
    int m_char_max_height;
    FT2DeviceInfo m_deviceInfo;

    wchar_t *m_unicode_character_set;

};

#include <map>
#include <vector>
class PixelFontRenderer;
class TextureFontRenderer;

class PixelFont{

public:
    struct GlyphInfo{
        unsigned int x,y,w,h,advX,advY;
        //unsigned int binaryDataBegin;
    };
    PixelFont(int w, int h);
    //aqui tem q passar os parametros do FT2 do face/glyph
    void addGlyph(wchar_t c,
                  int x,
                  int y,
                  int w,
                  int h,
                  int advX,
                  int advY,
                  const void* buffer_w_h,
                  bool inverted = false);
    ~PixelFont();

    bool getGlyph(wchar_t c,
                  GlyphInfo** info,
                  void** data);

    const std::vector<wchar_t>& getCharset();

    PixelFontRenderer getPixelFontRenderer();
    TextureFontRenderer getTextureFontRenderer();

    PixelFont* copyInverted();



    int height();
    int width();
private:
    //aloca memoria para um glyph, e retorna os ponteiros de cada coisa
    void glyphAlloc(int w,int h, GlyphInfo** glyph, void** buffer, int* binaryPointer);
//header
//std::map<wchar_t, GlyphInfo> glyphs;
std::map<wchar_t, int> glyphs;//tem um ponteiro para a posicao do glyph no binaryData
int glyph_w;
int glyph_h;
void* binaryData;
size_t binaryDataSize;

std::vector<wchar_t> mCharset;

};

class PixelFontFactory{
public:
    static PixelFont* generatePixelFont(const char* ttf_filename,
                                        const FT2Configuration& configuration,
                                        bool inverted);
};

#endif // FT2_FONT_GENERATOR_H
