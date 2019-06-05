#include "quat.h"
#include "all_math.h"
//#include <string.h>

namespace aRibeiro{

#if defined(ARIBEIRO_SSE2)
    //const __m128 _vec4_zero_sse = _mm_set1_ps(0.0f);
    //const __m128 _vec4_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
    
    const __m128 _quat_conjugate_mask = (__m128){-0.f,-0.f,-0.f,0.f}; // -0.f = 1 << 31
    
#endif
    
    
    /*
quat::quat(){
    x = y = z = 0.0f;
    w = 1;
}
quat::quat( const float x, const float y, const float z, const float w ){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
quat::quat( const quat &v ){
    *this = v;
}
bool quat::operator==(const quat&v) const {
    for(int i=0;i<4;i++){
        if (absv(array[i]-v.array[i]) > 1e-4f)
            return false;
    }
    return true;
	//return memcmp(array, v.array, sizeof(float) * 4) == 0;
}
bool quat::operator!=(const quat&v) const {
    return !((*this) == v);
	//return memcmp(array, v.array, sizeof(float) * 4) != 0;
}

float& quat::operator[](const int v){
    return array[v];
}
     */

}
