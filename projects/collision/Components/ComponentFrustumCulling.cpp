#include "ComponentFrustumCulling.h"
#include "../Transform.h"

const ComponentType ComponentFrustumCulling::Type = ComponentTypeFrustumCulling;

ComponentFrustumCulling::ComponentFrustumCulling():Component(ComponentTypeFrustumCulling){
	cullingShape = CullingShapeNone;
}

ComponentFrustumCulling::~ComponentFrustumCulling() {
    //precisa de evento de attach to transform e detach from transform para lidar com essas situações
    if (transform != NULL)
        transform->OnVisited.remove(this, &ComponentFrustumCulling::OnTransformVisited);
}

void ComponentFrustumCulling::computeFinalPositions(bool visitedFlag){
    if (cullingShape == CullingShapeSphere){
        mat4 &m = transform->getMatrix(visitedFlag);
        vec3 scale = transform->getScale(visitedFlag);
        
        sphere = collision::Sphere(
                                   toVec3(m * toPtn4(sphereCenter)),
                                   sphereRadius * minimum(minimum(scale.x, scale.y), scale.z)
                                   );
    } else if (cullingShape == CullingShapeAABB){
        mat4 &m = transform->getMatrix(visitedFlag);
        vec3 scale = transform->getScale(visitedFlag);
        
        vec3 center = toVec3(m * toPtn4(aabbCenter));
        vec3 dimension = aabbDimension * scale;
        
        dimension *= 0.5f;
        
        aabb = collision::AABB(center - dimension, center + dimension);
    }
}

void ComponentFrustumCulling::start() {
    computeFinalPositions(false);
}

void ComponentFrustumCulling::attachToTransform(Transform *t) {
    t->OnVisited.add(this, &ComponentFrustumCulling::OnTransformVisited);
}

void ComponentFrustumCulling::detachFromTransform(Transform *t) {
    t->OnVisited.remove(this, &ComponentFrustumCulling::OnTransformVisited);
}

void ComponentFrustumCulling::OnTransformVisited(Transform *t) {
    computeFinalPositions(true);
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

