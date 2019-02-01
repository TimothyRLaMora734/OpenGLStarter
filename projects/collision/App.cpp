#include "App.h"

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "Size.h"

#include "GLRenderState.h"

App::App(int w,int h) {
	GLRenderState *renderState = GLRenderState::getInstance();
	renderState->initialize(w,h);


	canExitApplication = false;
	WindowSize.OnChange.add(this, &App::OnWindowSizeChanged);

	WindowSize = iSize(w, h);
}

App::~App() {
}

void App::OnWindowSizeChanged(Property<iSize> *prop) {
	GLRenderState *renderState = GLRenderState::getInstance();
	renderState->Viewport = iRect(prop->value.x, prop->value.y);
}

void App::draw() {
	GLRenderState *renderState = GLRenderState::getInstance();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

