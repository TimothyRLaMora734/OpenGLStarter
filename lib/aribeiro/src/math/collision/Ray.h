#ifndef Ray_h
#define Ray_h

#include <aribeiro/common.h>

//I need a complete definition to instantiate a vec3 as class field
#include <aribeiro/vec3.h>

namespace aRibeiro {

	class Ray {
	public:
		vec3 origin;
		vec3 dir;

		Ray();
		Ray(const vec3 &origin, const vec3 &dir);

	};

}


#endif