#include "GLRenderState.h"

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;


void GLRenderState::OnClearColorChange(Property<vec4> *prop) {
	glClearColor(prop->value.r, prop->value.g, prop->value.b, prop->value.a);
}
void GLRenderState::OnCullFaceChange(Property<CullFaceType> *prop) {
	switch (prop->value) {
	case CullFaceNone:
		glDisable(GL_CULL_FACE);
		break;
	case CullFaceFront:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case CullFaceBack:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	}
}
void GLRenderState::OnFrontFaceChange(Property<FrontFaceType> *prop) {
	switch (prop->value) {
	case FrontFaceCCW:
		glFrontFace(GL_CCW);
		break;
	case FrontFaceCW:
		glFrontFace(GL_CW);
		break;
	}
}
void GLRenderState::OnDepthTestChange(Property<DepthTestType> *prop) {
	switch (prop->value) {
		case DepthTestDisabled:
			glDisable(GL_DEPTH_TEST);
			break;
		case DepthTestEqual:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_EQUAL);
			break;
		case DepthTestLess:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			break;
		case DepthTestLessEqual:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			break;
		case DepthTestGreater:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_GREATER);
			break;
		case DepthTestGreaterEqual:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_GEQUAL);
			break;
	}
}
void GLRenderState::OnClearDepthChange(Property<float> *prop) {
	glClearDepth(prop->value);
}
void GLRenderState::OnDepthWriteChange(Property<bool> *prop) {
	glDepthMask(prop->value);
}
void GLRenderState::OnBlendModeChange(Property<BlendModeType> *prop) {
	switch (prop->value) {
	case BlendModeDisabled:
		glDisable(GL_BLEND);
		break;
	case BlendModeAlpha:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case BlendModeAdd:
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		break;
	case BlendModeAddAlpha:
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}

}
void GLRenderState::OnAlphaTestChange(Property<AlphaTestType> *prop) {
	switch (prop->value) {
	case AlphaTestDisabled:
		glDisable(GL_ALPHA_TEST);
		break;
	case AlphaTestEqual:
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_EQUAL, AlphaRef.value);
		break;
	case AlphaTestLess:
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_LESS, AlphaRef.value);
		break;
	case AlphaTestLessEqual:
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_LEQUAL, AlphaRef.value);
		break;
	case AlphaTestGreater:
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, AlphaRef.value);
		break;
	case AlphaTestGreaterEqual:
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, AlphaRef.value);
		break;
	}
}
void GLRenderState::OnAlphaRefChange(Property<float> *prop) {
	//forward the alpha ref change to another processor
	OnAlphaTestChange(&AlphaTest);
}
void GLRenderState::OnLineSmoothHintChange(Property<HintType> *prop) {
	switch (prop->value) {
	case HintDisabled:
		glDisable(GL_LINE_SMOOTH);
		break;
	case HintFastest:
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		break;
	case HintNicest:
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		break;
	}
}
void GLRenderState::OnLineWidthChange(Property<float> *prop) {
	glLineWidth(prop->value);
}
void GLRenderState::OnPointSizeChange(Property<float> *prop) {
	glPointSize(prop->value);
}
void GLRenderState::OnWireframeChange(Property<WireframeType> *prop) {
	switch (prop->value) {
	case WireframeDisabled:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case WireframeBoth:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case WireframeFront:
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case WireframeBack:
		glPolygonMode(GL_BACK, GL_LINE);
		break;
	}
}
void GLRenderState::OnCurrentShaderChange(Property<GLShader*> *prop) {
	if (prop->value == NULL)
		GLShader::disable();
	else
		prop->value->enable();
}
void GLRenderState::OnViewportChange(Property<iRect> *prop) {
	glViewport(prop->value.x, prop->value.y, prop->value.w, prop->value.h);
}
void GLRenderState::OnCurrentFramebufferObjectChange(Property<GLFramebufferObject*> *prop) {
	if (prop->value == NULL)
		GLFramebufferObject::disable();
	else
		prop->value->enable();
}

GLRenderState *GLRenderState::getInstance() {
	static GLRenderState renderState;
	return &renderState;
}

void GLRenderState::initialize(int w, int h) {
	if (initialized)
		return;
	initialized = true;

	ClearColor = vec4(0, 0, 0, 1);
	CullFace = CullFaceBack;
	FrontFace = FrontFaceCCW;
	DepthTest = DepthTestLessEqual;
	ClearDepth = 1.0f;
	DepthWrite = true;
	BlendMode = BlendModeAlpha;
	AlphaTest = AlphaTestDisabled;
	AlphaRef = 1.0f / 255.0f;
	LineSmoothHint = HintDisabled;
	LineWidth = 1.0f;
	PointSize = 1.0f;
	Wireframe = WireframeDisabled;
	CurrentShader = NULL;
	Viewport = iRect(w, h);
	CurrentFramebufferObject = NULL;

	//
	// assign change listeners
	//
	ClearColor.OnChange.add(this, &GLRenderState::OnClearColorChange);
	CullFace.OnChange.add(this, &GLRenderState::OnCullFaceChange);
	FrontFace.OnChange.add(this, &GLRenderState::OnFrontFaceChange);
	DepthTest.OnChange.add(this, &GLRenderState::OnDepthTestChange);
	ClearDepth.OnChange.add(this, &GLRenderState::OnClearDepthChange);
	DepthWrite.OnChange.add(this, &GLRenderState::OnDepthWriteChange);
	BlendMode.OnChange.add(this, &GLRenderState::OnBlendModeChange);
	AlphaTest.OnChange.add(this, &GLRenderState::OnAlphaTestChange);
	AlphaRef.OnChange.add(this, &GLRenderState::OnAlphaRefChange);
	LineSmoothHint.OnChange.add(this, &GLRenderState::OnLineSmoothHintChange);
	LineWidth.OnChange.add(this, &GLRenderState::OnLineWidthChange);
	PointSize.OnChange.add(this, &GLRenderState::OnPointSizeChange);
	Wireframe.OnChange.add(this, &GLRenderState::OnWireframeChange);
	CurrentShader.OnChange.add(this, &GLRenderState::OnCurrentShaderChange);
	Viewport.OnChange.add(this, &GLRenderState::OnViewportChange);
	CurrentFramebufferObject.OnChange.add(this, &GLRenderState::OnCurrentFramebufferObjectChange);

	//
	// Call all listener to do first setup with the default values
	//
	ClearColor.OnChange.call(&ClearColor);
	CullFace.OnChange.call(&CullFace);
	FrontFace.OnChange.call(&FrontFace);
	DepthTest.OnChange.call(&DepthTest);
	ClearDepth.OnChange.call(&ClearDepth);
	DepthWrite.OnChange.call(&DepthWrite);
	BlendMode.OnChange.call(&BlendMode);
	AlphaTest.OnChange.call(&AlphaTest);
	AlphaRef.OnChange.call(&AlphaRef);
	LineSmoothHint.OnChange.call(&LineSmoothHint);
	LineWidth.OnChange.call(&LineWidth);
	PointSize.OnChange.call(&PointSize);
	Wireframe.OnChange.call(&Wireframe);
	CurrentShader.OnChange.call(&CurrentShader);
	Viewport.OnChange.call(&Viewport);
	CurrentFramebufferObject.OnChange.call(&CurrentFramebufferObject);

	//
	// Not using fixed pipeline lighting
	//
	glDisable(GL_LIGHTING);

}

void GLRenderState::releaseResources() {
	CurrentShader = NULL;
	CurrentFramebufferObject = NULL;
}

GLRenderState::GLRenderState() {
	initialized = false;
}
