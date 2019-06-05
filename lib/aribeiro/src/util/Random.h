#ifndef Random_h__
#define Random_h__

#include <aribeiro/all_math.h>

namespace aRibeiro {

	class Random {

        static bool initialized;
        
	public:

		static void setSeed(long int v);

		static double getDouble();
		static float getFloat();
		static vec2 getVec2();
		static vec3 getVec3();
		static vec4 getVec4();
		static mat4 getMat4();
		//static tensor4 getTensor4();
		static quat getQuat();

	};

}

#endif
