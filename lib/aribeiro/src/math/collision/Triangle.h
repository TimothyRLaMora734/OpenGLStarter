#ifndef Triangle_hpp
#define Triangle_hpp

#include <aribeiro/common.h>
#include <aribeiro/vec3.h>

namespace aRibeiro {

	class Ray;
	class Sphere;
	class LineSegment;
	class AABB;

	// this class dont hold the triangle positions, because it is thought to be used within vertex lists...
	class Triangle {

	public:
		vec3 a, b, c;

		Triangle();
		Triangle(const vec3 &a, const vec3 &b, const vec3 &c);

		static bool raycastTriangle(const Ray &ray, const vec3 &a, const vec3 &b, const vec3&c, float *outT);
		static bool raycastTriangle(const Ray &ray, const Triangle &t, float *outT);

		//returns the closest point inside the triangle relative to the parameter p
		static vec3 closestPointToTriangle(const vec3& p, const vec3& a, const vec3& b, const vec3& c);
		static vec3 closestPointToTriangle(const vec3& p, const Triangle &t);

		//Given line pq and ccw triangle abc, return whether line pierces triangle. If
		//so, also return the barycentric coordinates (u,v,w) of the intersection point
		static bool segmentIntersectsTriangle(const vec3& p, const vec3& q,
			const vec3& a, const vec3& b, const vec3& c,
			float &u, float &v, float &w);

		static bool segmentIntersectsTriangle(const LineSegment& ls,
			const vec3& a, const vec3& b, const vec3& c,
			float &u, float &v, float &w);

		static bool segmentIntersectsTriangle(const LineSegment& ls,
			const Triangle& t,
			float &u, float &v, float &w);

		static bool segmentIntersectsTriangle(const vec3& p, const vec3& q,
			const Triangle& t,
			float &u, float &v, float &w);

		// given a sphere p,radius -- returns the penetration vector of this sphere if it collides with the triangle
		//  If you are using a movable sphere, you can check the max step it can do based in its radius... 
		//     and consider the max step to look the minor triangle in the collision scene
		static bool sphereIntersectsTriangle(const Sphere& sphere, const vec3& a, const vec3& b, const vec3& c, vec3 *penetration);
		static bool sphereIntersectsTriangle(const Sphere& sphere, const Triangle& t, vec3 *penetration);

		//
		// Cloned methods from other collision classes
		//
		static bool aabbIntersectsTriangle(const AABB &box, const vec3 &v0, const vec3 &v1, const vec3 &v2);



	};

}

#endif