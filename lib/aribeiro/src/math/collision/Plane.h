#ifndef Plane_hpp
#define Plane_hpp

#include <aribeiro/common.h>
#include <aribeiro/vec3.h>

namespace aRibeiro {
	
	class Ray;
	class Triangle;
	class LineSegment;

	class Plane {
	public:
		vec3 normal;
		float distance;

		Plane();
		Plane(const vec3 &point, const vec3 &normal);
		Plane(const vec3 &a, const vec3 &b, const vec3 &c);

		void normalize();

		//
		// Static constructors
		//

		static Plane fromTriangle(const vec3 &a, const vec3 &b, const vec3 &c);
		static Plane fromTriangle(const Triangle &t);

		//
		// Point operations
		//
		static vec3 closestPointToPlane(vec3 q, const Plane &plane);
		static float pointDistanceToPlane(vec3 q, const Plane &plane);

		static bool raycastPlane(const Ray &r, const Plane &plane, float *outT);
		
		static bool segmentIntersectsPlane(const vec3 &a, const vec3 &b, const Plane &plane, float *outT);
		static bool segmentIntersectsPlane(const LineSegment &ls, const Plane &plane, float *outT);


		// Given planes p1 and p2, compute line L = p+t*d of their intersection.
		// Return 0 if no such line exists
		static bool intersectPlaneToPlane(const Plane &p1, const Plane &p2, vec3 *outP, vec3 *outD);
		// Compute the point p at which the three planes p1, p2 and p3 intersect (if at all)
		static bool intersectPlanes(const Plane &p1, const Plane &p2, const Plane &p3, vec3 *outP);


	};

}

#endif