#ifndef color_conversion__H
#define color_conversion__H

#include <aribeiro/vec3.h>
#include <aribeiro/common.h>


#if defined(ARIBEIRO_OS_WINDOWS)
#include <inttypes.h>
#include <sys/types.h>
#include <stdint.h>
#else
#include <sys/types.h>
#endif

namespace aRibeiro {
    
    class FloatColorConversion {
    public:
        static vec3 RGBtoHSV(const vec3 &rgb);
        static vec3 HSVtoRGB(const vec3 &hsv);
        static vec3 RGBtoCIE(const vec3 &rgb);
        static vec3 CIEtoRGB(const vec3 &cie);
    };

    class UByteColorConversion {
    public:
        static void RGBtoCMY(uint8_t r, uint8_t g, uint8_t b, uint8_t *c, uint8_t *m, uint8_t *y);
        static void CMYtoRGB(uint8_t c, uint8_t m, uint8_t y, uint8_t *r, uint8_t *g, uint8_t *b);
        static void RGBtoCMYK(uint8_t r, uint8_t g, uint8_t b, uint8_t *c, uint8_t *m, uint8_t *y, uint8_t *k);
        static void CMYKtoRGB(uint8_t c, uint8_t m, uint8_t y, uint8_t k, uint8_t *r, uint8_t *g, uint8_t *b);
        static void YUVtoRGB(uint8_t y,uint8_t u,uint8_t v,uint8_t *r, uint8_t *g, uint8_t *b);
        static void RGBtoYUV(uint8_t r,uint8_t g,uint8_t b,uint8_t *y, uint8_t *u, uint8_t *v);
    };
        
}


#endif
