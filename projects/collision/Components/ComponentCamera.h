#ifndef component_camera__H
#define component_camera__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Component.h"
#include "../util/FreeMoveCamera.h"

class AppBase;

class ComponentCameraPerspective:public Component {
    AppBase *app;
                                    
    void OnUpdateCameraFloatParameter(Property<float> *prop);
    void OnWindowSizeChanged(Property<iSize> *prop);
public:
    Property<float> fovDegrees;
    Property<float> nearPlane;
    Property<float> farPlane;
    
    mat4 projection;
    
    ComponentCameraPerspective(AppBase *appBase);
    ~ComponentCameraPerspective();
    
    void precomputeViewProjection(bool useVisitedFlag);
    
    mat4 viewProjection;
    mat4 view;
    mat4 viewIT;
    mat4 viewInv;
};


class ComponentFps:public Component {
    AppBase *app;
public:
    
};


#endif
