#ifndef component_camera__H
#define component_camera__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Component.h"

#include "../util/Size.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class AppBase;

class ComponentCameraPerspective:public Component {
    AppBase *app;
                                    
    void OnUpdateCameraFloatParameter(Property<float> *prop);
    void OnWindowSizeChanged(Property<iSize> *prop);
    
    void OnLeftHandedChanged(Property<bool> *prop);
    void OnRightHandedChanged(Property<bool> *prop);
    
    void configureProjection();
public:
    
    static const ComponentType Type;
    
    Property<float> fovDegrees;
    Property<float> nearPlane;
    Property<float> farPlane;
    
    Property<bool> leftHanded;
    Property<bool> rightHanded;
    
    mat4 projection;
    
    ComponentCameraPerspective(AppBase *appBase);
    ~ComponentCameraPerspective();
    
    void precomputeViewProjection(bool useVisitedFlag);
    
    mat4 viewProjection;
    mat4 view;
    mat4 viewIT;
    mat4 viewInv;
    
    void addLinesComponent();
};


class ComponentFps:public Component {
    AppBase *app;
    ComponentCameraPerspective *camera;
    
    PressReleaseDetector left, right, up, down;
    sf::Vector2i iSizeCenter;
    vec2 screenCenter;
    
    vec3 euler;
    
protected:
    void start();
    void OnLateUpdate(PlatformTime *time);
    void OnWindowSizeChanged(Property<iSize> *prop);
    void OnMousePosChanged(Property<vec2> *prop);
public:
    
    float forwardSpeed;
    float strafeSpeed;
    float angleSpeed;
    
    static const ComponentType Type;
    ComponentFps(AppBase *app);
    ~ComponentFps();
};


#endif
