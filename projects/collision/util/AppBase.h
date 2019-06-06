#ifndef AppBase__H
#define AppBase__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Size.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

BEGIN_DECLARE_DELEGATE(MouseEvent, sf::Mouse::Button button, vec2 pos) CALL_PATTERN(button, pos) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(KeyboardEvent, sf::Keyboard::Key code) CALL_PATTERN(code) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(CallEvent) CALL_PATTERN() END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(UpdateEvent, PlatformTime *time) CALL_PATTERN(time) END_DECLARE_DELEGATE;

class AppBase {

	void OnWindowSizeChanged(Property<iSize> *prop);

public:
	sf::RenderWindow *window;

	bool canExitApplication;

	//
	// Properties
	//
	Property<iSize> WindowSize;
	Property<vec2> MousePos;

	//
	// Events
	//
	MouseEvent OnMouseDown;
	MouseEvent OnMouseUp;
	CallEvent OnMouseWheelUp;
	CallEvent OnMouseWheelDown;
	KeyboardEvent OnKeyDown;
	KeyboardEvent OnKeyUp;

	UpdateEvent OnUpdate;
	UpdateEvent OnLateUpdate;
	
	AppBase(sf::RenderWindow *window, int w, int h);
	virtual ~AppBase();
	virtual void draw();

	SSE2_CLASS_NEW_OPERATOR
};

#endif
