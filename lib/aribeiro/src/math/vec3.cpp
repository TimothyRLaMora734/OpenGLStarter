#include "vec3.h"
#include "all_math.h"
#include <string.h>
namespace aRibeiro{

#if defined(ARIBEIRO_SSE2)
    const __m128 _vec3_zero_sse = _mm_set1_ps(0.0f);
    const __m128 _vec3_sign_mask = _mm_load_(-0.f,-0.f,-0.f,0.0f);//_mm_set1_ps(-0.f); // -0.f = 1 << 31
#endif

    /*
vec3::vec3(){
#if defined(ARIBEIRO_SSE2)
    array_sse = _vec3_zero_sse;
#else
    x = y = z = 0.0f;
#endif
}

vec3::vec3( const float v ){
#if defined(ARIBEIRO_SSE2)
    array_sse = (__m128){v,v,v,0};
#else
    x = y = z = v;
#endif
    
}

vec3::vec3( const float x, const float y, const float z ){
#if defined(ARIBEIRO_SSE2)
    array_sse = (__m128){x,y,z,0};
#else
    this->x = x;
    this->y = y;
    this->z = z;
#endif
}
vec3::vec3( const vec2 &xy , float z){
#if defined(ARIBEIRO_SSE2)
    array_sse = (__m128){xy.x,xy.y,z,0};
#else
    x = xy.x;
    y = xy.y;
    this->z = z;
#endif
}
vec3::vec3( float x, const vec2 &yz){
#if defined(ARIBEIRO_SSE2)
    array_sse = (__m128){x,yz.x,yz.y,0};
#else
    this->x = x;
    y = yz.x;
    z = yz.y;
#endif
}

vec3::vec3( const vec3 &v ){
#if defined(ARIBEIRO_SSE2)
    array_sse = v.array_sse;
#else
    *this = v;
#endif
}

vec3::vec3( const vec3 &a, const vec3 &b ){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_sub_ps(a.array_sse, b.array_sse);
#else
    x = b.x - a.x;
    y = b.y - a.y;
    z = b.z - a.z;
#endif
}

bool vec3::operator==(const vec3&v) const {
#if defined(ARIBEIRO_SSE2)
    __m128 diff_abs = _mm_sub_ps(array_sse, v.array_sse);
    //abs
    diff_abs = _mm_andnot_ps(_vec3_sign_mask, diff_abs);
    
    //static const __m128 epsilon = _mm_set1_ps(1e-4f); // -0.f = 1 << 31
    //_mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));
    
    for(int i=0;i<3;i++){
        if (diff_abs[i] > 1e-4f)
            return false;
    }
    
    return true;
#else
    for(int i=0;i<3;i++){
        if (absv(array[i]-v.array[i]) > 1e-4f)
            return false;
    }
    return true;
	//return memcmp(array, v.array, sizeof(float) * 3) == 0;
#endif
}

bool vec3::operator!=(const vec3&v) const {
    return !((*this) == v);
	//return memcmp(array, v.array, sizeof(float) * 3) != 0;
}

vec3& vec3::operator+=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_add_ps(array_sse, v.array_sse);
#else
    x+=v.x;
    y+=v.y;
    z+=v.z;
#endif
    return (*this);
}

vec3& vec3::operator-=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_sub_ps(array_sse, v.array_sse);
#else
    x-=v.x;
    y-=v.y;
    z-=v.z;
#endif
    return (*this);
}

vec3 vec3::operator-() const{
#if defined(ARIBEIRO_SSE2)
    
    vec3 result;
    
    result.array_sse = _mm_xor_ps(_vec3_sign_mask, array_sse);
    
    return result;
#else
    return vec3(-x,-y,-z);
#endif
}

vec3& vec3::operator*=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_mul_ps(array_sse, v.array_sse);
#else
    x*=v.x;
    y*=v.y;
    z*=v.z;
#endif
    return (*this);
}

vec3& vec3::operator/=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
    __m128 param = (__m128){v.x,v.y,v.z,1.0f};
    array_sse = _mm_div_ps(array_sse, param);
#else
    x/=v.x;
    y/=v.y;
    z/=v.z;
#endif
    return (*this);
}

vec3& vec3::operator+=(const float v){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_add_ps(array_sse, _mm_set1_ps(v));
#else
    x+=v;
    y+=v;
    z+=v;
#endif
    return (*this);
}

vec3& vec3::operator-=(const float v){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_sub_ps(array_sse, _mm_set1_ps(v));
#else
    x-=v;
    y-=v;
    z-=v;
#endif
    return (*this);
}

vec3& vec3::operator*=(const float v){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_mul_ps(array_sse, _mm_set1_ps(v));
#else
    x*=v;
    y*=v;
    z*=v;
#endif
    return (*this);
}

vec3& vec3::operator/=(const float v){
#if defined(ARIBEIRO_SSE2)
    array_sse = _mm_div_ps(array_sse, _mm_set1_ps(v));
#else
    x/=v;
    y/=v;
    z/=v;
#endif
    return (*this);
}

float& vec3::operator[](const int v){
    return array[v];
}

float vec3::operator[](const int v)const {
	return array[v];
}
*/
}
