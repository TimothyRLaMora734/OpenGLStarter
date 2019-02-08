#ifndef color_conversion__H
#define color_conversion__H

#include <aribeiro/vec3.h>
#include <aribeiro/common.h>
#if defined(ARIBEIRO_OS_WINDOWS)
#include <inttypes.h>
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
        static void RGBtoCMY(u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t *c, u_int8_t *m, u_int8_t *y);
        static void CMYtoRGB(u_int8_t c, u_int8_t m, u_int8_t y, u_int8_t *r, u_int8_t *g, u_int8_t *b);
        static void RGBtoCMYK(u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t *c, u_int8_t *m, u_int8_t *y, u_int8_t *k);
        static void CMYKtoRGB(u_int8_t c, u_int8_t m, u_int8_t y, u_int8_t k, u_int8_t *r, u_int8_t *g, u_int8_t *b);
        static void YUVtoRGB(u_int8_t y,u_int8_t u,u_int8_t v,u_int8_t *r, u_int8_t *g, u_int8_t *b);
        static void RGBtoYUV(u_int8_t r,u_int8_t g,u_int8_t b,u_int8_t *y, u_int8_t *u, u_int8_t *v);
    };
        
}


#endif
