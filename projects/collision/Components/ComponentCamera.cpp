#include "ComponentCamera.h"
#include "../App.h"

ComponentCameraPerspective::ComponentCameraPerspective(AppBase *appBase):Component(ComponentTypeCamera) {
    app = appBase;
    
    fovDegrees = 60.0f;
    nearPlane = 0.01f;
    farPlane = 10.0f;
    
    app->WindowSize.OnChange.add(this, &ComponentCameraPerspective::OnWindowSizeChanged);
    
    //call the first setup
    OnWindowSizeChanged(&appBase->WindowSize);
    
    fovDegrees.OnChange.add(this, &ComponentCameraPerspective::OnUpdateCameraFloatParameter);
    nearPlane.OnChange.add(this, &ComponentCameraPerspective::OnUpdateCameraFloatParameter);
    farPlane.OnChange.add(this, &ComponentCameraPerspective::OnUpdateCameraFloatParameter);
}

ComponentCameraPerspective::~ComponentCameraPerspective() {
    app->WindowSize.OnChange.remove(this, &ComponentCameraPerspective::OnWindowSizeChanged);
}

void ComponentCameraPerspective::OnUpdateCameraFloatParameter(Property<float> *prop) {
    iSize size = app->WindowSize;
    projection = projection_perspective(fovDegrees, (float)size.x / (float)size.y, nearPlane, farPlane);
}

void ComponentCameraPerspective::OnWindowSizeChanged(Property<iSize> *prop) {
    iSize size = prop->value;
    projection = projection_perspective(fovDegrees, (float)size.x / (float)size.y, nearPlane, farPlane);
}

void ComponentCameraPerspective::precomputeViewProjection(bool useVisitedFlag) {
    viewProjection = projection * transform->getMatrixInverse(useVisitedFlag);
    
    view = transform->getMatrixInverse(useVisitedFlag);
    viewInv = transform->getMatrix(useVisitedFlag);
    viewIT = transpose(viewInv);
    
}
