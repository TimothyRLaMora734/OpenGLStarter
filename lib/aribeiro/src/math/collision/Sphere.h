#ifndef Sphere_hpp
#define Sphere_hpp

#include <aribeiro/common.h>
#include <aribeiro/vec3.h>

namespace aRibeiro {
	
namespace collision {
        
	class Ray;
	class LineSegment;
	class AABB;
	class Frustum;
	class Triangle;

	_SSE2_ALIGN_PRE class Sphere {
	public:

		vec3 center;
		float radius;

		Sphere();
		Sphere(const vec3 &center, float radius);
        
        static bool sphereOverlapsSphere(const Sphere& a,const Sphere& b);

		static Sphere joinSpheres(const Sphere &s0, const Sphere &s1);

		// Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
		// returns t value of intersection and intersection point q
		static bool raycastSphere(const Ray &r, const Sphere &sphere, float *outT, vec3 *outNormal);
		static bool segmentIntersectsSphere(const vec3& p, const vec3& q, const Sphere &sphere);
		static bool segmentIntersectsSphere(const LineSegment& ls, const Sphere &sphere);
		static bool pointInsideSphere(const vec3& p, const Sphere &sphere);

		//
		// Cloned methods from other collision classes
		//
		static bool aabbOverlapsSphere(const AABB& aabb, const Sphere &sphere, vec3 *penetration);
		static bool frustumOverlapsSphere(const Frustum &f, const Sphere &s);
		static bool triangleIntersectsSphere(const vec3& a, const vec3& b, const vec3& c, const Sphere& sphere, vec3 *penetration);
		static bool triangleIntersectsSphere(const Triangle& t, const Sphere& sphere, vec3 *penetration);


	};
}
    
}

#endif
