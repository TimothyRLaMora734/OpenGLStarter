#include "ft2_font_generator.h"

#include <wchar.h>
#include <string.h>

//freetype include
#include <ft2build.h>
#include FT_FREETYPE_H


#include <iostream>
#define __STDC_CONSTANT_MACROS
#include <stdint.h>

#include <stdlib.h>
#include <assert.h>
#include "pixelfontrenderer.h"
#include "texturefontrenderer.h"

using namespace std;
#define FT_CHECK_ERROR(_fnc, message) \
  if ((_fnc) != 0){ \
    cerr << message << endl; \
    exit(-1); \
  }

FT_Library  library = NULL;

class StaticInitFT2{
public:
    StaticInitFT2(){
        FT_CHECK_ERROR( FT_Init_FreeType( &library ) ,
                           "Erro do initialize FT2 lib" );
        printf("FT2 initted!!\n");
    }
    ~StaticInitFT2(){
        FT_Done_FreeType(library);
    }
};
StaticInitFT2 mStaticInitFT2;



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
FT2Configuration::FT2Configuration(int align, // 4 bytes
                     FT2_format format, //rgba default
                     //if the size_type choosed is pixel then you must pass just the with and height
                     //   otherwise, you must specify all the others parameters
                     FT2_size_type size_type,
                     //the unity of the width and height depends on the size_type specified.
                     //    if size_type is SIZE_TYPE_PIXEL then it is pixels
                     //    if size_type is SIZE_TYPE_PT then it is pt
                     int char_max_width,
                     int char_max_height,
                     int device_width,
                     int device_height,
                     int device_DPI, // each pixel size will multiply by 1/64 of points
                     const wchar_t *unicode_character_set
                     ){
    m_alignment = align;
    m_format = format;
    m_size_type = size_type;
    m_char_max_width = char_max_width;
    m_char_max_height = char_max_height;
    m_deviceInfo.device_width = device_width;
    m_deviceInfo.device_height = device_height;
    m_deviceInfo.device_DPI = device_DPI;

    size_t num_chars = wcslen(unicode_character_set)+1;
    m_unicode_character_set = new wchar_t[num_chars+2];
    memset(m_unicode_character_set,0,num_chars*sizeof(wchar_t));
    wcscpy(m_unicode_character_set,unicode_character_set);
    m_unicode_character_set[num_chars-1] = 0xe7;
    m_unicode_character_set[num_chars] = 0xc7;
    m_unicode_character_set[num_chars+1] = 0x00;
}

FT2Configuration::FT2Configuration(const FT2Configuration& param){
    memcpy(this,&param,sizeof(FT2Configuration));
    if (param.m_unicode_character_set){
        size_t num_chars = wcslen(param.m_unicode_character_set)+1;
        m_unicode_character_set = new wchar_t[num_chars];
        memset(m_unicode_character_set,0,num_chars*sizeof(wchar_t));
        wcscpy(m_unicode_character_set,param.m_unicode_character_set);
    }
}

FT2Configuration::~FT2Configuration(){
    if (m_unicode_character_set)
        delete[] m_unicode_character_set;
}

void FT2Configuration::operator=(const FT2Configuration& param){
    if (m_unicode_character_set)
        delete[] m_unicode_character_set;
    memcpy(this,&param,sizeof(FT2Configuration));
    if (param.m_unicode_character_set){
        size_t num_chars = wcslen(param.m_unicode_character_set)+1;
        m_unicode_character_set = new wchar_t[num_chars];
        memset(m_unicode_character_set,0,num_chars*sizeof(wchar_t));
        wcscpy(m_unicode_character_set,param.m_unicode_character_set);
    }
    //return *this;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
PixelFont::PixelFont(int w,int h){
    glyph_w = w;
    glyph_h = h;
    binaryData = NULL;
    binaryDataSize = 0;
}
PixelFont::~PixelFont(){
    if (binaryData)
        free(binaryData);
}

PixelFontRenderer PixelFont::getPixelFontRenderer(){
    return PixelFontRenderer(this);
}

TextureFontRenderer PixelFont::getTextureFontRenderer(){
    return TextureFontRenderer(this);
}

PixelFont* PixelFont::copyInverted(){
    PixelFont* result = new PixelFont(glyph_w,glyph_h);

    for (size_t i=0;i<mCharset.size();i++){
        wchar_t c = mCharset[i];
        PixelFont::GlyphInfo *info;
        void* data;
        getGlyph(c,&info,&data);
        result->addGlyph(c,info->x,info->y,info->w,info->h,info->advX,info->advY,data,true);
    }
    return result;
    //result->glyphs = glyphs;
    //void* binaryData;
    //size_t binaryDataSize;
    //result->mCharset = mCharset;
}

bool PixelFont::getGlyph(wchar_t c,
                          GlyphInfo** info,
                          void** data){
    std::map<wchar_t, int>::iterator it = glyphs.find(c);
    if (it == glyphs.end())
        return false;
    int bufferIndex = it->second;
    *info = (GlyphInfo*)((char*)binaryData + bufferIndex);
    *data = ((char*)binaryData + bufferIndex + sizeof(GlyphInfo));
    return true;
}

const std::vector<wchar_t>& PixelFont::getCharset(){
    return mCharset;
}

//aqui tem q passar os parametros do FT2 do face/glyph
void PixelFont::addGlyph(wchar_t c,
                         int x,
                         int y,
                         int w,
                         int h,
                         int advX,
                         int advY,
                         const void* buffer_w_h,
                         bool inverted){
    GlyphInfo* newGlyph;
    void* bufferToWrite;
    int bufferPosition;
    glyphAlloc(w,h,&newGlyph, &bufferToWrite, &bufferPosition);

    newGlyph->x = x;
    if (inverted)
        newGlyph->y = h-y-1;
    else
        newGlyph->y = y;
    newGlyph->w = w;
    newGlyph->h = h;
    newGlyph->advX = advX;
    newGlyph->advY = advY;

    if (inverted){
        for (int i=0;i<h;i++)
            memcpy((char*)bufferToWrite+i*w,(char*)buffer_w_h + (h-1-i)*w,w*sizeof(char));
    }else{
        memcpy(bufferToWrite,buffer_w_h,w*h*sizeof(char));
    }

    this->glyphs[c] = bufferPosition;

    mCharset.push_back(c);
}

void PixelFont::glyphAlloc(int w,int h, GlyphInfo** glyph, void** buffer, int* binaryPointer){
    //8bpp*w*h + 1 * GlyphInfo
    size_t bytesToAlloc = w*h*sizeof(char)+ sizeof(GlyphInfo);
    *binaryPointer = binaryDataSize;
    binaryDataSize += bytesToAlloc;
    if (!binaryData)
        binaryData = malloc(binaryDataSize);
    else
        binaryData = realloc(binaryData,binaryDataSize);
    assert(binaryData);
    *glyph = (GlyphInfo*)((char*)binaryData + (*binaryPointer));
    *buffer = ((char*)binaryData + (*binaryPointer) + sizeof(GlyphInfo));
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
PixelFont* PixelFontFactory::generatePixelFont(const char* ttf_filename,
                             const FT2Configuration& configuration,
                             bool inverted){
    //return NULL;
    PixelFont *result = NULL;
    FT_Face face;
    FT_Int face_index_to_load = 0;
    FT_CHECK_ERROR( FT_New_Face( library,ttf_filename,face_index_to_load,&face ) ,
                   "Erro do load font file FT2 lib" );

    //aqui a face ja esta carregada
    cout << "File Opened: " << ttf_filename << endl;
    cout << "Faces: " << face->num_faces << endl;
    cout << "Selected face: " << face->face_index << endl;
    cout << "Family: " << face->family_name << endl;
    cout << "Style: " << face->style_name << endl;

    result = new PixelFont(configuration.m_char_max_width,configuration.m_char_max_height);
    if(configuration.m_size_type==SIZE_TYPE_PIXEL){
        //device independent font pixel resolution
        FT_CHECK_ERROR( FT_Set_Pixel_Sizes(
                        face,   // handle to face object
                        configuration.m_char_max_width,      // pixel_width
                        configuration.m_char_max_height ),   // pixel_height
                        "error to set the font size");
    }else if (configuration.m_size_type==SIZE_TYPE_PT){
        //device dependent font pixel resolution
        int DPI = configuration.m_deviceInfo.device_DPI;
        FT_CHECK_ERROR( FT_Set_Char_Size(face,    // handle to face object
                                         DPI*configuration.m_char_max_width,       // char_width in 1/64th of points
                                         DPI*configuration.m_char_max_height,  // char_height in 1/64th of points
                                         configuration.m_deviceInfo.device_width,     // horizontal device resolution
                                         configuration.m_deviceInfo.device_height ),
                       "error to set the font size");   // vertical device resolution
    }else{
        delete result;
        FT_Done_Face(face);
        return NULL;
    }

    //generate the glyphs data
    for (size_t i=0;i<wcslen(configuration.m_unicode_character_set);i++){
        //converting the char encoding to glyph index
        //FT_ULong charcode_utf32 = L'ç';
        FT_ULong charcode_utf32 = configuration.m_unicode_character_set[i];
        FT_Int glyph_index = FT_Get_Char_Index( face, charcode_utf32 );
        if (glyph_index == 0){
            cout << "glyph not found" << endl;
            continue;
        }
        FT_ULong load_flags = FT_LOAD_DEFAULT;
        FT_CHECK_ERROR( FT_Load_Glyph( face, glyph_index, load_flags ),
                       "Error to load the character");
        //FT_RENDER_MODE_NORMAL - 8bpp grayscale
        //FT_RENDER_MODE_MONO - 1bpp
        FT_Render_Mode render_flags = FT_RENDER_MODE_NORMAL;
        FT_CHECK_ERROR( FT_Render_Glyph( face->glyph, render_flags ),
                       "Error to render the character");
        //customRenderBitmap(face->glyph);
        result->addGlyph(configuration.m_unicode_character_set[i],
                                face->glyph->bitmap_left,
                                face->glyph->bitmap_top,
                                face->glyph->bitmap.width,
                                face->glyph->bitmap.rows,
                                (face->glyph->advance.x >> 6),
                                (face->glyph->advance.y >> 6),
                                face->glyph->bitmap.buffer,
                                inverted);

    }
    FT_Done_Face(face);
    //face = 0;
    return result;
}

int PixelFont::height(){
    return glyph_h;
}

int PixelFont::width(){
    return glyph_w;
}

