#include <aribeiro/aribeiro.h>
using namespace aRibeiro;
#include "App.h"

App::App(sf::RenderWindow *window, int w, int h):
	AppBase(window,w,h)
{
    renderState = GLRenderState::getInstance();
    shaderColor = new GLShaderColor();

    //initialize auxiliary variables
    angle_rad = 0;
    rotateCounterClockwise = true;

    //setup renderstate
    renderState->ClearColor = vec4(1.0f,1.0f,250.0f/255.0f,1.0f);
    renderState->Wireframe = WireframeBack;
	renderState->CullFace = CullFaceNone;
	//renderState->Wireframe = WireframeBoth;
	//renderState->CullFace = CullFaceBack;
    
    //
    // Construct scene graph
    //
    
	root = new Transform();

    cameraPerspective = new ComponentCameraPerspective(this);
    
    Transform* cameraTransform = root->addChild(new Transform());
    comps.add(cameraTransform->addComponent(cameraPerspective));
    comps.add(cameraTransform->addComponent(new ComponentFps(this)));
    
    cameraPerspective->transform->LocalPosition = vec3(0.0f, 0.4f, 2.0f);
    
	box = root->addChild(new Transform());
	box->LocalScale = vec3(0.2f, 0.2f, 0.2f);
    box->Position = vec3(0,-1.5f,-1);
    comps.add(box->addComponent(ComponentColorMeshVBO::createBox(vec4(0.0f,0.5f,0.0f,1.0f), vec3(1.0f,1.0f,1.0f))));
	

	Transform* sphere = box->addChild(new Transform());
	sphere->LocalPosition = vec3(1.0f, 0.0f, 0.0f);
    comps.add(sphere->addComponent(ComponentColorMeshVBO::createSphere(vec4(0.0f,0.0f,0.5f,1.0f), 0.5f,7,7)));
	
	sphere = box->addChild(new Transform());
	sphere->LocalPosition = vec3(-1.0f, 0.0f, 0.0f);
    comps.add(sphere->addComponent(ComponentColorMeshVBO::createSphere(vec4(0.5f,0.0f,0.0f,1.0f), 0.5f,7,7)));
	
    Transform* otherNode = root->addChild(new Transform());
    otherNode->addChild(new Transform());
    otherNode = otherNode->addChild(new Transform())->addChild(new Transform());
	otherNode->LocalPosition = vec3(0, 0, -5.0f);
    comps.add(otherNode->addComponent(ComponentColorMeshVBO::createSphere(vec4(0.5f,0.0f,0.5f,1.0f), 0.25f,3,3)));

    Transform * plane = root->addChild(new Transform());
    comps.add(plane->addComponent(ComponentColorMeshVBO::createPlane(vec4(0.5f,0.5f,0.5f,1.0f), vec3(4.0f,0.0f,4.0f))));
    
    smallTriangle = root->addChild(new Transform());
	smallTriangle->LocalPosition = vec3(0.0f, 0.4f, 0.0f);
	smallTriangle->LocalScale = vec3(0.2f, 0.2f, 0.2f);
    comps.add(smallTriangle->addComponent(ComponentColorMeshVBO::createTriangle(vec4(1.0f,0.0f,0.0f,1.0f))));
    
    otherNode = smallTriangle->addChild(new Transform());
	otherNode->LocalPosition = vec3(-1.0f, 0.0f, 0.0f);
    comps.add(otherNode->addComponent(ComponentColorMeshVBO::createTriangle(vec4(0.0f,1.0f,0.0f,1.0f))));
    
    otherNode = smallTriangle->addChild(new Transform());
	otherNode->LocalPosition = vec3(1.0f, 0.0f, 0.0f);
    comps.add(otherNode->addComponent(ComponentColorMeshVBO::createTriangle(vec4(0.0f,0.0f,1.0f,1.0f))));
    
    //smallTriangle =
    bigTriangle = box->addChild(new Transform());
	bigTriangle->Scale = vec3(1.0f);
    comps.add(bigTriangle->addComponent(ComponentColorMeshVBO::createTriangle(vec4(0.0f,1.0f,1.0f,1.0f))));

    time.update();
}

App::~App(){
    setNullAndDelete(shaderColor);
	deleteTree(&root);
}

void App::draw() {
    time.update();
    
    //set min delta time (the passed time or the time to render at 24fps)
    time.deltaTime = minimum(time.deltaTime,1.0f/24.0f);
    
    printf("%f\n",1.0f/time.deltaTime);
    
	OnUpdate(&time);
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //
    // Update Parameters
    //
    if (rotateCounterClockwise)
        angle_rad += time.deltaTime * DEG2RAD(200.0f);
    else
        angle_rad -= time.deltaTime * DEG2RAD(200.0f) + 2.0*PI*10.0f;
    angle_rad = fmod(angle_rad, 2.0*PI*10.0f);

    box->Position = move(box->Position,  vec3(0,1,-1), time.deltaTime * 0.25f);
    if (box->Position == vec3(0,1,-1))
        box->Position = vec3(0,-1.5f,-1);
    
    //
    // Update Nodes
    //
    
    //cameraPerspective->transform->lookAt(bigTriangle);
    smallTriangle->Euler = vec3(0.0f, angle_rad, angle_rad*0.1f);
    
    
    OnLateUpdate(&time);
    
    //
    // Draw
    //
	drawModelsFromTree();
}

void App::drawTraverseTreeDepthFirst(Transform *element) {
    
    bool alreadySetupMatrix = false;
    
    for(int i=0;i<element->getComponentCount();i++){
        Component* component = element->getComponentAt(i);
        //check start called from component
        component->callStartOnce();
        
        // First Setup Transform Matrix
        if (component->getType() == ComponentTypeColorMeshVBO
            &&
            !alreadySetupMatrix){
            alreadySetupMatrix = true;
            mat4 mvp;
            mat4 mv;
            mat4 mvIT;
            mat4 mvInv;
            element->computeRenderMatrix(cameraPerspective->viewProjection,
                                         cameraPerspective->view,
                                         cameraPerspective->viewIT,
                                         cameraPerspective->viewInv,
                                         &mvp,
                                         &mv,
                                         &mvIT,
                                         &mvInv);
            
            shaderColor->setMatrix(mvp);
        }
        
        
        // render object
        if (component->getType() == ComponentTypeColorMeshVBO){
            ComponentColorMeshVBO* componentColorMeshVBO = (ComponentColorMeshVBO*)component;
            shaderColor->setColor( componentColorMeshVBO->color );
            componentColorMeshVBO->setLayoutPointers(GLShaderColor::vPosition);
            componentColorMeshVBO->draw();
            componentColorMeshVBO->unsetLayoutPointers(GLShaderColor::vPosition);
        }
    }
    
	for (int i = 0; i < element->getChildCount(); i++)
		drawTraverseTreeDepthFirst(element->getChildAt(i));
}

void App::drawModelsFromTree() {
	renderState->CurrentShader = shaderColor;
    
    stat_num_visited = 0;
    stat_num_recalculated = 0;
    stat_draw_recalculated = 0;
    
    root->resetVisited();
    root->preComputeTransforms();
    
    cameraPerspective->precomputeViewProjection(true);
    
	drawTraverseTreeDepthFirst(root);
    
    printf("visited: %i\ttransform recalc: %i\tdraw recalc: %i\n",
           stat_num_visited,
           stat_num_recalculated,
           stat_draw_recalculated
           );

	//TrianglesModel::unsetLayoutPointers(GLShaderColor::vPosition);

}

void App::deleteTree(Transform **element) {
	for (int i = (*element)->getChildCount()-1; i >=0 ; i--) {
		Transform * t = (*element)->removeChild(i);
		deleteTree(&t);
	}
    for(int i=(*element)->getComponentCount()-1;i>=0;i--){
        comps.remove((*element)->removeComponentAt(i));
    }
	setNullAndDelete(*element);
}
