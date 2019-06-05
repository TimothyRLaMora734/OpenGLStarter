#include "vec2.h"
#include "all_math.h"

namespace aRibeiro{

#if defined(ARIBEIRO_SSE2)
    
    const __m128 _vec2_zero_sse = _mm_set1_ps(0.0f);
    const __m128 _vec2_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
    
#endif
    
    /*
vec2::vec2(){
    x = y = 0.0f;
}
vec2::vec2( const float v ){
    x = y = v;
}
vec2::vec2( const float x, const float y ){
    this->x = x;
    this->y = y;
}
vec2::vec2( const vec2 &v ){
    *this = v;
}
vec2::vec2( const vec2 &a, const vec2 &b ){
    x = b.x - a.x;
    y = b.y - a.y;
}
bool vec2::operator==(const vec2&v) const {
    for(int i=0;i<2;i++){
        if (absv(array[i]-v.array[i]) > 1e-4f)
            return false;
    }
    return true;
	//return memcmp(array, v.array, sizeof(float) * 2) == 0;
}
bool vec2::operator!=(const vec2&v) const {
    return !((*this) == v);
	//return memcmp(array, v.array, sizeof(float) * 2) != 0;
}

vec2& vec2::operator+=(const vec2& v){
    x+=v.x;
    y+=v.y;
    return (*this);
}

vec2& vec2::operator-=(const vec2& v){
    x-=v.x;
    y-=v.y;
    return (*this);
}

vec2 vec2::operator-() const{
    return vec2(-x,-y);
}

vec2& vec2::operator*=(const vec2& v){
    x*=v.x;
    y*=v.y;
    return (*this);
}

vec2& vec2::operator/=(const vec2& v){
    x/=v.x;
    y/=v.y;
    return (*this);
}

vec2& vec2::operator+=(const float v){
    x+=v;
    y+=v;
    return (*this);
}

vec2& vec2::operator-=(const float v){
    x-=v;
    y-=v;
    return (*this);
}

vec2& vec2::operator*=(const float v){
    x*=v;
    y*=v;
    return (*this);
}

vec2& vec2::operator/=(const float v){
    x/=v;
    y/=v;
    return (*this);
}

float& vec2::operator[](const int v){
    return array[v];
}

float vec2::operator[](const int v)const {
	return array[v];
}

     */
}
