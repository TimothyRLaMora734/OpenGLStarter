#ifndef Ray_h
#define Ray_h

#include <aribeiro/common.h>

//I need a complete definition to instantiate a vec3 as class field
#include <aribeiro/vec3.h>

namespace aRibeiro {
namespace collision {
	class AABB;
	class Plane;
	class Sphere;
	class Triangle;

	class Ray {
	public:
		vec3 origin;
		vec3 dir;

		Ray();
		Ray(const vec3 &origin, const vec3 &dir);

		//
		// Cloned methods from other collision classes
		//
		static bool raycastAABB(const Ray &r, const AABB& a, float *outTmin, vec3 *outNormal);
		static bool raycastPlane(const Ray &r, const Plane &plane, float *outT, vec3 *outNormal);
		static bool raycastSphere(const Ray &r, const Sphere &sphere, float *outT, vec3 *outNormal);
		static bool raycastTriangle(const Ray &ray, const vec3 &a, const vec3 &b, const vec3&c, float *outT, vec3 *outNormal);
		static bool raycastTriangle(const Ray &ray, const Triangle &t, float *outT, vec3 *outNormal);

	};

}
}


#endif
