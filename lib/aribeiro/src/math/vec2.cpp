#include "vec2.h"
#include "all_math.h"
#include <string.h>

namespace aRibeiro{

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
    return memcmp(array, v.array, sizeof(float)*2)==0;
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

vec2 vec2::operator-(){
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

}
