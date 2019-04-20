#include <aribeiro/aribeiro.h>
using namespace aRibeiro;
#include "App.h"

App::App(sf::RenderWindow *window, int w, int h):
	AppBase(window,w,h),
	freeMoveCamera(this)
{

    renderState = GLRenderState::getInstance();
    
    shaderColor = new GLShaderColor();

    //initialize auxiliary variables
	freeMoveCamera.position = vec3(0.0f, 0.4f, 2.0f);
    angle_rad = 0;
    rotateCounterClockwise = true;
    objectPosition = vec3(0,-1.5f,-1);
    
    //listen the resize window event
    WindowSize.OnChange.add(this, &App::OnWindowResize);
    OnWindowResize(&WindowSize);

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
    cameraTransform->addComponent(cameraPerspective);
    
    cameraPerspective->transform->LocalPosition = vec3(0.0f, 0.4f, 2.0f);
    
	box = root->addChild(new Transform());
	box->LocalScale = vec3(0.2f, 0.2f, 0.2f);
    comps.add(
      box->addComponent(ComponentColorMeshVBO::createBox(vec4(0.0f,0.5f,0.0f,1.0f), vec3(1.0f,1.0f,1.0f)))
    );
	

	Transform* sphere = box->addChild(new Transform());
	sphere->LocalPosition = vec3(1.0f, 0.0f, 0.0f);
    comps.add(
      sphere->addComponent(ComponentColorMeshVBO::createSphere(vec4(0.0f,0.0f,0.5f,1.0f), 0.5f,7,7))
    );
	

	sphere = box->addChild(new Transform());
	sphere->LocalPosition = vec3(-1.0f, 0.0f, 0.0f);
    comps.add(
              sphere->addComponent(ComponentColorMeshVBO::createSphere(vec4(0.5f,0.0f,0.0f,1.0f), 0.5f,7,7))
              );
	
    
    Transform* otherNode = root->addChild(new Transform());
    otherNode->addChild(new Transform());
    otherNode = otherNode->addChild(new Transform())->addChild(new Transform());
	otherNode->LocalPosition = vec3(0, 0, -5.0f);
    comps.add(
              otherNode->addComponent(ComponentColorMeshVBO::createSphere(vec4(0.5f,0.0f,0.5f,1.0f), 0.25f,3,3))
              );

    Transform * plane = root->addChild(new Transform());
    comps.add(
              plane->addComponent(ComponentColorMeshVBO::createPlane(vec4(0.5f,0.5f,0.5f,1.0f), vec3(4.0f,0.0f,4.0f)))
              );
    
    smallTriangle = root->addChild(new Transform());
	smallTriangle->LocalPosition = vec3(0.0f, 0.4f, 0.0f);
	smallTriangle->LocalScale = vec3(0.2f, 0.2f, 0.2f);
    comps.add(
              smallTriangle->addComponent(ComponentColorMeshVBO::createTriangle(vec4(1.0f,0.0f,0.0f,1.0f)))
              );
    
    
    otherNode = smallTriangle->addChild(new Transform());
	otherNode->LocalPosition = vec3(-1.0f, 0.0f, 0.0f);
    comps.add(
              otherNode->addComponent(ComponentColorMeshVBO::createTriangle(vec4(0.0f,1.0f,0.0f,1.0f)))
              );
    
    
    otherNode = smallTriangle->addChild(new Transform());
	otherNode->LocalPosition = vec3(1.0f, 0.0f, 0.0f);
    comps.add(
              otherNode->addComponent(ComponentColorMeshVBO::createTriangle(vec4(0.0f,0.0f,1.0f,1.0f)))
              );
    
    
    //smallTriangle =
    bigTriangle = box->addChild(new Transform());
	bigTriangle->Scale = vec3(1.0f);
    comps.add(
              bigTriangle->addComponent(ComponentColorMeshVBO::createTriangle(vec4(0.0f,1.0f,1.0f,1.0f)))
              );

    time.update();
}

App::~App(){
    setNullAndDelete(shaderColor);
	deleteTree(&root);
}

void App::OnWindowResize(Property<iSize> *prop){
    iSize size = prop->value;
    // configure the projection
    //projection = projection_perspective(60.0f, (float)size.x/(float)size.y, 0.01f, 10.0f);
}

void App::draw() {
    time.update();
	OnUpdate(&time);
    processInput();
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //
    // Update Parameters
    //
    if (rotateCounterClockwise)
        angle_rad += time.deltaTime * DEG2RAD(200.0f);
    else
        angle_rad -= time.deltaTime * DEG2RAD(200.0f) + 2.0*PI*10.0f;
    angle_rad = fmod(angle_rad, 2.0*PI*10.0f);

    objectPosition = move(objectPosition,  vec3(0,1,-1), time.deltaTime * 0.25f);
    if (sqrDistance(objectPosition,vec3(0,1,-1)) <= 1e-6f)
        objectPosition = vec3(0,-1.5f,-1);
    
    //
    // Update Nodes
    //
    
    cameraPerspective->transform->lookAt(bigTriangle);

    //smallTriangle->Rotation = quatFromEuler(0, angle_rad, angle_rad*0.1f);
    
    smallTriangle->Euler = vec3(0.0f, angle_rad, angle_rad*0.1f);
    
    //smallTriangle->getChildAt(0)->Euler = (vec3)smallTriangle->getChildAt(0)->Euler;
    
    //smallTriangle->getChildAt(0)->LocalRotation = quatFromEuler(0, angle_rad * 0.2f, 0) ;
    
    //smallTriangle->getChildAt(0)->lookAt(cameraPerspective->transform);
    //smallTriangle->getChildAt(0)->Rotation = (quat)smallTriangle->getChildAt(0)->Rotation * quatFromEuler(0, DEG2RAD(180.0f), 0);
    
    //smallTriangle->getChildAt(0)->Rotation = (quat)smallTriangle->getChildAt(0)->Rotation;
    
    //smallTriangle->getChildAt(0)->Scale = (vec3)smallTriangle->getChildAt(0)->Scale;
    
    
    
    //smallTriangle->Euler = (vec3)smallTriangle->Euler;
    //smallTriangle->Rotation = (quat)smallTriangle->Rotation;
    
    //box->LocalPosition = objectPosition;
    box->Position = objectPosition;
    
    //
    // Draw
    //
	drawModelsFromTree();

	OnLateUpdate(&time);
}

void App::processInput() {
    left.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) );
    right.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) );
    up.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) );
    down.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) );
    
    if (left.down)
        rotateCounterClockwise = true;
    if (right.down)
        rotateCounterClockwise = false;
}

void App::drawPrimitive(GLuint oglPrimitive, const mat4 &modelViewProjection, const vec3 vertexBuffer[], const vec4 &color, int count){
    
    renderState->CurrentShader = shaderColor;
    
    shaderColor->setMatrix(modelViewProjection);
    shaderColor->setColor(color);
    
    OPENGL_CMD(glEnableVertexAttribArray(GLShaderColor::vPosition));
    OPENGL_CMD(glVertexAttribPointer(GLShaderColor::vPosition, 3, GL_FLOAT, false, sizeof(vec3), &vertexBuffer[0]));
    OPENGL_CMD(glDrawArrays(oglPrimitive, 0, count));
    OPENGL_CMD(glDisableVertexAttribArray(GLShaderColor::vPosition));
}

void App::drawTraverseTreeDepthFirst(Transform *element) {
    
    bool alreadySetupMatrix = false;
    
    for(int i=0;i<element->getComponentCount();i++){
        Component* component = element->getComponentAt(i);
        if (component->getType() == ComponentTypeColorMeshVBO){
            // Setup matrix
            if (!alreadySetupMatrix){
                alreadySetupMatrix = true;
                mat4 mvp;
                mat4 mv;
                mat4 mvIT;
                mat4 mvInv;
                /*
                element->computeRenderMatrix(
                                             freeMoveCamera.getViewProjection(),
                                             freeMoveCamera.getView(),
                                             freeMoveCamera.getViewIT(),
                                             freeMoveCamera.getViewInv(),
                                             &mvp,
                                             &mv,
                                             &mvIT,
                                             &mvInv);*/
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
            {
                ComponentColorMeshVBO* componentColorMeshVBO = (ComponentColorMeshVBO*)component;
                shaderColor->setColor( componentColorMeshVBO->color );
                componentColorMeshVBO->setLayoutPointers(GLShaderColor::vPosition);
                
                componentColorMeshVBO->draw();
                
                componentColorMeshVBO->unsetLayoutPointers(GLShaderColor::vPosition);
            }
        }
    }
    
    /*
	if (element->model != NULL) {
        mat4 mvp;
        mat4 mv;
        mat4 mvIT;
        mat4 mvInv;
        element->computeRenderMatrix(
                                     freeMoveCamera.getViewProjection(),
                                     freeMoveCamera.getView(),
                                     freeMoveCamera.getViewIT(),
                                     freeMoveCamera.getViewInv(),
                                     &mvp,
                                     &mv,
                                     &mvIT,
                                     &mvInv);
        
		shaderColor->setMatrix(mvp);
		shaderColor->setColor( element->model->color );
		element->model->setLayoutPointers(GLShaderColor::vPosition);

		element->model->draw();
        
        element->model->unsetLayoutPointers(GLShaderColor::vPosition);
	}
     */
    

	for (int i = 0; i < element->getChildCount(); i++) {
		drawTraverseTreeDepthFirst(element->getChildAt(i));
	}
}

void App::drawModelsFromTree() {
    freeMoveCamera.updateBaseMatrix();
    
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
    /*
	if ((*element)->model != NULL)
		setNullAndDelete((*element)->model);
     */
    for(int i=(*element)->getComponentCount()-1;i>=0;i--){
        comps.remove((*element)->removeComponentAt(i));
    }
	setNullAndDelete(*element);
}
