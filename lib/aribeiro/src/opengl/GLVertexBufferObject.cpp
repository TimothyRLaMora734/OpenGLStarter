#include "GLVertexBufferObject.h"

#include <aribeiro/PlatformGL.h>
#include <aribeiro/common.h>

#if defined(ARIBEIRO_OS_WINDOWS)
#include <inttypes.h>
#include <sys/types.h>
#include <stdint.h>
#else
#include <sys/types.h>
#endif

namespace aRibeiro {
    
    //private copy constructores, to avoid copy...
    GLVertexBufferObject::GLVertexBufferObject(const GLVertexBufferObject& v){}
    void GLVertexBufferObject::operator=(const GLVertexBufferObject& v){}

    
    GLVertexBufferObject::GLVertexBufferObject(){
        OPENGL_CMD( glGenBuffers(1, &mVBO) );
    }
    
    GLVertexBufferObject::~GLVertexBufferObject() {
        OPENGL_CMD( glDeleteBuffers(1, &mVBO) );
    }
    
    void GLVertexBufferObject::uploadData(void* data, int sizeBytes, bool dynamic) const {
        OPENGL_CMD( glBindBuffer(GL_ARRAY_BUFFER, mVBO) );
        if (dynamic)
            OPENGL_CMD( glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, GL_DYNAMIC_DRAW) );
        else
            OPENGL_CMD( glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW) );
        OPENGL_CMD( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    }
    
    void GLVertexBufferObject::uploadIndex(void* data, int sizeBytes, bool dynamic) const {
        OPENGL_CMD( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO) );
        if (dynamic)
            OPENGL_CMD( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBytes, data, GL_DYNAMIC_DRAW) );
        else
            OPENGL_CMD( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW) );
        OPENGL_CMD( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
    }
    
    void GLVertexBufferObject::setIndex() const {
        OPENGL_CMD( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO) );
    }
    
    void GLVertexBufferObject::drawIndex(GLint primitive, int count, GLint type, int offset) {
        OPENGL_CMD( glDrawElements(primitive,// mode
                                   count,  // count
                                   type,   // type
                                   (void*)(uintptr_t)offset // element array buffer offset
                                   ) );
    }
    
    void GLVertexBufferObject::unsetIndex() {
        OPENGL_CMD( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
    }
    
    
    void GLVertexBufferObject::setLayout(int layoutIndex, int sizeCount, int type, int strideBytes, int offset) {
        OPENGL_CMD( glBindBuffer(GL_ARRAY_BUFFER, mVBO) );
        OPENGL_CMD( glVertexAttribPointer(layoutIndex, sizeCount, type, GL_FALSE, strideBytes, (void*)(uintptr_t)offset) );
        OPENGL_CMD( glEnableVertexAttribArray(layoutIndex) );
    }
    
    void GLVertexBufferObject::unsetLayout(int layoutIndex) {
        OPENGL_CMD( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        OPENGL_CMD( glDisableVertexAttribArray(layoutIndex) );
    }
    
    void GLVertexBufferObject::drawArrays(GLint primitive, int count, int offset) {
        OPENGL_CMD(glDrawArrays(primitive, offset, count));
    }
    
}


