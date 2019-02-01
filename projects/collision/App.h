#ifndef App__H
#define App__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Size.h"

#include <SFML/Window.hpp>

BEGIN_DECLARE_DELEGATE(MouseEvent, sf::Mouse::Button button, vec2 pos) CALL_PATTERN(button, pos) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(KeyboardEvent, sf::Keyboard::Key code) CALL_PATTERN(code) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(CallEvent) CALL_PATTERN() END_DECLARE_DELEGATE;

class App {

	void OnWindowSizeChanged(Property<iSize> *prop);

public:
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
	
	App(int w, int h);
	virtual ~App();
	void draw();
};

#endif
