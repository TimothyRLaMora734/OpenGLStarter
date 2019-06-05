#include "tensor4.h"
#include "all_math.h"

#include <string.h>

namespace aRibeiro{

    /*
tensor4::tensor4(){
    _11=_12=_13=_14=_21=_22=_23=_24=
    _31=_32=_33=_34=_41=_42=_43=_44= 0;
}
tensor4::tensor4( const float v ){
    _11=_12=_13=_14=_21=_22=_23=_24=
    _31=_32=_33=_34=_41=_42=_43=_44= v;
}
tensor4::tensor4(const float __11,const float __12,const float __13,const float __14,
                 const float __21,const float __22,const float __23,const float __24,
                 const float __31,const float __32,const float __33,const float __34,
                 const float __41,const float __42,const float __43,const float __44){
    _11=__11;_12=__12;_13=__13;_14=__14;
    _21=__21;_22=__22;_23=__23;_24=__24;
    _31=__31;_32=__32;_33=__33;_34=__34;
    _41=__41;_42=__42;_43=__43;_44=__44;
}
tensor4::tensor4(const tensor4 &m){
    memcpy(array,m.array,sizeof(float)*4*4);
}

tensor4& tensor4::operator+=(const tensor4& v){
    _11+=v._11;_12+=v._12;_13+=v._13;_14+=v._14;
    _21+=v._21;_22+=v._22;_23+=v._23;_24+=v._24;
    _31+=v._31;_32+=v._32;_33+=v._33;_34+=v._34;
    _41+=v._41;_42+=v._42;_43+=v._43;_44+=v._44;
    return *this;
}
tensor4& tensor4::operator-=(const tensor4& v){
    _11-=v._11;_12-=v._12;_13-=v._13;_14-=v._14;
    _21-=v._21;_22-=v._22;_23-=v._23;_24-=v._24;
    _31-=v._31;_32-=v._32;_33-=v._33;_34-=v._34;
    _41-=v._41;_42-=v._42;_43-=v._43;_44-=v._44;
    return *this;
}
tensor4 tensor4::operator-() const{
    return tensor4(-_11,-_21,-_31,-_41,
                   -_12,-_22,-_32,-_42,
                   -_13,-_23,-_33,-_43,
                   -_14,-_24,-_34,-_44);
}
tensor4& tensor4::operator*=(const tensor4& v){
    _11*=v._11;_12*=v._12;_13*=v._13;_14*=v._14;
    _21*=v._21;_22*=v._22;_23*=v._23;_24*=v._24;
    _31*=v._31;_32*=v._32;_33*=v._33;_34*=v._34;
    _41*=v._41;_42*=v._42;_43*=v._43;_44*=v._44;
    return *this;
}
tensor4& tensor4::operator/=(const tensor4& v){
    _11/=v._11;_12/=v._12;_13/=v._13;_14/=v._14;
    _21/=v._21;_22/=v._22;_23/=v._23;_24/=v._24;
    _31/=v._31;_32/=v._32;_33/=v._33;_34/=v._34;
    _41/=v._41;_42/=v._42;_43/=v._43;_44/=v._44;
    return *this;
}
tensor4& tensor4::operator+=(const float v){
    _11+=v;_12+=v;_13+=v;_14+=v;
    _21+=v;_22+=v;_23+=v;_24+=v;
    _31+=v;_32+=v;_33+=v;_34+=v;
    _41+=v;_42+=v;_43+=v;_44+=v;
    return *this;
}
tensor4& tensor4::operator-=(const float v){
    _11-=v;_12-=v;_13-=v;_14-=v;
    _21-=v;_22-=v;_23-=v;_24-=v;
    _31-=v;_32-=v;_33-=v;_34-=v;
    _41-=v;_42-=v;_43-=v;_44-=v;
    return *this;
}
tensor4& tensor4::operator*=(const float v){
    _11*=v;_12*=v;_13*=v;_14*=v;
    _21*=v;_22*=v;_23*=v;_24*=v;
    _31*=v;_32*=v;_33*=v;_34*=v;
    _41*=v;_42*=v;_43*=v;_44*=v;
    return *this;
}
tensor4& tensor4::operator/=(const float v){
    _11/=v;_12/=v;_13/=v;_14/=v;
    _21/=v;_22/=v;_23/=v;_24/=v;
    _31/=v;_32/=v;_33/=v;_34/=v;
    _41/=v;_42/=v;_43/=v;_44/=v;
    return *this;
}
mat4& tensor4::asMat4()const{
    return *((mat4*)this);
}

bool tensor4::operator==(const tensor4&v) const {
    for(int i=0;i<16;i++){
        if (absv(array[i]-v.array[i]) > 1e-4f)
            return false;
    }
    return true;
	//return memcmp(array, v.array, sizeof(float) * 16) == 0;
}

bool tensor4::operator!=(const tensor4&v) const {
    return !((*this) == v);
	//return memcmp(array, v.array, sizeof(float) * 16) != 0;
}
*/
}
