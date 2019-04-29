#include "ComponentFrustumVisibleSetColor.h"
#include "../Transform.h"


ComponentFrustumVisibleSetColor::ComponentFrustumVisibleSetColor() :Component(ComponentTypeNone) {

}

ComponentFrustumVisibleSetColor::~ComponentFrustumVisibleSetColor() {

}

void ComponentFrustumVisibleSetColor::start() {
	vbo = (ComponentColorMeshVBO *)transform->findComponent(ComponentTypeColorMeshVBO);
	frustum = (ComponentFrustumCulling *)transform->findComponent(ComponentTypeFrustumCulling);

	frustum->OnBecomeVisible.add(this, &ComponentFrustumVisibleSetColor::OnBecomeVisible);
	frustum->OnBecomeInvisible.add(this, &ComponentFrustumVisibleSetColor::OnBecomeInvisible);
}

void ComponentFrustumVisibleSetColor::OnBecomeVisible(Component *f, Component *camera) {
	vbo->color = vec4(0.5f, 0.5f, 0.0f, 1.0f);
}

void ComponentFrustumVisibleSetColor::OnBecomeInvisible(Component *f, Component *camera) {
	vbo->color = vec4(0.5f, 0.0f, 0.0f, 1.0f);
}
