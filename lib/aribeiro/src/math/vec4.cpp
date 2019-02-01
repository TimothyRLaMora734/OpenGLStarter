#include "vec4.h"
#include "all_math.h"
#include <string.h>

namespace aRibeiro{

vec4::vec4(){
    x = y = z = w = 0.0f;
}
vec4::vec4( const float v ){
    x = y = z = w = v;
}
vec4::vec4( const float x, const float y, const float z, const float w ){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
vec4::vec4( const vec3 &xyz , float w){
    x = xyz.x;
    y = xyz.y;
    z = xyz.z;
    this->w = w;
}
vec4::vec4( float x, const vec3 &yzw){
    this->x = x;
    y = yzw.x;
    z = yzw.y;
    w = yzw.z;
}
vec4::vec4( const vec4 &v ){
    *this = v;
}
vec4::vec4( const vec4 &a, const vec4 &b ){
    x = b.x - a.x;
    y = b.y - a.y;
    z = b.z - a.z;
    w = b.w - a.w;
}
bool vec4::operator==(const vec4&v) const {
	return memcmp(array, v.array, sizeof(float) * 4) == 0;
}
bool vec4::operator!=(const vec4&v) const {
	return memcmp(array, v.array, sizeof(float) * 4) != 0;
}
vec4& vec4::operator+=(const vec4& v){
    x+=v.x;
    y+=v.y;
    z+=v.z;
    w+=v.w;
    return (*this);
}
vec4& vec4::operator-=(const vec4& v){
    x-=v.x;
    y-=v.y;
    z-=v.z;
    w-=v.w;
    return (*this);
}
vec4 vec4::operator-(){
    return vec4(-x,-y,-z,-w);
}
vec4& vec4::operator*=(const vec4& v){
    x*=v.x;
    y*=v.y;
    z*=v.z;
    w*=v.w;
    return (*this);
}
vec4& vec4::operator/=(const vec4& v){
    x/=v.x;
    y/=v.y;
    z/=v.z;
    w/=v.w;
    return (*this);
}
vec4& vec4::operator+=(const float v){
    x+=v;
    y+=v;
    z+=v;
    w+=v;
    return (*this);
}
vec4& vec4::operator-=(const float v){
    x-=v;
    y-=v;
    z-=v;
    w-=v;
    return (*this);
}
vec4& vec4::operator*=(const float v){
    x*=v;
    y*=v;
    z*=v;
    w*=v;
    return (*this);
}
vec4& vec4::operator/=(const float v){
    x/=v;
    y/=v;
    z/=v;
    w/=v;
    return (*this);
}
float& vec4::operator[](const int v){
    return array[v];
}

float vec4::operator[](const int v)const {
	return array[v];
}

}
