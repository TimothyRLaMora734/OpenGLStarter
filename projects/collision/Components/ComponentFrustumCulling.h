#ifndef Frustum_Culling__H
#define Frustum_Culling__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Component.h"
//#include "ComponentCamera.h"
//class FrustumCulling;

enum CullingShape {
	CullingShapeNone,
	CullingShapeSphere
};

class ComponentFrustumCulling : public Component {

	std::map<Component*, bool> visibleFromCameras;

public:

	CullingShape cullingShape;
	vec3 sphereCenter;
	float sphereRadius;

	static const ComponentType Type;

	ComponentFrustumCulling();
	~ComponentFrustumCulling();

	static ComponentFrustumCulling *createShapeSphere(const vec3& sphereCenter, float sphereRadius);

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
