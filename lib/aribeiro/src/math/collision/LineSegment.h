#ifndef LineSegment_hpp
#define LineSegment_hpp

#include <aribeiro/common.h>
#include <aribeiro/vec3.h>

namespace aRibeiro {

	class LineSegment {
	public:
		vec3 a, b;

		LineSegment();
		LineSegment(const vec3 &a, const vec3 &b);

		// Given segment ab and point c, computes closest point d on ab.
		// Also returns t for the position of d, d(t)=a+ t*(b - a)
		static vec3 closestPointToSegment(const vec3 &p, const vec3 &a, const vec3 &b);
		static vec3 closestPointToSegment(const vec3 &p, const LineSegment &ls);

	};

}

#endif