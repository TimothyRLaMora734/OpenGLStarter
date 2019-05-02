
#ifndef Components___H
#define Components___H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "../SharedPointer.h"

//
// Events
//
class Component;

BEGIN_DECLARE_DELEGATE(VoidEvent) CALL_PATTERN() END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(TwoComponentsEvent, Component* a, Component* b) CALL_PATTERN(a,b) END_DECLARE_DELEGATE;

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
	ComponentTypeFrustumCulling
    
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
    
    virtual void attachToTransform(Transform *t);
    virtual void detachFromTransform(Transform *t);
    
    //friend class App;
};

#endif
