#include "quat.h"
#include "all_math.h"
#include <string.h>

namespace aRibeiro{

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

}
