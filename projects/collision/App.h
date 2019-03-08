#ifndef App__H
#define App__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "util/AppBase.h"
#include "util/GLRenderState.h"
#include "util/FreeMoveCamera.h"
#include "Models.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class App : public AppBase {


	FreeMoveCamera freeMoveCamera;

    // render state
    GLRenderState *renderState;
    //
    // time processor
    //
    PlatformTime time;
    //
    // default drawing shader
    //
    GLShaderColor *shaderColor;
    //
    // auxiliary matrix
    //
    //mat4 projection;
    //mat4 camera;
    TransformStack<mat4> modelHierarchy;
    //
    // input helper
    //
    PressReleaseDetector left,right,up,down;
    //
    // auxiliary variables
    //
    bool rotateCounterClockwise;
    float angle_rad;
    //vec3 cameraPosition;
    vec3 objectPosition;
    
    void processInput();
    
    void OnWindowResize(Property<iSize> *prop);
    
    void drawPrimitive(GLuint oglPrimitive, const mat4 &modelViewProjection, const vec3 vertexBuffer[], const vec4 &color, int count);

public:
    App(sf::RenderWindow *window, int w, int h);
	virtual ~App();
	virtual void draw();
};





#endif
