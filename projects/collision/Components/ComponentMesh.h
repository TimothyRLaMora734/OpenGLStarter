#ifndef ComponentMesh___H
#define ComponentMesh___H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Component.h"

void setTriangle(std::vector<unsigned short> *indices,
                 std::vector<vec3> *vertices,
                 std::vector<vec2> *uv = NULL,
                 std::vector<vec3> *normals = NULL,
                 std::vector<vec3> *tangents = NULL,
                 std::vector<vec3> *binormals = NULL);

void setPlane(const vec3 &dimension,
              std::vector<unsigned short> *indices,
              std::vector<vec3> *vertices,
              std::vector<vec2> *uv = NULL,
              std::vector<vec3> *normals = NULL,
              std::vector<vec3> *tangents = NULL,
              std::vector<vec3> *binormals = NULL);

void setBox(const vec3 &dimension,
            std::vector<unsigned short> *indices,
            std::vector<vec3> *vertices,
            std::vector<vec2> *uv = NULL,
            std::vector<vec3> *normals = NULL,
            std::vector<vec3> *tangents = NULL,
            std::vector<vec3> *binormals = NULL);

void setSphere(float radius, int sectorCount, int stackCount,
               std::vector<unsigned short> *indices,
               std::vector<vec3> *vertices,
               std::vector<vec2> *uv = NULL,
               std::vector<vec3> *normals = NULL,
               std::vector<vec3> *tangents = NULL,
               std::vector<vec3> *binormals = NULL
               );




class ComponentColorMesh: public Component {
public:
    
    static const ComponentType Type;
    
    std::vector<vec3> vertices;
    std::vector<unsigned short> indices;
    vec4 color;
    
    ComponentColorMesh():Component(ComponentTypeColorMesh) {
        
    }
    
    void setLayoutPointers(GLint positionLayout) {
        OPENGL_CMD(glEnableVertexAttribArray(positionLayout));
        OPENGL_CMD(glVertexAttribPointer(positionLayout, 3, GL_FLOAT, false, sizeof(vec3), &vertices[0]));
    }
    
    void draw() {
        OPENGL_CMD(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]));
    }
    
    static void unsetLayoutPointers(GLint positionLayout) {
        OPENGL_CMD(glDisableVertexAttribArray(positionLayout));
    }

	SSE2_CLASS_NEW_OPERATOR
};

class ComponentColorMeshVBO: public Component {
public:
    
    static const ComponentType Type;
    
    GLVertexBufferObject *data;
    GLVertexBufferObject *index;
    int indexCount;
    
    vec4 color;
    
    ComponentColorMeshVBO(ComponentColorMesh *src):Component(ComponentTypeColorMeshVBO){
        color = src->color;
        data = new GLVertexBufferObject();
        index = new GLVertexBufferObject();
        
        indexCount = src->indices.size();
        
        data->uploadData((void*)&src->vertices[0], sizeof(vec3)*src->vertices.size());
        index->uploadData((void*)&src->indices[0], src->indices.size()*sizeof(unsigned short));
    }
    
    ComponentColorMeshVBO(const std::vector<vec3> &vertices,
                          const std::vector<unsigned short> &indices,
                          const vec4 &color):Component(ComponentTypeColorMeshVBO){
        this->color = color;
        data = new GLVertexBufferObject();
        index = new GLVertexBufferObject();
        
        indexCount = indices.size();
        data->uploadData((void*)&vertices[0], sizeof(vec3)*vertices.size());
        index->uploadData((void*)&indices[0], indices.size()*sizeof(unsigned short));
    }
    
    ~ComponentColorMeshVBO(){
        setNullAndDelete(data);
        setNullAndDelete(index);
    }
    
    void setLayoutPointers(GLint positionLayout) {
        data->setLayout(positionLayout, 3, GL_FLOAT, sizeof(vec3), 0);
        index->setIndex();
    }
    
    void draw() {
        index->drawIndex(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT);
    }
    
    void unsetLayoutPointers(GLint positionLayout) {
        data->unsetLayout(positionLayout);
        index->unsetIndex();
    }
    
    //
    // Another constructor
    //
    static ComponentColorMeshVBO* createTriangle(const vec4 &color){
        std::vector<vec3> vertices;
        std::vector<unsigned short> indices;
        setTriangle(&indices, &vertices);
        return new ComponentColorMeshVBO(vertices, indices, color);
    }
    
    static ComponentColorMeshVBO* createPlane(const vec4 &color, const vec3 &dimension){
        std::vector<vec3> vertices;
        std::vector<unsigned short> indices;
        setPlane(dimension,&indices, &vertices);
        return new ComponentColorMeshVBO(vertices, indices, color);
    }
    
    static ComponentColorMeshVBO* createBox(const vec4 &color, const vec3 &dimension){
        std::vector<vec3> vertices;
        std::vector<unsigned short> indices;
        setBox(dimension,&indices, &vertices);
        return new ComponentColorMeshVBO(vertices, indices, color);
    }
    
    static ComponentColorMeshVBO* createSphere(const vec4 &color, float radius, int sectorCount, int stackCount){
        std::vector<vec3> vertices;
        std::vector<unsigned short> indices;
        setSphere(radius, sectorCount, stackCount, &indices, &vertices);
        return new ComponentColorMeshVBO(vertices, indices, color);
    }
    
	SSE2_CLASS_NEW_OPERATOR
};



#endif

