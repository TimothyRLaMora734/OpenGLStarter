#include "LineSegment.h"

#include <aribeiro/all_math.h>

namespace aRibeiro {

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

}
