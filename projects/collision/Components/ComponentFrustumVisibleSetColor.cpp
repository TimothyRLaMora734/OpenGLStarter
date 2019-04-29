#include "ComponentFrustumVisibleSetColor.h"
#include "../Transform.h"


ComponentFrustumVisibleSetColor::ComponentFrustumVisibleSetColor() :Component(ComponentTypeNone) {

}

ComponentFrustumVisibleSetColor::~ComponentFrustumVisibleSetColor() {
	if (frustumPtr != NULL) {
		frustumPtr->OnBecomeVisible.remove(this, &ComponentFrustumVisibleSetColor::OnBecomeVisible);
		frustumPtr->OnBecomeInvisible.remove(this, &ComponentFrustumVisibleSetColor::OnBecomeInvisible);
	}
}

void ComponentFrustumVisibleSetColor::start() {
	vboPtr = (ComponentColorMeshVBO *)transform->findComponent(ComponentTypeColorMeshVBO);
	frustumPtr = (ComponentFrustumCulling *)transform->findComponent(ComponentTypeFrustumCulling);

	frustumPtr->OnBecomeVisible.add(this, &ComponentFrustumVisibleSetColor::OnBecomeVisible);
	frustumPtr->OnBecomeInvisible.add(this, &ComponentFrustumVisibleSetColor::OnBecomeInvisible);
}

void ComponentFrustumVisibleSetColor::OnBecomeVisible(Component *f, Component *camera) {
	vboPtr->color = vec4(0.5f, 0.5f, 0.0f, 1.0f);
}

void ComponentFrustumVisibleSetColor::OnBecomeInvisible(Component *f, Component *camera) {
	vboPtr->color = vec4(0.5f, 0.0f, 0.0f, 1.0f);
}
