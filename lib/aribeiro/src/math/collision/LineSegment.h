#ifndef LineSegment_hpp
#define LineSegment_hpp

#include <aribeiro/common.h>
#include <aribeiro/vec3.h>

namespace aRibeiro {

	class AABB;
	class Plane;
	class Sphere;
	class LineSegment;
	class Triangle;


	class LineSegment {
	public:
		vec3 a, b;

		LineSegment();
		LineSegment(const vec3 &a, const vec3 &b);

		// Given segment ab and point c, computes closest point d on ab.
		// Also returns t for the position of d, d(t)=a+ t*(b - a)
		static vec3 closestPointToSegment(const vec3 &p, const vec3 &a, const vec3 &b);
		static vec3 closestPointToSegment(const vec3 &p, const LineSegment &ls);

		//
		// Cloned methods from other collision classes
		//
		static bool aabbIntersectsSegment(const AABB &aabb, const vec3& p0, const vec3& p1);
		static bool aabbIntersectsSegment(const AABB &aabb, const LineSegment& ls);

		static bool planeIntersectsSegment(const Plane &plane, const vec3 &a, const vec3 &b);
		static bool planeIntersectsSegment(const Plane &plane, const LineSegment &ls);

		static bool sphereIntersectsSegment(const Sphere &sphere, const vec3& p, const vec3& q);
		static bool sphereIntersectsSegment(const Sphere &sphere, const LineSegment& ls);

		static bool triangleIntersectsSegment(
			const vec3& a, const vec3& b, const vec3& c,
			const vec3& p, const vec3& q,
			float &u, float &v, float &w);

		static bool triangleIntersectsSegment(
			const vec3& a, const vec3& b, const vec3& c,
			const LineSegment& ls,
			float &u, float &v, float &w);

		static bool triangleIntersectsSegment(
			const Triangle& t,
			const LineSegment& ls,
			float &u, float &v, float &w);

		static bool triangleIntersectsSegment(
			const Triangle& t,
			const vec3& p, const vec3& q,
			float &u, float &v, float &w);


	};

}

#endif