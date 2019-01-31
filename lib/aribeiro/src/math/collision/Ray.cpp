#include "Ray.h"

#include "../all_math.h"

namespace aRibeiro {

	Ray::Ray() {

	}

	Ray::Ray(const vec3 &origin, const vec3 &dir) {
		this->origin = origin;
		this->dir = dir;
	}


	bool Ray::raycastAABB(const Ray &r, const AABB& a, float *outTmin) {
		return AABB::raycastAABB(r, a, outTmin);
	}

	bool Ray::raycastPlane(const Ray &r, const Plane &plane, float *outT) {
		return Plane::raycastPlane(r, plane, outT);
	}

	bool Ray::raycastSphere(const Ray &r, const Sphere &sphere, float *outT) {
		return Sphere::raycastSphere(r, sphere, outT);
	}

	bool Ray::raycastTriangle(const Ray &ray, const vec3 &a, const vec3 &b, const vec3&c, float *outT) {
		return Triangle::raycastTriangle(ray, a, b, c, outT);
	}

	bool Ray::raycastTriangle(const Ray &ray, const Triangle &t, float *outT) {
		return Triangle::raycastTriangle(ray, t, outT);
	}


}

