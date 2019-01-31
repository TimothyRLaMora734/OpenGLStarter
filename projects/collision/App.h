#ifndef App__H
#define App__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include <SFML/Window.hpp>

BEGIN_DECLARE_DELEGATE(SetSizeEvent, int width, int height) CALL_PATTERN( width, height ) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(Vec2Event, vec2 parameter) CALL_PATTERN( parameter ) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(MouseEvent, sf::Mouse::Button button, vec2 parameter) CALL_PATTERN(button, parameter) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(KeyboardEvent, sf::Keyboard::Key code) CALL_PATTERN(code) END_DECLARE_DELEGATE;
BEGIN_DECLARE_DELEGATE(CallEvent) CALL_PATTERN() END_DECLARE_DELEGATE;


class App {

public:
	bool canExitApplication;

	SetSizeEvent OnSetSize;

	MouseEvent OnMouseDown;
	MouseEvent OnMouseUp;
	Vec2Event OnMouseMove;

	CallEvent OnMouseWheelUp;
	CallEvent OnMouseWheelDown;

	KeyboardEvent OnKeyDown;
	KeyboardEvent OnKeyUp;
	
	App();
	virtual ~App();
	void draw();
};

#endif