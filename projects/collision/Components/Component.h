
#ifndef Components___H
#define Components___H

enum ComponentType{
    ComponentTypeNone = 0,
    
    ComponentTypeLines,
    ComponentTypeLinesVBO,
    ComponentTypeColorMesh,
    ComponentTypeColorMeshVBO,
    
    ComponentTypeAABB,
    ComponentTypeCameraPerspective,
    ComponentTypeMesh,
    ComponentTypeSphere,
    
    //
    // User Defined Realtime-Type
    //
    ComponentTypeFps,
    
};

class Transform;

class Component {
private:
    Component(const Component& v);
    void operator=(const Component& v);
    bool mStartCalled;
protected:
    ComponentType type;
    Component(ComponentType type);
    virtual void start();
public:
    Transform *transform;
    ComponentType getType() const ;
    virtual ~Component();
    
    void callStartOnce();
    
    //friend class App;
};

#endif
