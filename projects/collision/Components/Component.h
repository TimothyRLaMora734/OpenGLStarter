
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
class Component;

class StartEventManager{
    std::vector<Component *> componentList;
public:
    static StartEventManager* getInstance();
    void registerNewComponent(Component *c);
    void unregisterComponent(Component *c);
    void processAllComponentsWithTransform();
};

class Component {
private:
    Component(const Component& v);
    void operator=(const Component& v);
    //bool mStartCalled;
protected:
    ComponentType type;
    Component(ComponentType type);
public:
    Transform *transform;
    ComponentType getType() const ;
    virtual ~Component();
    
    virtual void start();
    //void callStartOnce();
    
    virtual void attachToTransform(Transform *t);
    virtual void detachFromTransform(Transform *t);

	SSE2_CLASS_NEW_OPERATOR
    
    //friend class App;
};

#endif
