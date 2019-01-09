#include "AABB.h"
#include "all_math.h"

namespace aRibeiro{


//--------------------------------------------------------------------------
AABB::AABB(const vec3 &a,const vec3 &b){
    if (a.x<b.x){
      min_box.x = a.x;
      max_box.x = b.x;
    }else{
      max_box.x = a.x;
      min_box.x = b.x;
    }
    if (a.y<b.y){
      min_box.y = a.y;
      max_box.y = b.y;
    }else{
      max_box.y = a.y;
      min_box.y = b.y;
    }
    if (a.z<b.z){
      min_box.z = a.z;
      max_box.z = b.z;
    }else{
      max_box.z = a.z;
      min_box.z = b.z;
    }
}
//--------------------------------------------------------------------------
AABB::AABB(const vec2 &a,const vec2 &b){
    if (a.x<b.x){
      min_box.x = a.x;
      max_box.x = b.x;
    }else{
      max_box.x = a.x;
      min_box.x = b.x;
    }
    if (a.y<b.y){
      min_box.y = a.y;
      max_box.y = b.y;
    }else{
      max_box.y = a.y;
      min_box.y = b.y;
    }
    min_box.z = -1;
    max_box.z =  1;
}
//--------------------------------------------------------------------------
AABB::AABB(){
    min_box = max_box = vec3(0);
}
//--------------------------------------------------------------------------
void AABB::glDrawLines(const AABB &aabb){
    #ifdef __gl_h_
    glBegin(GL_LINES);
      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.max_box.z);
      glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.max_box.z);
      glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.max_box.z);
      glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.min_box.z);glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.max_box.z);

      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.min_box.z);
      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.max_box.z);glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.max_box.z);
      glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.max_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.max_box.z);
      glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.min_box.z);

      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.min_box.z);
      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.max_box.z);glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.max_box.z);
      glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.max_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.max_box.z);
      glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.min_box.z);
    glEnd();
    #endif
}
void AABB::glDrawQuads(const AABB &aabb){
    #ifdef __gl_h_
    glBegin(GL_QUADS);
      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.min_box.z);
      glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.min_box.z);

      glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.max_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.max_box.z);
      glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.max_box.z);glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.max_box.z);

      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.min_box.z);
      glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.max_box.z);glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.max_box.z);

      glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.min_box.z);glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.max_box.z);
      glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.max_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.min_box.z);

      glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.min_box.z);glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.min_box.z);
      glVertex3f(aabb.max_box.x,aabb.max_box.y,aabb.max_box.z);glVertex3f(aabb.max_box.x,aabb.min_box.y,aabb.max_box.z);

      glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.max_box.z);glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.max_box.z);
      glVertex3f(aabb.min_box.x,aabb.max_box.y,aabb.min_box.z);glVertex3f(aabb.min_box.x,aabb.min_box.y,aabb.min_box.z);
    glEnd();
    #endif
}
//--------------------------------------------------------------------------
bool AABB::pointInAABB(const vec2& ptn,const AABB& b){
return (ptn.x<b.max_box.x && ptn.x>b.min_box.x &&
      ptn.y<b.max_box.y && ptn.y>b.min_box.y );
}
bool AABB::pointInAABB(const vec3& ptn,const AABB& b){
return (ptn.x<b.max_box.x && ptn.x>b.min_box.x &&
      ptn.y<b.max_box.y && ptn.y>b.min_box.y &&
      ptn.z<b.max_box.z && ptn.z>b.min_box.z);
}
bool AABB::isAABBoverlaped(const AABB& a,const AABB& b){
return (
     (((a.max_box.x >= b.min_box.x) && (a.max_box.x <= b.max_box.x)) ||
      ((b.min_box.x >= a.min_box.x) && (b.min_box.x <= a.max_box.x))) ||
     (((b.max_box.x >= a.min_box.x) && (b.max_box.x <= a.max_box.x)) ||
      ((a.min_box.x >= b.min_box.x) && (a.min_box.x <= b.max_box.x)))
    ) &&(
     (((a.max_box.y >= b.min_box.y) && (a.max_box.y <= b.max_box.y)) ||
      ((b.min_box.y >= a.min_box.y) && (b.min_box.y <= a.max_box.y))) ||
     (((b.max_box.y >= a.min_box.y) && (b.max_box.y <= a.max_box.y)) ||
      ((a.min_box.y >= b.min_box.y) && (a.min_box.y <= b.max_box.y)))
    ) &&(
     (((a.max_box.z >= b.min_box.z) && (a.max_box.z <= b.max_box.z)) ||
      ((b.min_box.z >= a.min_box.z) && (b.min_box.z <= a.max_box.z))) ||
     (((b.max_box.z >= a.min_box.z) && (b.max_box.z <= a.max_box.z)) ||
      ((a.min_box.z >= b.min_box.z) && (a.min_box.z <= b.max_box.z)))
    );
}
AABB AABB::AABBjoin(const AABB& a,const AABB& b){
return AABB(minimum(a.min_box,b.min_box), maximum(a.max_box,b.max_box));
}

}

