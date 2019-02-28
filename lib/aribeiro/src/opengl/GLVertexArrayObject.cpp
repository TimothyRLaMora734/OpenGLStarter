#include "GLVertexArrayObject.h"

#include <aribeiro/PlatformGL.h>


namespace aRibeiro {
    
    
        //private copy constructores, to avoid copy...
    GLVertexArrayObject::GLVertexArrayObject(const GLVertexArrayObject& v){}
    void GLVertexArrayObject::operator=(const GLVertexArrayObject& v){}
    
    GLVertexArrayObject::GLVertexArrayObject() {
#if __APPLE__
        OPENGL_CMD( glGenVertexArraysAPPLE(1, &mVAO) );
#else
        OPENGL_CMD( glGenVertexArrays(1, &mVAO) );
#endif
    }
    
    GLVertexArrayObject::~GLVertexArrayObject() {
#if __APPLE__
        OPENGL_CMD( glDeleteVertexArraysAPPLE(1, &mVAO) );
#else
        OPENGL_CMD( glDeleteVertexArrays(1, &mVAO) );
#endif
    }
    
    void GLVertexArrayObject::enable() const {
#if __APPLE__
        OPENGL_CMD( glBindVertexArrayAPPLE(mVAO) );
#else
        OPENGL_CMD( glBindVertexArray(mVAO) );
#endif
    }
    
    void GLVertexArrayObject::disable() {
#if __APPLE__
        OPENGL_CMD( glBindVertexArrayAPPLE(0) );
#else
        OPENGL_CMD( glBindVertexArray(0) );
#endif
    }
    
    void GLVertexArrayObject::drawIndex(GLint primitive, int count, GLint type, int offset) {
        OPENGL_CMD( glDrawElements(primitive,// mode
                                   count,  // count
                                   type,   // type
                                   (void*)(uintptr_t)offset // element array buffer offset
                                   ) );
    }
    
    void GLVertexArrayObject::drawArrays(GLint primitive, int count, int offset) {
        OPENGL_CMD(glDrawArrays(primitive, offset, count));
    }
}

