#ifndef FPS_CAMERA_H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "AppBase.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class FreeMoveCamera {

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
		iSizeCenter.y = size.y - 1 - iSizeCenter.y;

		screenCenter.x = iSizeCenter.x;
		screenCenter.y = iSizeCenter.y;

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

	mat4 computeTransformMatrix() {
		return translate(position) * toMat4(rotation);
	}

	mat4 computeViewMatrix() {
		//return toMat4(rotation) * translate(-position) ;// inv(computeTransformMatrix());
		mat4 matrix = toMat4(inv(rotation));

		vec3 t_inv = -position;

		matrix.d1 = matrix.a1*t_inv.x + matrix.b1*t_inv.y + matrix.c1*t_inv.z;
		matrix.d2 = matrix.a2*t_inv.x + matrix.b2*t_inv.y + matrix.c2*t_inv.z;
		matrix.d3 = matrix.a3*t_inv.x + matrix.b3*t_inv.y + matrix.c3*t_inv.z;

		return matrix;
	}

	mat4 computeViewProjectionMatrix() {
		return projection * computeViewMatrix();
	}
};


#endif