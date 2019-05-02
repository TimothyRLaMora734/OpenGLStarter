#include <aribeiro/aribeiro.h>
using namespace aRibeiro;
#include "App.h"



Transform* loadSceneTestCollision()
{
    Transform* _0 = new Transform();
    _0->Name = std::string("TestCollision");
    _0->LocalPosition = vec3(0,0,0);
    _0->LocalRotation = quat(0,0,0,1);
    _0->LocalScale = vec3(1,1,1);
    {
        Transform* _1 = _0->addChild(new Transform());
        _1->Name = std::string("Main Camera");
        _1->LocalPosition = vec3(0,1.84,-2.12);
        _1->LocalRotation = quat(0.3571975,0,0,0.9340289);
        _1->LocalScale = vec3(1,1,1);
    }
    {
        Transform* _2 = _0->addChild(new Transform());
        _2->Name = std::string("Camera1");
        _2->LocalPosition = vec3(0,0,0);
        _2->LocalRotation = quat(0,0,0,1);
        _2->LocalScale = vec3(1,1,1);
        {
            Transform* _3 = _2->addChild(new Transform());
            _3->Name = std::string("CubeCollision");
            _3->LocalPosition = vec3(-1.5,0,0);
            _3->LocalRotation = quat(0.2044709,-0.6768985,0.204471,0.6768985);
            _3->LocalScale = vec3(0.32,0.32,1);
        }
        {
            Transform* _4 = _2->addChild(new Transform());
            _4->Name = std::string("SphereCollision");
            _4->LocalPosition = vec3(1.5,0,0);
            _4->LocalRotation = quat(0,-0.7071068,0,0.7071068);
            _4->LocalScale = vec3(1,1,1);
        }
    }
    {
        Transform* _5 = _0->addChild(new Transform());
        _5->Name = std::string("Cube");
        _5->LocalPosition = vec3(0,0,1.5);
        _5->LocalRotation = quat(0,0,0,1);
        _5->LocalScale = vec3(1,1,1);
    }
    {
        Transform* _6 = _0->addChild(new Transform());
        _6->Name = std::string("RedSphere");
        _6->LocalPosition = vec3(0,0,-1.5);
        _6->LocalRotation = quat(-0.4548343,0.327839,0.1847353,0.8071681);
        _6->LocalScale = vec3(1,1,1);
    }
    return _0;
}



App::App(sf::RenderWindow *window, int w, int h):
	AppBase(window,w,h)
{
    renderState = GLRenderState::getInstance();
    shaderColor = new GLShaderColor();

    //initialize auxiliary variables
    //angle_rad = 0;
    //rotateCounterClockwise = true;

    //setup renderstate
    renderState->ClearColor = vec4(1.0f,1.0f,250.0f/255.0f,1.0f);
    
    renderState->FrontFace = FrontFaceCW;
    
    renderState->Wireframe = WireframeBack;
	renderState->CullFace = CullFaceNone;
	//renderState->Wireframe = WireframeBoth;
	//renderState->CullFace = CullFaceBack;
    
    //
    // Construct scene graph
    //
    
    root = loadSceneTestCollision();
    
    Transform *mainCameraTransform = root->findTransformByName("Main Camera");
    mainCameraTransform->addComponent(cameraPerspective = new ComponentCameraPerspective(this));
    mainCameraTransform->addComponent(new ComponentFps(this));

    //cameraPerspective->rightHanded = true;
    
	Transform* cube = root->findTransformByName("Cube");
	cube->addComponent(ComponentColorMeshVBO::createBox(vec4(0.0f,0.5f,0.0f,1.0f), vec3(1.0f,1.0f,1.0f)));
	cube->addComponent(ComponentFrustumCulling::createShapeAABB(collision::AABB(vec3(-0.5f), vec3(0.5f))));
	cube->addComponent(new ComponentFrustumVisibleSetColor());

    
    std::vector<Transform*> spheres = root->findTransformsByName("RedSphere");
    if (spheres.size() > 0){
        Component* sphereModel = ComponentColorMeshVBO::createSphere(vec4(0.5f,0.0f,0.0f,1.0f), 0.5f,16,16);
		for (int i = 0; i < spheres.size(); i++) {
			comps.add(spheres[i]->addComponent(sphereModel));

			spheres[i]->addComponent(ComponentFrustumCulling::createShapeSphere(vec3(0),0.5f));
			spheres[i]->addComponent(new ComponentFrustumVisibleSetColor());
			
		}
    }
    
    spheres = root->findTransformsByName("MagentaSphere");
    if (spheres.size() > 0){
        Component* sphereModel = ComponentColorMeshVBO::createSphere(vec4(0.5f,0.0f,0.5f,1.0f), 0.5f,16,16);
        for (int i=0;i<spheres.size();i++)
            comps.add(spheres[i]->addComponent(sphereModel));
    }
    
    root->findTransformByName("CubeCollision")->addComponent(ComponentColorMeshVBO::createBox(vec4(0.0f,0.5f,0.0f,1.0f), vec3(1.0f,1.0f,1.0f)));
    
    root->findTransformByName("SphereCollision")->addComponent(ComponentColorMeshVBO::createSphere(vec4(0.0f,0.0f,0.5f,1.0f), 0.5f,16,16));
    
    
    
    
    
    ComponentCameraPerspective* cameraTest;
    camera1 = root->findTransformByName("Camera1");
    camera1->addComponent(cameraTest = new ComponentCameraPerspective(this));
    
    //cameraTest->rightHanded = true;
    cameraTest->nearPlane = 0.5f;
    cameraTest->farPlane = 2.5f;
    cameraTest->addLinesComponent();
    

    //camera1->lookAtLeftHanded(root->findTransformByName("RedSphere"));
    
    
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
    /*
    if (rotateCounterClockwise)
        angle_rad += time.deltaTime * DEG2RAD(200.0f);
    else
        angle_rad -= time.deltaTime * DEG2RAD(200.0f) + 2.0*PI*10.0f;
    angle_rad = fmod(angle_rad, 2.0*PI*10.0f);

    box->Position = move(box->Position,  vec3(0,1,-1), time.deltaTime * 0.25f);
    if (box->Position == vec3(0,1,-1))
        box->Position = vec3(0,-1.5f,-1);
    */
    //
    // Update Nodes
    //
    
    /*
    //cameraPerspective->transform->lookAt(bigTriangle);
    smallTriangle->Euler = vec3(0.0f, angle_rad, angle_rad*0.1f);
    */
    
    vec3 euler = camera1->LocalEuler;
    euler.y += time.deltaTime * DEG2RAD(10.0f);
    camera1->LocalEuler = euler;

	vec3 p;// = camera1->LocalPosition;
	static float f = 0;
	f = fmod(f + time.deltaTime*DEG2RAD(30.0f), DEG2RAD(360));

	p = camera1->LocalRotation * vec3(0,0,1) * cos(f) * 4.0f;

	camera1->LocalPosition = p;
    
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
        if ((
             component->getType() == ComponentTypeColorMeshVBO ||
             component->getType() == ComponentTypeLinesVBO
             )
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
        
        else if (component->getType() == ComponentTypeLinesVBO){
            ComponentLinesVBO* componentLinesVBO = (ComponentLinesVBO*)component;
            
            renderState->LineWidth = componentLinesVBO->width;
            shaderColor->setColor( componentLinesVBO->color );
            
            componentLinesVBO->setLayoutPointers(GLShaderColor::vPosition);
            componentLinesVBO->draw();
            componentLinesVBO->unsetLayoutPointers(GLShaderColor::vPosition);
            
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

	//
	// Process camera visibility example
	//
	ComponentCameraPerspective *cameraComponent = (ComponentCameraPerspective *)camera1->findComponent(ComponentTypeCameraPerspective);
	cameraComponent->precomputeViewProjection(true);
	collision::Frustum frustum(cameraComponent->projection,cameraComponent->view);
	
	processCameraVisibilityExample(root, cameraComponent, frustum);
}

void App::deleteTree(Transform **element) {
	for (int i = (*element)->getChildCount()-1; i >=0 ; i--) {
		Transform * t = (*element)->removeChild(i);
		deleteTree(&t);
	}
    for(int i=(*element)->getComponentCount()-1;i>=0;i--){
		Component* component = (*element)->removeComponentAt(i);
		//if (comps.willDeleteOnRemove(component))
			//SharedPointerDatabase::getInstance()->notifyDeletion(component);
        comps.remove(component);
    }
	setNullAndDelete(*element);
}


void App::processCameraVisibilityExample(Transform *element, ComponentCameraPerspective *&camera, const collision::Frustum &frustum) {

	//bool alreadySetupMatrix = false;

	ComponentFrustumCulling *frustumCulling = (ComponentFrustumCulling *)element->findComponent(ComponentTypeFrustumCulling);

	if (frustumCulling != NULL) {
        
        if (frustumCulling->cullingShape == CullingShapeSphere) {
            frustumCulling->setVisibilityFromCamera(camera, collision::Frustum::sphereOverlapsFrustum(frustumCulling->sphere, frustum));
        }
        else if (frustumCulling->cullingShape == CullingShapeAABB) {
            frustumCulling->setVisibilityFromCamera(camera, collision::Frustum::aabbOverlapsFrustum(frustumCulling->aabb, frustum));
        }
        
        
		//check the frustum
        /*
		if (frustumCulling->cullingShape == CullingShapeSphere) {
			mat4 &m = element->getMatrix(true);
			vec3 scale = element->getScale(true);

			collision::Sphere sphere(
				toVec3(m * toPtn4(frustumCulling->sphereCenter)),
				frustumCulling->sphereRadius * minimum(minimum(scale.x, scale.y), scale.z)
			);

			frustumCulling->setVisibilityFromCamera(camera, collision::Frustum::sphereOverlapsFrustum(sphere, frustum));
		}
		else if (frustumCulling->cullingShape == CullingShapeAABB) {
			mat4 &m = element->getMatrix(true);
			vec3 scale = element->getScale(true);

			vec3 center = toVec3(m * toPtn4(frustumCulling->aabbCenter));
			vec3 dimension = frustumCulling->aabbDimension * scale;

			dimension *= 0.5f;

			collision::AABB aabb = collision::AABB(center - dimension, center + dimension);

			frustumCulling->setVisibilityFromCamera(camera, collision::Frustum::aabbOverlapsFrustum(aabb, frustum));
		}
         */
	}

	for (int i = 0; i < element->getChildCount(); i++)
		processCameraVisibilityExample(element->getChildAt(i), camera, frustum);
}
