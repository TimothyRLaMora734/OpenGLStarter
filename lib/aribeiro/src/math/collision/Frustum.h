#ifndef Frustum__H
#define Frustum__H

#include <aribeiro/common.h>

//I need a complete definition to instantiate a vec3 as class field
#include <aribeiro/mat4.h>
#include <aribeiro/vec3.h>
#include <aribeiro/Plane.h>

namespace aRibeiro {
namespace collision {
	class Sphere;
	class AABB;

	class Frustum {
		void computePlanes(const mat4& clipMatrix);
        
        float minProjections[6];
        float maxProjections[6];
        
	public:
        
        Plane positivex, negativex, bottom, top, near, far;
        vec3 vertices[8];
        
        Plane& operator[](int idx);
        const Plane& operator[](int idx)const;

		Frustum(const mat4& projection);
		Frustum(const mat4& projection, const mat4& camera);

		static bool pointInsideFrustum(const vec3 &p,const Frustum &f);
		static bool sphereOverlapsFrustum(const Sphere &s, const Frustum &f);
		static bool aabbOverlapsFrustum(const AABB &aabb, const Frustum &f);


	};
}
}


#endif
