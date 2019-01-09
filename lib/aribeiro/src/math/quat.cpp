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
    return memcmp(array, v.array, sizeof(float)*4)==0;
}
float& quat::operator[](const int v){
    return array[v];
}

}
