#ifndef Frustum_Culling__H
#define Frustum_Culling__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Component.h"
//#include "ComponentCamera.h"
//class FrustumCulling;

enum CullingShape {
	CullingShapeNone,
	CullingShapeSphere,
	CullingShapeAABB
};

class ComponentFrustumCulling : public Component {

	std::map<Component*, bool> visibleFromCameras;

public:

	CullingShape cullingShape;

	//sphere shape
	vec3 sphereCenter;
	float sphereRadius;
    collision::Sphere sphere;

	//aabb
	vec3 aabbDimension;
	vec3 aabbCenter;
    collision::AABB aabb;

	static const ComponentType Type;

	ComponentFrustumCulling();
	~ComponentFrustumCulling();
    
    void computeFinalPositions(bool visitedFlag);
    
    void start();
    void attachToTransform(Transform *t);
    void detachFromTransform(Transform *t);
    
    
    void OnTransformVisited(Transform *t);

	static ComponentFrustumCulling *createShapeSphere(const vec3& sphereCenter, float sphereRadius);
	static ComponentFrustumCulling *createShapeAABB(const collision::AABB& aabb);

	//Property<bool> VisibleFromFrustum;
	//this, cameraPerspective
	TwoComponentsEvent OnBecomeVisible;
	TwoComponentsEvent OnBecomeInvisible;


	void setVisibilityFromCamera(Component* cameraPerspective, bool visibility);
	void resetVisibility();

	bool isVisibleFromCamera(Component* cameraPerspective);


//	VoidEvent OnVisibleFromFrustum;

};

#endif
