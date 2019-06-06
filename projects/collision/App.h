#ifndef App__H
#define App__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "util/AppBase.h"
#include "util/GLRenderState.h"
#include "util/FreeMoveCamera.h"
#include "Models.h"
#include "util/ReferenceCounter.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class App : public AppBase {
    GLRenderState *renderState;
    GLShaderColor *shaderColor;
    PlatformTime time;
    //
    // Graph Variables
    //
    Transform *root;
    //bool rotateCounterClockwise;
    //float angle_rad;
	
    //Transform* box;
    //Transform* bigTriangle;
    //Transform* smallTriangle;
    
    Transform* camera1;
    
    
    ComponentCameraPerspective *cameraPerspective;
    ReferenceCounter <Component*>comps;
    
    // Scene Graph Ops
	void drawTraverseTreeDepthFirst(Transform *element);
	void drawModelsFromTree();
	void deleteTree(Transform **element);

	void processCameraVisibilityExample(Transform *element, ComponentCameraPerspective *&camera, const collision::Frustum &frustum);

public:
    App(sf::RenderWindow *window, int w, int h);
	virtual ~App();
	virtual void draw();

	SSE2_CLASS_NEW_OPERATOR

};

#endif
