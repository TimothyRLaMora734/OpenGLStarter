#ifndef VertexArrayObject__H
#define VertexArrayObject__H

#include <glew/glew.h>

namespace aRibeiro {
    
    class GLVertexArrayObject {
        //private copy constructores, to avoid copy...
        GLVertexArrayObject(const GLVertexArrayObject& v);
        void operator=(const GLVertexArrayObject& v);
        
        GLuint mVAO;
        
    public:
        
        GLVertexArrayObject();
        
        virtual ~GLVertexArrayObject();
        
        void enable() const ;
        static void disable();
        static void drawIndex(GLint primitive, int count, GLint type, int offset = 0);
        static void drawArrays(GLint primitive, int count, int offset = 0);
        
    };
    
}

#endif
