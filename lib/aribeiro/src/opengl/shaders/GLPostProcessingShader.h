#ifndef GLPostProcessingShader__H
#define GLPostProcessingShader__H

#include <aribeiro/GLShader.h>

namespace aRibeiro {
    
    class GLPostProcessingShader: public GLShader {
    public:
        static const int aVec2Position = 0;
        static const int aVec2UV = 1;
    };
    
}

#endif
