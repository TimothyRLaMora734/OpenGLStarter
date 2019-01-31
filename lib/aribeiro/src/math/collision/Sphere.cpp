#include "Sphere.h"

#include <aribeiro/all_math.h>

namespace aRibeiro {


	Sphere::Sphere() {
		radius = 1.0f;
	}

	Sphere::Sphere(const vec3 &center, float radius) {
		this->center = center;
		this->radius = radius;
	}

	Sphere Sphere::joinSpheres(const Sphere &s0, const Sphere &s1)
	{
		Sphere s;
		// Compute the squared distance between the sphere centers
		vec3 d = s1.center - s0.center;
		float dist2 = dot(d, d);
		float sqr_r = (s1.radius - s0.radius);
		sqr_r *= sqr_r;
		if (sqr_r >= dist2) {
			// The sphere with the larger radius encloses the other;
			// just set s to be the larger of the two spheres
			if (s1.radius >= s0.radius)
				s = s1;
			else
				s = s0;
		}
		else {
			// Spheres partially overlapping or disjoint
			float dist = sqrt(dist2);
			s.radius = (dist + s0.radius + s1.radius) * 0.5f;
			s.center = s0.center;
			if (dist > EPSILON)
				s.center += ((s.radius - s0.radius) / dist) * d;
		}
		return s;
	}

	// Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
	// returns t value of intersection and intersection point q
	bool Sphere::raycastSphere(const Ray &ray, const Sphere &sphere, float *outT)
	{
		vec3 m = ray.origin - sphere.center;
		float b = dot(m, ray.dir);
		float c = dot(m, m) - sphere.radius * sphere.radius;
		// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
		if (c > 0.0f && b > 0.0f)
			return false;
		float discr = b * b - c;
		// A negative discriminant corresponds to ray missing sphere
		if (discr < 0.0f)
			return false;
		// Ray now found to intersect sphere, compute smallest t value of intersection
		float t = -b - sqrt(discr);
		// If t is negative, ray started inside sphere so clamp t to zero
		if (t < 0.0f)
			t = 0.0f;
		//q = p + t * d;
		*outT = t;
		return true;
	}

	bool Sphere::segmentIntersectsSphere(const vec3& p, const vec3& q, const Sphere &sphere) {
		float t;
		vec3 pq = q - p;
		float lengthPQ = dot(pq, pq);
		const float TOLERANCE = 0.001f;
		// check if can normalize segment
		if (fabs(lengthPQ) > TOLERANCE && fabs(lengthPQ - 1.0f) > TOLERANCE)
			pq *= 1.0f / lengthPQ;
		else {
			//test the point inside the sphere
			return pointInsideSphere(p, sphere);
		}

		if (raycastSphere( Ray(p, pq), sphere, &t )) {
			return (t < lengthPQ);
		}
		return false;
	}

	bool Sphere::segmentIntersectsSphere(const LineSegment& ls, const Sphere &sphere) {
		return segmentIntersectsSphere(ls.a,ls.b, sphere);
	}

	bool Sphere::pointInsideSphere(const vec3& p, const Sphere &sphere) {
		vec3 p_sc = sphere.center - p;
		float sqrDst = dot(p_sc, p_sc);
		return sqrDst <= (sphere.radius * sphere.radius);
	}


	bool Sphere::aabbOverlapsSphere(const AABB& aabb, const Sphere &sphere, vec3 *penetration) {
		bool result = AABB::sphereOverlapsAABB(sphere, aabb, penetration);
		if (result)
			*penetration = -(*penetration);
		return result;
	}

	bool Sphere::frustumOverlapsSphere(const Frustum &f, const Sphere &s) {
		return Frustum::sphereOverlapsFrustum(s, f);
	}

	bool Sphere::triangleIntersectsSphere(const vec3& a, const vec3& b, const vec3& c, const Sphere& sphere, vec3 *penetration) {
		bool result = Triangle::sphereIntersectsTriangle(sphere, a, b, c, penetration);
		if (result)
			*penetration = -(*penetration);
		return result;
	}

	bool Sphere::triangleIntersectsSphere(const Triangle& t, const Sphere& sphere, vec3 *penetration) {
		bool result = Triangle::sphereIntersectsTriangle(sphere, t, penetration);
		if (result)
			*penetration = -(*penetration);
		return result;
	}

}
