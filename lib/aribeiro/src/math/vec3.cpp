#include "vec3.h"
#include "all_math.h"
#include <string.h>
namespace aRibeiro{

vec3::vec3(){
    x = y = z = 0.0f;
}

vec3::vec3( const float v ){
    x = y = z = v;
}

vec3::vec3( const float x, const float y, const float z ){
    this->x = x;
    this->y = y;
    this->z = z;
}
vec3::vec3( const vec2 &xy , float z){
    x = xy.x;
    y = xy.y;
    this->z = z;
}
vec3::vec3( float x, const vec2 &yz){
    this->x = x;
    y = yz.x;
    z = yz.y;
}

vec3::vec3( const vec3 &v ){
    *this = v;
}

vec3::vec3( const vec3 &a, const vec3 &b ){
    x = b.x - a.x;
    y = b.y - a.y;
    z = b.z - a.z;
}

bool vec3::operator==(const vec3&v) const {
    for(int i=0;i<3;i++){
        if (absv(array[i]-v.array[i]) > 1e-4f)
            return false;
    }
    return true;
	//return memcmp(array, v.array, sizeof(float) * 3) == 0;
}

bool vec3::operator!=(const vec3&v) const {
    return !((*this) == v);
	//return memcmp(array, v.array, sizeof(float) * 3) != 0;
}

vec3& vec3::operator+=(const vec3& v){
    x+=v.x;
    y+=v.y;
    z+=v.z;
    return (*this);
}

vec3& vec3::operator-=(const vec3& v){
    x-=v.x;
    y-=v.y;
    z-=v.z;
    return (*this);
}

vec3 vec3::operator-() const{
    return vec3(-x,-y,-z);
}

vec3& vec3::operator*=(const vec3& v){
    x*=v.x;
    y*=v.y;
    z*=v.z;
    return (*this);
}

vec3& vec3::operator/=(const vec3& v){
    x/=v.x;
    y/=v.y;
    z/=v.z;
    return (*this);
}

vec3& vec3::operator+=(const float v){
    x+=v;
    y+=v;
    z+=v;
    return (*this);
}

vec3& vec3::operator-=(const float v){
    x-=v;
    y-=v;
    z-=v;
    return (*this);
}

vec3& vec3::operator*=(const float v){
    x*=v;
    y*=v;
    z*=v;
    return (*this);
}

vec3& vec3::operator/=(const float v){
    x/=v;
    y/=v;
    z/=v;
    return (*this);
}

float& vec3::operator[](const int v){
    return array[v];
}

float vec3::operator[](const int v)const {
	return array[v];
}

}
