#include "LineSegment.h"

#include <aribeiro/all_math.h>

namespace aRibeiro {
namespace collision {
	LineSegment::LineSegment() {

	}

	LineSegment::LineSegment(const vec3 &a, const vec3 &b) {
		this->a = a;
		this->b = b;
	}

	// Given segment ab and point c, computes closest point d on ab.
	// Also returns t for the position of d, d(t)=a+ t*(b - a)
	vec3 LineSegment::closestPointToSegment(const vec3 &p, const vec3 &a, const vec3 &b)
	{
		vec3 ab = b - a;
		// Project p onto ab, computing parameterized position d(t)=a+ t*(b – a)
		float t = dot(p - a, ab) / dot(ab, ab);
		// If outside segment, clamp t (and therefore d) to the closest endpoint
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		// Compute projected position from the clamped t
		return a + t * ab;
	}

	vec3 LineSegment::closestPointToSegment(const vec3 &p, const LineSegment &ls) {
		return closestPointToSegment(p, ls.a, ls.b);
	}


	bool LineSegment::aabbIntersectsSegment(const AABB &aabb, const vec3& p0, const vec3& p1) {
		return AABB::segmentIntersectsAABB(p0, p1, aabb);
	}

	bool LineSegment::aabbIntersectsSegment(const AABB &aabb, const LineSegment& ls) {
		return AABB::segmentIntersectsAABB(ls, aabb);
	}

	bool LineSegment::planeIntersectsSegment(const Plane &plane, const vec3 &a, const vec3 &b) {
		return Plane::segmentIntersectsPlane(a, b, plane);
	}

	bool LineSegment::planeIntersectsSegment(const Plane &plane, const LineSegment &ls) {
		return Plane::segmentIntersectsPlane(ls, plane);
	}

	bool LineSegment::sphereIntersectsSegment(const Sphere &sphere, const vec3& p, const vec3& q) {
		return Sphere::segmentIntersectsSphere(p, q, sphere);
	}

	bool LineSegment::sphereIntersectsSegment(const Sphere &sphere, const LineSegment& ls) {
		return Sphere::segmentIntersectsSphere(ls, sphere);
	}

	bool LineSegment::triangleIntersectsSegment(
		const vec3& a, const vec3& b, const vec3& c,
		const vec3& p, const vec3& q,
		float &u, float &v, float &w) {
		return Triangle::segmentIntersectsTriangle(p,q,a,b,c,u,v,w);
	}

	bool LineSegment::triangleIntersectsSegment(
		const vec3& a, const vec3& b, const vec3& c,
		const LineSegment& ls,
		float &u, float &v, float &w) {
		return Triangle::segmentIntersectsTriangle(ls, a, b, c, u, v, w);
	}

	bool LineSegment::triangleIntersectsSegment(
		const Triangle& t,
		const LineSegment& ls,
		float &u, float &v, float &w) {
		return Triangle::segmentIntersectsTriangle(ls, t, u, v, w);
	}

	bool LineSegment::triangleIntersectsSegment(
		const Triangle& t,
		const vec3& p, const vec3& q,
		float &u, float &v, float &w) {
		return Triangle::segmentIntersectsTriangle(p, q, t, u, v, w);
	}

}
}
