#include "ComponentCamera.h"
#include "../App.h"

#include "ComponentLines.h"

const ComponentType ComponentCameraPerspective::Type = ComponentTypeCameraPerspective;

ComponentCameraPerspective::ComponentCameraPerspective(AppBase *appBase):Component(ComponentTypeCameraPerspective) {
    app = appBase;
    
    fovDegrees = 60.0f;
    nearPlane = 0.01f;
    farPlane = 10.0f;
    
    leftHanded = true;
    rightHanded = false;
    
    app->WindowSize.OnChange.add(this, &ComponentCameraPerspective::OnWindowSizeChanged);
    
    //call the first setup
    OnWindowSizeChanged(&appBase->WindowSize);
    
    fovDegrees.OnChange.add(this, &ComponentCameraPerspective::OnUpdateCameraFloatParameter);
    nearPlane.OnChange.add(this, &ComponentCameraPerspective::OnUpdateCameraFloatParameter);
    farPlane.OnChange.add(this, &ComponentCameraPerspective::OnUpdateCameraFloatParameter);
    
    leftHanded.OnChange.add(this, &ComponentCameraPerspective::OnLeftHandedChanged);
    rightHanded.OnChange.add(this, &ComponentCameraPerspective::OnRightHandedChanged);
}

ComponentCameraPerspective::~ComponentCameraPerspective() {
    app->WindowSize.OnChange.remove(this, &ComponentCameraPerspective::OnWindowSizeChanged);
}

void ComponentCameraPerspective::OnUpdateCameraFloatParameter(Property<float> *prop) {
    configureProjection();
}

void ComponentCameraPerspective::OnWindowSizeChanged(Property<iSize> *prop) {
    configureProjection();
}

void ComponentCameraPerspective::OnLeftHandedChanged(Property<bool> *prop) {
    rightHanded.value = ! prop->value;
    configureProjection();
}

void ComponentCameraPerspective::OnRightHandedChanged(Property<bool> *prop) {
    leftHanded.value = ! prop->value;
    configureProjection();
}

void ComponentCameraPerspective::configureProjection() {
    iSize size = app->WindowSize;
    if (rightHanded)
        projection = projection_perspective(fovDegrees, (float)size.x / (float)size.y, nearPlane, farPlane);
    else
        projection = projection_perspectiveLH(fovDegrees, (float)size.x / (float)size.y, nearPlane, farPlane);
}


void ComponentCameraPerspective::precomputeViewProjection(bool useVisitedFlag) {
    viewProjection = projection * transform->getMatrixInverse(useVisitedFlag);
    
    view = transform->getMatrixInverse(useVisitedFlag);
    viewInv = transform->getMatrix(useVisitedFlag);
    viewIT = transpose(viewInv);
    
}

void ComponentCameraPerspective::addLinesComponent() {
    ComponentLinesVBO *linesVBO = (ComponentLinesVBO *)transform->findComponent(ComponentTypeLinesVBO);
    if (linesVBO == NULL)
        linesVBO = (ComponentLinesVBO *)transform->addComponent(new ComponentLinesVBO());
    
    linesVBO->vertices.clear();
    
    collision::Frustum frustum(projection);
    
    vec3 ptnInPlane =
    (
     frustum.vertices[0]+frustum.vertices[1]+
     frustum.vertices[4]+frustum.vertices[5]
     )*0.25f;
    linesVBO->vertices.push_back(ptnInPlane);
    linesVBO->vertices.push_back(ptnInPlane + frustum.positivex.normal * 0.2f);
    
    ptnInPlane =
    (
     frustum.vertices[0]+frustum.vertices[3]+
     frustum.vertices[4]+frustum.vertices[7]
     )*0.25f;
    linesVBO->vertices.push_back(ptnInPlane);
    linesVBO->vertices.push_back(ptnInPlane + frustum.top.normal * 0.2f);
    
    ptnInPlane =
    (
     frustum.vertices[0]+frustum.vertices[1]+
     frustum.vertices[2]+frustum.vertices[3]
     )*0.25f;
    linesVBO->vertices.push_back(ptnInPlane);
    linesVBO->vertices.push_back(ptnInPlane + frustum.near.normal * 0.2f);
    
    linesVBO->vertices.push_back(frustum.vertices[0]);
    linesVBO->vertices.push_back(frustum.vertices[1]);
    
    linesVBO->vertices.push_back(frustum.vertices[1]);
    linesVBO->vertices.push_back(frustum.vertices[2]);
    
    linesVBO->vertices.push_back(frustum.vertices[2]);
    linesVBO->vertices.push_back(frustum.vertices[3]);
    
    linesVBO->vertices.push_back(frustum.vertices[3]);
    linesVBO->vertices.push_back(frustum.vertices[0]);
    
    linesVBO->vertices.push_back(frustum.vertices[0+4]);
    linesVBO->vertices.push_back(frustum.vertices[1+4]);
    
    linesVBO->vertices.push_back(frustum.vertices[1+4]);
    linesVBO->vertices.push_back(frustum.vertices[2+4]);
    
    linesVBO->vertices.push_back(frustum.vertices[2+4]);
    linesVBO->vertices.push_back(frustum.vertices[3+4]);
    
    linesVBO->vertices.push_back(frustum.vertices[3+4]);
    linesVBO->vertices.push_back(frustum.vertices[0+4]);
    
    linesVBO->vertices.push_back(frustum.vertices[0]);
    linesVBO->vertices.push_back(frustum.vertices[0+4]);
    
    linesVBO->vertices.push_back(frustum.vertices[1]);
    linesVBO->vertices.push_back(frustum.vertices[1+4]);
    
    linesVBO->vertices.push_back(frustum.vertices[2]);
    linesVBO->vertices.push_back(frustum.vertices[2+4]);
    
    linesVBO->vertices.push_back(frustum.vertices[3]);
    linesVBO->vertices.push_back(frustum.vertices[3+4]);
    
    vec3 top = (frustum.vertices[0]+frustum.vertices[3]) * 0.5f;
    top.y *= 2.0f;
    
    linesVBO->vertices.push_back(frustum.vertices[0]);
    linesVBO->vertices.push_back(top);
    
    linesVBO->vertices.push_back(frustum.vertices[3]);
    linesVBO->vertices.push_back(top);
    
    vec3 lp = transform->LocalPosition;
    quat lr = transform->LocalRotation;
    
    transform->Position = vec3(0);
    transform->Rotation = quat();
    
    mat4 & m = transform->worldToLocalMatrix();
    for(int i=0;i<linesVBO->vertices.size();i++){
        linesVBO->vertices[i] = toVec3( m * toPtn4(linesVBO->vertices[i]) );
    }
    
    transform->LocalPosition = lp;
    transform->LocalRotation = lr;
    
    linesVBO->syncVBODynamic();
    
}

const ComponentType ComponentFps::Type = ComponentTypeFps;

void ComponentFps::start(){
    camera = (ComponentCameraPerspective*)transform->findComponent(ComponentTypeCameraPerspective);
    
    euler = transform->Euler;
    
    app->OnLateUpdate.add(this, &ComponentFps::OnLateUpdate);
    app->MousePos.OnChange.add(this, &ComponentFps::OnMousePosChanged);
    app->WindowSize.OnChange.add(this, &ComponentFps::OnWindowSizeChanged);
    
    OnWindowSizeChanged(&app->WindowSize);
    OnMousePosChanged(&app->MousePos);
    
    //make the euler.x to be inside the view range
    while (euler.y < 0)
        euler.y += DEG2RAD(360.0f);
    while (euler.x > DEG2RAD(90.0f))
        euler.x += -DEG2RAD(360.0f);
    while (euler.x < -DEG2RAD(90.0f))
        euler.x += DEG2RAD(360.0f);
    euler.x = clamp(euler.x, -DEG2RAD(90.0f), DEG2RAD(90.0f));
    
    transform->Rotation = quatFromEuler(euler.x, euler.y, euler.z);
    
}

void ComponentFps::OnLateUpdate(PlatformTime *time) {
    left.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
    right.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
    up.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
    down.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));
    
    vec3 forwardVec;
    
    if (camera->rightHanded)
        forwardVec = transform->Rotation * vec3(0, 0, -1);
    else
        forwardVec = transform->Rotation * vec3(0, 0, 1);
        
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
    //force use the mousePOS as the screen center...
    app->MousePos.value = screenCenter;
}

void ComponentFps::OnMousePosChanged(Property<vec2> *prop) {
    
    vec2 pos = prop->value;
    
    vec2 delta = pos - screenCenter;
    
    if (sqrLength(delta) > 1e-4f) {
        if (camera->rightHanded) {
            euler.y = fmod(euler.y - delta.x * angleSpeed + 2.0f*PI, 2.0f*PI);
            euler.x = euler.x + delta.y * angleSpeed;
        }else{
            euler.y = fmod(euler.y + delta.x * angleSpeed + 2.0f*PI, 2.0f*PI);
            euler.x = euler.x - delta.y * angleSpeed;
        }
        
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
    
    
