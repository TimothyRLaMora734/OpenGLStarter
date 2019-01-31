#ifndef Sphere_hpp
#define Sphere_hpp

#include <aribeiro/common.h>
#include <aribeiro/vec3.h>

namespace aRibeiro {
	
	class Ray;
	class LineSegment;

	class Sphere {
	public:

		vec3 center;
		float radius;

		Sphere();
		Sphere(const vec3 &center, float radius);

		static Sphere joinSpheres(const Sphere &s0, const Sphere &s1);

		// Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
		// returns t value of intersection and intersection point q
		static bool raycastSphere(const Ray &r, const Sphere &sphere, float *outT);
		static bool segmentIntersectsSphere(const vec3& p, const vec3& q, const Sphere &sphere);
		static bool segmentIntersectsSphere(const LineSegment& ls, const Sphere &sphere);
		static bool pointInsideSphere(const vec3& p, const Sphere &sphere);

	};
}

#endif
