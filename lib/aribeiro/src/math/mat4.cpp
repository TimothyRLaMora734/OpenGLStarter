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
mat4::mat4(const float __11,const float __12,const float __13,const float __14,
           const float __21,const float __22,const float __23,const float __24,
           const float __31,const float __32,const float __33,const float __34,
           const float __41,const float __42,const float __43,const float __44){
    _11=__11;_12=__12;_13=__13;_14=__14;
    _21=__21;_22=__22;_23=__23;_24=__24;
    _31=__31;_32=__32;_33=__33;_34=__34;
    _41=__41;_42=__42;_43=__43;_44=__44;
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
tensor4& mat4::asTensor4()const{
    return *((tensor4*)this);
}


const mat4 mat4::IdentityMatrix(1,0,0,0,
                                0,1,0,0,
                                0,0,1,0,
                                0,0,0,1);

}

