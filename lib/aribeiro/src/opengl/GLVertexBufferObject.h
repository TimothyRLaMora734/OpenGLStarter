#ifndef GLVertexBufferObject__H
#define GLVertexBufferObject__H

#include <glew/glew.h>

namespace aRibeiro {

    class GLVertexBufferObject {
        //private copy constructores, to avoid copy...
        GLVertexBufferObject(const GLVertexBufferObject& v);
        void operator=(const GLVertexBufferObject& v);
        
        GLuint mVBO;
    public:
        
        GLVertexBufferObject();
        
        virtual ~GLVertexBufferObject();
        
        void uploadData(void* data, int sizeBytes, bool dynamic = false) const ;
        
        void uploadIndex(void* data, int sizeBytes, bool dynamic = false) const ;
        
        void setIndex() const ;
        
        static void drawIndex(GLint primitive, int count, GLint type, int offset = 0) ;
        
        static void unsetIndex() ;
        
        void setLayout(int layoutIndex, int sizeCount, int type, int strideBytes, int offset);
        
        static void unsetLayout(int layoutIndex);
        
        static void drawArrays(GLint primitive, int count, int offset = 0);
    };
}

#endif

