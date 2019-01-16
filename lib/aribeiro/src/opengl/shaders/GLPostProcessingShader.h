#ifndef GLPostProcessingShader__H
#define GLPostProcessingShader__H

#include <aribeiro/GLShader.h>

namespace aRibeiro {
    
    class GLPostProcessingShader: public GLShader {
    public:
        int aVec2Position;
        int aVec2UV;
    };
    
}

#endif
