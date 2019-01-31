#include "Ray.h"

#include "../all_math.h"

namespace aRibeiro {

	Ray::Ray() {

	}

	Ray::Ray(const vec3 &origin, const vec3 &dir) {
		this->origin = origin;
		this->dir = dir;
	}


}

