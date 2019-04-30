#include "ComponentFrustumCulling.h"

const ComponentType ComponentFrustumCulling::Type = ComponentTypeFrustumCulling;

ComponentFrustumCulling::ComponentFrustumCulling():Component(ComponentTypeFrustumCulling){
	cullingShape = CullingShapeNone;
}

ComponentFrustumCulling::~ComponentFrustumCulling() {

}

ComponentFrustumCulling *ComponentFrustumCulling::createShapeSphere(const vec3& sphereCenter, float sphereRadius) {
	ComponentFrustumCulling *result = new ComponentFrustumCulling();

	result->cullingShape = CullingShapeSphere;
	result->sphereCenter = sphereCenter;
	result->sphereRadius = sphereRadius;

	return result;
}

ComponentFrustumCulling *ComponentFrustumCulling::createShapeAABB(const collision::AABB& aabb) {
	ComponentFrustumCulling *result = new ComponentFrustumCulling();

	result->cullingShape = CullingShapeAABB;
	result->aabbCenter = (aabb.min_box + aabb.max_box) * 0.5f;
	result->aabbDimension = aabb.max_box - aabb.min_box;

	return result;
}

void ComponentFrustumCulling::setVisibilityFromCamera(Component* cameraPerspective, bool visibility) {
	std::map<Component*, bool>::iterator it;
	it = visibleFromCameras.find(cameraPerspective);

	if (visibility) {
		if (it == visibleFromCameras.end()) {
			visibleFromCameras[cameraPerspective] = true;
			OnBecomeVisible(this, cameraPerspective);
		}
	}
	else {
		if (it != visibleFromCameras.end()) {
			visibleFromCameras.erase(it);
			OnBecomeInvisible(this, cameraPerspective);
		}
	}
}

void ComponentFrustumCulling::resetVisibility() {
	std::map<Component*, bool>::iterator it,aux;
	for (it = visibleFromCameras.begin(); it != visibleFromCameras.end(); it++) {
		Component* cameraComponent = it->first;
		aux = it;
		visibleFromCameras.erase(aux);
		//visibleFromCameras.erase(it++);
		OnBecomeInvisible(this, cameraComponent);
	}
	//visibleFromCameras.clear();
}

bool ComponentFrustumCulling::isVisibleFromCamera(Component* cameraPerspective) {
	std::map<Component*, bool>::iterator it;
	it = visibleFromCameras.find(cameraPerspective);
	return it != visibleFromCameras.end();
}

