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
	box = root->addChild(new Transform());
	box->model = CreateBox(vec3(1.0f,1.0f,1.0f));
    box->model->color = vec4(0.0f,0.5f,0.0f,1.0f);
	box->setLocalScale(vec3(0.2f,0.2f,0.2f));

	Transform* sphere = box->addChild(new Transform());
	sphere->model = CreateSphere(7, 7, 0.5f);
    sphere->model->color = vec4(0.0f,0.0f,0.5f,1.0f);
	sphere->setLocalPosition(vec3(1.0f,0.0f,0.0f));

	sphere = box->addChild(new Transform());
	sphere->model = CreateSphere(7, 7, 0.5f);
    sphere->model->color = vec4(0.5f,0.0f,0.0f,1.0f);
	sphere->setLocalPosition(vec3(-1.0f, 0.0f, 0.0f));
    
    Transform* otherNode = root->addChild(new Transform());
    otherNode->addChild(new Transform());
    otherNode = otherNode->addChild(new Transform())->addChild(new Transform());
    
    otherNode->model = CreateSphere(3, 3, 0.25f);
    otherNode->model->color = vec4(0.5f,0.0f,0.5f,1.0f);
    otherNode->setLocalPosition(vec3(0,0,-5.0f));

    Transform * plane = root->addChild(new Transform());
    plane->model = CreatePlane(vec3(4.0f,0.0f,4.0f));
    plane->model->color = vec4(0.5f,0.5f,0.5f,1.0f);
    
    smallTriangle = root->addChild(new Transform());
    smallTriangle->model = CreateTriangle();
    smallTriangle->model->color = vec4(1.0f,0.0f,0.0f,1.0f);
    smallTriangle->setLocalPosition(vec3(0.0f,0.4f,0.0f));
    smallTriangle->setLocalScale(vec3(0.2f,0.2f,0.2f));
    
    otherNode = smallTriangle->addChild(new Transform());
    otherNode->model = CreateTriangle();
    otherNode->model->color = vec4(0.0f,1.0f,0.0f,1.0f);
    otherNode->setLocalPosition(vec3(-1.0f,0.0f,0.0f));
    
    otherNode = smallTriangle->addChild(new Transform());
    otherNode->model = CreateTriangle();
    otherNode->model->color = vec4(0.0f,0.0f,1.0f,1.0f);
    otherNode->setLocalPosition(vec3(1.0f,0.0f,0.0f));
    
    //smallTriangle =
    bigTriangle = box->addChild(new Transform());
    bigTriangle->model = CreateTriangle();
    bigTriangle->model->color = vec4(0.0f,1.0f,1.0f,1.0f);
    bigTriangle->setScale(vec3(1.0f));

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
    smallTriangle->setLocalEuler(vec3(0.0f, angle_rad, angle_rad*0.1f));
    box->setLocalPosition(objectPosition);

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
	}

	std::vector<Transform*> &children = element->getChildren();
	for (int i = 0; i < children.size(); i++) {
		drawTraverseTreeDepthFirst(children[i]);
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
    
	drawTraverseTreeDepthFirst(root);
    
    printf("visited: %i\ttransform recalc: %i\tdraw recalc: %i\n",
           stat_num_visited,
           stat_num_recalculated,
           stat_draw_recalculated
           );

	TrianglesModel::unsetLayoutPointers(GLShaderColor::vPosition);

}

void App::deleteTree(Transform **element) {
	std::vector<Transform*> &children = (*element)->getChildren();
	for (int i = children.size()-1; i >=0 ; i--) {
		Transform * t = (*element)->removeChild(i);
		deleteTree(&t);
	}
	if ((*element)->model != NULL)
		setNullAndDelete((*element)->model);
	setNullAndDelete(*element);
}
