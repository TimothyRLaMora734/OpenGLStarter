#include "mat4.h"
#include "all_math.h"

#include <math.h>
#include <string.h>

namespace aRibeiro{

#define mkIndex(y, x) ((y)*4+(x))


mat4::mat4(){
    _11=_12=_13=_14=_21=_22=_23=_24=
    _31=_32=_33=_34=_41=_42=_43=_44= 0;
}
mat4::mat4(const float value){
    _11=_12=_13=_14=_21=_22=_23=_24=
    _31=_32=_33=_34=_41=_42=_43=_44= value;
}
mat4::mat4(const float a1,const float b1,const float c1,const float d1,
           const float a2,const float b2,const float c2,const float d2,
           const float a3,const float b3,const float c3,const float d3,
           const float a4,const float b4,const float c4,const float d4){
    _11=a1;_12=b1;_13=c1;_14=d1;
    _21=a2;_22=b2;_23=c2;_24=d2;
    _31=a3;_32=b3;_33=c3;_34=d3;
    _41=a4;_42=b4;_43=c4;_44=d4;
}

mat4::mat4(const mat4 &m){
    memcpy(array,m.array,sizeof(float)*4*4);
}
mat4& mat4::operator*=(const mat4 &M){
    float a,b,c,d;
    a = _11;b = _12;c = _13;d = _14;
    _11 = (a*M._11+b*M._21+c*M._31+d*M._41);
    _12 = (a*M._12+b*M._22+c*M._32+d*M._42);
    _13 = (a*M._13+b*M._23+c*M._33+d*M._43);
    _14 = (a*M._14+b*M._24+c*M._34+d*M._44);

    a = _21;b = _22;c = _23;d = _24;
    _21 = (a*M._11+b*M._21+c*M._31+d*M._41);
    _22 = (a*M._12+b*M._22+c*M._32+d*M._42);
    _23 = (a*M._13+b*M._23+c*M._33+d*M._43);
    _24 = (a*M._14+b*M._24+c*M._34+d*M._44);

    a = _31;b = _32;c = _33;d = _34;
    _31 = (a*M._11+b*M._21+c*M._31+d*M._41);
    _32 = (a*M._12+b*M._22+c*M._32+d*M._42);
    _33 = (a*M._13+b*M._23+c*M._33+d*M._43);
    _34 = (a*M._14+b*M._24+c*M._34+d*M._44);

    a = _41;b = _42;c = _43;d = _44;
    _41 = (a*M._11+b*M._21+c*M._31+d*M._41);
    _42 = (a*M._12+b*M._22+c*M._32+d*M._42);
    _43 = (a*M._13+b*M._23+c*M._33+d*M._43);
    _44 = (a*M._14+b*M._24+c*M._34+d*M._44);

    return *this;
}
mat4 mat4::operator*(const mat4 &M) const {
    return (mat4(*this)*=M);
}
float& mat4::operator()(const int x,const int y){
    return array[mkIndex(y, x)];
}
vec4& mat4::operator[](const int x){
    return *((vec4*)&array[x*4]);
}
vec4 mat4::operator[](const int v)const {
	return *((vec4*)&array[v * 4]);
}

tensor4& mat4::asTensor4()const{
    return *((tensor4*)this);
}

bool mat4::operator==(const mat4&v) const {
	return memcmp(array, v.array, sizeof(float) * 16) == 0;
}

bool mat4::operator!=(const mat4&v) const {
	return memcmp(array, v.array, sizeof(float) * 16) != 0;
}


const mat4 mat4::IdentityMatrix(1,0,0,0,
                                0,1,0,0,
                                0,0,1,0,
                                0,0,0,1);

}

