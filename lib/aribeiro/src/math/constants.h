
#ifndef constants_h
#define constants_h

#include <aribeiro/common.h>

#include "limits.h"
#include "float.h"
#include <cmath> //para sqrtf

#ifdef __cplusplus
namespace aRibeiro {
#endif

const float PI = 3.1415926535897932384626433832795f;
const float _PI_180 = 0.0174532925199432957692222222222222f;
const float _180_PI = 57.2957795130823208767981548141052f;

#define RAD2DEG(rad) ((rad)*(_180_PI))
#define DEG2RAD(deg) ((deg)*(_PI_180))

union FloatFromBinaryContent{
  unsigned int binaryContent;
  float value;
};
const FloatFromBinaryContent SPD_FLOAT_INFINITY_UNION = {0x7f800000};
const float SPD_FLT_INF = SPD_FLOAT_INFINITY_UNION.value;


#define _sqrt sqrtf
#define _rsqrt(x) (1.0f/sqrtf(x))

#ifndef sqrtf
#define sqrtf(V) (float)(sqrt(V))
#endif

#ifndef cosf
#define cosf(V) (float)(cos(V))
#endif

#ifndef sinf
#define sinf(V) (float)(sin(V))
#endif

#ifndef tanf
#define tanf(V) (float)(tan(V))
#endif

#undef Myabs
#define Myabs(V) ((V<0)?-V:V)



    const float EPSILON = 1e-4f;//0.001f;
    const float EPSILON2 = 1e-6f;//0.0001f;

#ifdef __cplusplus
}
#endif

//#include "CossineSineArray.h"

#endif

