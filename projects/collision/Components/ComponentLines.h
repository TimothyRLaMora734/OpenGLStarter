#ifndef ComponentLines___H
#define ComponentLines___H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Component.h"


class ComponentLines: public Component {
public:
    static const ComponentType Type;
    
    std::vector<vec3> vertices;
    vec4 color;
    float width;
    
    ComponentLines(ComponentType type = ComponentTypeLines):Component(type) {
        width = 1.0f;
        color = vec4(0,0,0,1);
    }
    
    virtual ~ComponentLines() {
        
    }
    
    virtual void setLayoutPointers(GLint positionLayout) {
        if (vertices.size() == 0)
            return;
        
        OPENGL_CMD(glEnableVertexAttribArray(positionLayout));
        OPENGL_CMD(glVertexAttribPointer(positionLayout, 3, GL_FLOAT, false, sizeof(vec3), &vertices[0]));
    }
    
    virtual void draw() {
        if (vertices.size() == 0)
            return;
        
        OPENGL_CMD(glDrawArrays(GL_LINES, 0, vertices.size()));
    }
    
    virtual void unsetLayoutPointers(GLint positionLayout) {
        if (vertices.size() == 0)
            return;
        
        OPENGL_CMD(glDisableVertexAttribArray(positionLayout));
    }
};

class ComponentLinesVBO: public ComponentLines {
public:
    static const ComponentType Type;
    
    GLVertexBufferObject *data;
    int uploadedCount;
    
    ComponentLinesVBO():ComponentLines(ComponentTypeLinesVBO){
        data = new GLVertexBufferObject();
        
        uploadedCount = 0;
    }
    
    void syncVBOStatic() {
        data->uploadData((void*)&vertices[0], sizeof(vec3)*vertices.size());
        uploadedCount = vertices.size();
    }
    
    void syncVBODynamic() {
        data->uploadData((void*)&vertices[0], sizeof(vec3)*vertices.size(), true);
        uploadedCount = vertices.size();
    }
    
    virtual ~ComponentLinesVBO(){
        setNullAndDelete(data);
    }
    
    virtual void setLayoutPointers(GLint positionLayout) {
        if (uploadedCount != 0)
            data->setLayout(positionLayout, 3, GL_FLOAT, sizeof(vec3), 0);
        else
            ComponentLines::setLayoutPointers(positionLayout);
    }
    
    virtual void draw() {
        if (uploadedCount != 0)
            data->drawArrays(GL_LINES,uploadedCount);
        else
            ComponentLines::draw();
    }
    
    virtual void unsetLayoutPointers(GLint positionLayout) {
        if (uploadedCount != 0)
            data->unsetLayout(positionLayout);
        else
            ComponentLines::unsetLayoutPointers(positionLayout);
    }
};



#endif


