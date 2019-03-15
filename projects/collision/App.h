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

    //
    // render state
    //
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
    // input helper
    //
    PressReleaseDetector left,right,up,down;
    //
    // auxiliary variables
    //
    FreeMoveCamera freeMoveCamera;
    Transform *root;
    bool rotateCounterClockwise;
    float angle_rad;
    vec3 objectPosition;
	
    Transform* box;
    Transform* bigTriangle;
    Transform* smallTriangle;
    
    void processInput();
    
    void OnWindowResize(Property<iSize> *prop);
    
    void drawPrimitive(GLuint oglPrimitive, const mat4 &modelViewProjection, const vec3 vertexBuffer[], const vec4 &color, int count);

	void drawTraverseTreeDepthFirst(Transform *element);

	void drawModelsFromTree();

	void deleteTree(Transform **element);

public:
    App(sf::RenderWindow *window, int w, int h);
	virtual ~App();
	virtual void draw();
};





#endif
