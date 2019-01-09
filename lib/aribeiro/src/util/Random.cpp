
#include "Random.h"
#include "win32/drand48.h"

namespace aRibeiro {


	void Random::setSeed(long int v) {
		srand48(v);
	}

	double Random::getDouble() {
		return drand48();
	}
	float Random::getFloat() {
		return (float)drand48();
	}
	vec2 Random::getVec2() {
		return vec2((float)drand48(), (float)drand48());
	}
	vec3 Random::getVec3() {
		return vec3((float)drand48(), (float)drand48(), (float)drand48());
	}
	vec4 Random::getVec4() {
		return vec4((float)drand48(), (float)drand48(), (float)drand48(), (float)drand48());
	}
	
	mat4 Random::getMat4() {
		return mat4((float)drand48(), (float)drand48(), (float)drand48(), (float)drand48(),
			(float)drand48(), (float)drand48(), (float)drand48(), (float)drand48(),
			(float)drand48(), (float)drand48(), (float)drand48(), (float)drand48(),
			(float)drand48(), (float)drand48(), (float)drand48(), (float)drand48());
	}
	tensor4 Random::getTensor4() {
		return tensor4((float)drand48(), (float)drand48(), (float)drand48(), (float)drand48(),
			(float)drand48(), (float)drand48(), (float)drand48(), (float)drand48(),
			(float)drand48(), (float)drand48(), (float)drand48(), (float)drand48(),
			(float)drand48(), (float)drand48(), (float)drand48(), (float)drand48());
	}

	quat Random::getQuat(){
		return quatFromEuler( DEG2RAD( (float)drand48() * 360.0f ) , DEG2RAD((float)drand48() * 360.0f), DEG2RAD((float)drand48() * 360.0f));
	}

}

