#include "ComponentCamera.h"
#include "../App.h"

const ComponentType ComponentCameraPerspective::Type = ComponentTypeCamera;

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

const ComponentType ComponentFps::Type = ComponentTypeFps;

void ComponentFps::start(){
    app->OnLateUpdate.add(this, &ComponentFps::OnLateUpdate);
    app->MousePos.OnChange.add(this, &ComponentFps::OnMousePosChanged);
    app->WindowSize.OnChange.add(this, &ComponentFps::OnWindowSizeChanged);
    
    OnWindowSizeChanged(&app->WindowSize);
}

void ComponentFps::OnLateUpdate(PlatformTime *time) {
    left.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
    right.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
    up.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
    down.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));
    
    vec3 forwardVec = transform->Rotation * vec3(0, 0, -1);
    vec3 rightVec = transform->Rotation * vec3(1, 0, 0);

    if (left.pressed)
        transform->Position = transform->Position - rightVec * strafeSpeed * time->deltaTime;
    if (right.pressed)
        transform->Position = transform->Position + rightVec * strafeSpeed * time->deltaTime;
    if (up.pressed)
        transform->Position = transform->Position + forwardVec * strafeSpeed * time->deltaTime;
    if (down.pressed)
        transform->Position = transform->Position - forwardVec * strafeSpeed * time->deltaTime;
}

void ComponentFps::OnWindowSizeChanged(Property<iSize> *prop) {
    iSize size = prop->value;
    iSizeCenter.x = size.x / 2;
    iSizeCenter.y = size.y / 2;
    
    screenCenter.x = iSizeCenter.x;
    screenCenter.y = size.y - 1 - iSizeCenter.y;// iSizeCenter.y;
    
    sf::Mouse::setPosition(iSizeCenter, *app->window);
}

void ComponentFps::OnMousePosChanged(Property<vec2> *prop) {
    vec2 pos = prop->value;
    
    vec2 delta = pos - screenCenter;
    
    if (sqrLength(delta) > 1e-4f) {
        
        euler.y = fmod(euler.y - delta.x * angleSpeed + 2.0f*PI, 2.0f*PI);
        euler.x = euler.x + delta.y * angleSpeed;
        euler.x = clamp(euler.x, -DEG2RAD(90.0f), DEG2RAD(90.0f));
        
        transform->Rotation = quatFromEuler(euler.x, euler.y, euler.z);
        
        sf::Mouse::setPosition(iSizeCenter, *app->window);
    }
}


ComponentFps::ComponentFps(AppBase *app):Component(ComponentTypeFps){
    this->app = app;
    
    forwardSpeed = 1.0f;
    strafeSpeed = 1.0f;
    angleSpeed = DEG2RAD(0.10f);
    
}

ComponentFps::~ComponentFps(){
    app->OnLateUpdate.remove(this, &ComponentFps::OnLateUpdate);
    app->MousePos.OnChange.remove(this, &ComponentFps::OnMousePosChanged);
    app->WindowSize.OnChange.remove(this, &ComponentFps::OnWindowSizeChanged);
}
    
    
