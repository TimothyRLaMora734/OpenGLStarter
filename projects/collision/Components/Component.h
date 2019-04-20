
#ifndef Components___H
#define Components___H

enum ComponentType{
    ComponentTypeNone = 0,
    
    ComponentTypeAABB = 1,
    ComponentTypeCamera = 2,
    ComponentTypeLines = 3,
    ComponentTypeMesh = 4,
    ComponentTypeSphere = 5,
    ComponentTypeColorMesh = 6,
    ComponentTypeColorMeshVBO = 7,
    
    ComponentTypeFpsBehaviour = 8,
    
};

class Transform;

class Component {
private:
    Component(const Component& v);
    void operator=(const Component& v);
protected:
    ComponentType type;
    Component(ComponentType type);
public:
    Transform *transform;
    ComponentType getType() const ;
    virtual ~Component() ;
};

#endif
