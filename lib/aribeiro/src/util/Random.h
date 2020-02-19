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
		static quat getQuat();
        
        static vec2 getVec2Direction();
        static vec3 getVec3Direction();
        static vec4 getVec4Ptn();
        static vec4 getVec4Vec();
        static vec4 getVec4VecDirection();
        
        static int getRange(int min, int max);

	};

}

#endif
