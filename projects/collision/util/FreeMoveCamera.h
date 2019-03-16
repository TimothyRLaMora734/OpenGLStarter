#ifndef FPS_CAMERA_H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "AppBase.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "../Transform.h"



class BaseCamera{
protected:
    mat4 viewProjection;
    mat4 view;
    mat4 viewIT;
    mat4 viewInv;
public:
    BaseCamera(){
        mat4 viewProjection = mat4::IdentityMatrix;
        mat4 view = mat4::IdentityMatrix;
        mat4 viewIT = mat4::IdentityMatrix;
        mat4 viewInv = mat4::IdentityMatrix;
    }
    virtual ~BaseCamera(){
    }
    virtual mat4& getViewProjection(){
        return viewProjection;
    }
    virtual mat4& getView(){
        return view;
    }
    virtual mat4& getViewIT(){
        return viewIT;
    }
    virtual mat4& getViewInv(){
        return viewInv;
    }
};



class FreeMoveCamera:public BaseCamera{

	PressReleaseDetector left, right, up, down;

	AppBase *appBase;
	iSize size;
	sf::Vector2i iSizeCenter;
	vec2 screenCenter;

	vec3 euler;

	void OnUpdateCameraFloatParameter(Property<float> *prop) {
		projection = projection_perspective(fovDegrees, (float)size.x / (float)size.y, nearPlane, farPlane);
	}

	void OnWindowSizeChanged(Property<iSize> *prop) {
		iSize size = prop->value;
		this->size = size;
		iSizeCenter.x = size.x / 2;
		iSizeCenter.y = size.y / 2;

		//fix inverted mouse coord
		//iSizeCenter.y = size.y - 1 - iSizeCenter.y;

		screenCenter.x = iSizeCenter.x;
		screenCenter.y = size.y - 1 - iSizeCenter.y;// iSizeCenter.y;

		sf::Mouse::setPosition(iSizeCenter, *appBase->window);

		projection = projection_perspective(fovDegrees, (float)size.x / (float)size.y, nearPlane, farPlane);
	}

	void OnMousePosChanged(Property<vec2> *prop) {
		vec2 pos = prop->value;

		vec2 delta = pos - screenCenter;

		if (sqrLength(delta) > 1e-6f) {

			euler.y = fmod(euler.y - delta.x * angleSpeed + 2.0f*PI, 2.0f*PI);
			//euler.x = fmod(euler.x + delta.y * angleSpeed + 2.0f*PI, 2.0f*PI);
			euler.x = euler.x + delta.y * angleSpeed;
			euler.x = clamp(euler.x, -DEG2RAD(90.0f), DEG2RAD(90.0f));

			rotation = quatFromEuler(euler.x, euler.y, euler.z);

			sf::Mouse::setPosition(iSizeCenter, *appBase->window);
		}
	}

	void Update(PlatformTime *time) {
		left.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
		right.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
		up.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
		down.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));

		vec3 forwardVec = rotation * vec3(0, 0, -1);
		vec3 rightVec = rotation * vec3(1, 0, 0);

		if (left.pressed)
			position -= rightVec * strafeSpeed * time->deltaTime;
		if (right.pressed)
			position += rightVec * strafeSpeed * time->deltaTime;
		if (up.pressed)
			position += forwardVec * strafeSpeed * time->deltaTime;
		if (down.pressed)
			position -= forwardVec * strafeSpeed * time->deltaTime;
	}
    
    quat rotationCache;
    vec3 positionCache;
    mat4 projectionCache;
    
public:

	Property<float> fovDegrees;
	Property<float> nearPlane;
	Property<float> farPlane;

	float forwardSpeed;
	float strafeSpeed;
	float angleSpeed;

	quat rotation;
	vec3 position;
	mat4 projection;

	FreeMoveCamera(AppBase *appBase) {
		this->appBase = appBase;

		fovDegrees = 60.0f;
		nearPlane = 0.01f;
		farPlane = 10.0f;

		appBase->MousePos.OnChange.add(this, &FreeMoveCamera::OnMousePosChanged);
		appBase->WindowSize.OnChange.add(this, &FreeMoveCamera::OnWindowSizeChanged);
		appBase->OnUpdate.add(this, &FreeMoveCamera::Update);

		//call the first setup
		OnWindowSizeChanged(&appBase->WindowSize);

		forwardSpeed = 1.0f;
		strafeSpeed = 1.0f;
		angleSpeed = DEG2RAD(0.10f);

		fovDegrees.OnChange.add(this, &FreeMoveCamera::OnUpdateCameraFloatParameter);
		nearPlane.OnChange.add(this, &FreeMoveCamera::OnUpdateCameraFloatParameter);
		farPlane.OnChange.add(this, &FreeMoveCamera::OnUpdateCameraFloatParameter);
        
	}

	~FreeMoveCamera() {
		appBase->MousePos.OnChange.remove(this, &FreeMoveCamera::OnMousePosChanged);
		appBase->WindowSize.OnChange.remove(this, &FreeMoveCamera::OnWindowSizeChanged);
		appBase->OnUpdate.remove(this, &FreeMoveCamera::Update);
	}
    
    void updateBaseMatrix(){
        
        if(rotationCache == rotation && positionCache == position && projectionCache == projection)
            return;
        
        rotationCache = rotation;
        positionCache = position;
        projectionCache = projection;
        
        mat4 rotationBase = toMat4(rotation);
        
        viewInv = rotationBase;
        
        viewInv.d1 = position.x;
        viewInv.d2 = position.y;
        viewInv.d3 = position.z;
        
        view = transpose(rotationBase);
        viewIT = view;
        
        vec3 t_inv = -position;
        
        view.d1 = view.a1*t_inv.x + view.b1*t_inv.y + view.c1*t_inv.z;
        view.d2 = view.a2*t_inv.x + view.b2*t_inv.y + view.c2*t_inv.z;
        view.d3 = view.a3*t_inv.x + view.b3*t_inv.y + view.c3*t_inv.z;
        
        viewIT.a4 = position.x;
        viewIT.b4 = position.y;
        viewIT.c4 = position.z;
        
        viewProjection = projection * view;
    }
    
};


#endif
