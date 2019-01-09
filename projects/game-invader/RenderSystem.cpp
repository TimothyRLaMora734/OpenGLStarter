#include "RenderSystem.h"

#include <string.h>

RenderSystem::RenderSystem(GameResources *gr) {

	gameResources = gr;

	glClearColor(0, 0, 0, 1);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);


	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);

	//glDepthMask(GL_TRUE);
	glDepthMask(GL_TRUE);//depth writing enabled

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);//use alpha blend in separate channels
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 2.0f / 255.0f);

	glDisable(GL_ALPHA_TEST);

	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glLineWidth(4.0);

	shader = new GLShaderTextureColor();
	shaderVertexColor = new GLShaderVertexColor();
}

RenderSystem::~RenderSystem() {
	setNullAndDelete(shader);
	setNullAndDelete(shaderVertexColor);
}

void RenderSystem::pushProjetionAndConfigureParallel() {
	projection.push();
	projection.top = translate(-1.0f, -1.0f, 0.0f) * scale(2.0f / dimension.x, 2.0f / dimension.y, 0.0f);
	model.push();
	model.top = mat4::IdentityMatrix;
}

void RenderSystem::pushProjetionAndConfigurePerpective(float aspectX) {
	projection.push();
	projection.top = projection_perspective(60.0f, aspectX, 0.001f, 1000.0f);
	model.push();
	model.top = mat4::IdentityMatrix;
}

void RenderSystem::popProjection()
{
	projection.pop();
	model.pop();
}

void RenderSystem::setupViewport(int w, int h) {

	if (projection.size() > 0)
		printf("Error, stack is not root...\n");

	glViewport(0, 0, w, h);

	projection.top = projection_perspective(60.0f, (double)w / (double)h, 0.001f, 1000.0f);
	model.top = mat4::IdentityMatrix;

	dimension = vec2(w, h);
	dimension_inv.x = 2.0f / (float)w;
	dimension_inv.y = 2.0f / (float)h;

}

mat4 &RenderSystem::getMVP() {
	//mat4 lastmodel, lastprojection, returnedMVP;
	//return projection.top * model.top;

	//
	// computes the MVP if the model and projection matrix are different...
	//
	if ( (memcmp(projection.top.array, lastprojection.array, sizeof(mat4)) != 0) ||
		 (memcmp(model.top.array, lastmodel.array, sizeof(mat4)) != 0) ) {

		returnedMVP = projection.top * model.top;

		lastprojection = projection.top;
		lastmodel = model.top;

	}

	return returnedMVP;
}

void RenderSystem::drawAABB(const AABB &aabb, const vec4 &color) {

    vec3 pos [] = {
        vec3(aabb.min_box.x, aabb.min_box.y, 0.0f),
        vec3(aabb.max_box.x, aabb.min_box.y, 0.0f),
        vec3(aabb.max_box.x, aabb.max_box.y, 0.0f),
        vec3(aabb.min_box.x, aabb.max_box.y, 0.0f)
    };

    vec4 vcolor [] = {
        color,
        color,
        color,
        color
    };

	drawColor(GL_QUADS, pos, vcolor, 4);
}


void RenderSystem::drawTexture(GLTexture *texture, const AABB &aabb) {

    vec3 vpos [] = {

        vec3(aabb.min_box.x, aabb.min_box.y, 0.0f),
        vec3(aabb.max_box.x, aabb.min_box.y, 0.0f),
        vec3(aabb.max_box.x, aabb.max_box.y, 0.0f),
        vec3(aabb.min_box.x, aabb.max_box.y, 0.0f)
    };

    vec2 vuv [] = {
        vec2(0, 1),
        vec2(1, 1),
        vec2(1, 0),
        vec2(0, 0)
    };

	drawTexture(texture, GL_QUADS, vpos, vuv, 4);
}

void RenderSystem::drawTexture(GLTexture *texture, GLuint oglPrimitive, const vec3 *vertexBuffer, const vec2 *uvBuffer, int count) {

	shader->enable();

	shader->setMatrix(getMVP());

	texture->active(0);
	shader->setTexture(0);

	//
	// Set the vertex position attrib array
	//
	OPENGL_CMD(glEnableVertexAttribArray(shader->vPosition));

	OPENGL_CMD(glVertexAttribPointer(shader->vPosition, 3, GL_FLOAT, false, sizeof(vec3), &vertexBuffer[0]));

	//
	// Set the vertex uv attrib array
	//
	OPENGL_CMD(glEnableVertexAttribArray(shader->vUV));
	OPENGL_CMD(glVertexAttribPointer(shader->vUV, 2, GL_FLOAT, false, sizeof(vec2), &uvBuffer[0]));

	//
	// Draw quad
	//
	OPENGL_CMD(glDrawArrays(oglPrimitive, 0, count));

	//
	// Disable arrays after draw
	//
	OPENGL_CMD(glDisableVertexAttribArray(shader->vPosition));
	OPENGL_CMD(glDisableVertexAttribArray(shader->vUV));


}


void RenderSystem::drawColor(GLuint oglPrimitive, const vec3 *vertexBuffer, const vec4 *colorBuffer, int count) {


	shaderVertexColor->enable();

	shaderVertexColor->setMatrix(getMVP());

	//
	// Set the vertex position attrib array
	//
	OPENGL_CMD(glEnableVertexAttribArray(shaderVertexColor->vPosition));
	OPENGL_CMD(glVertexAttribPointer(shaderVertexColor->vPosition, 3, GL_FLOAT, false, sizeof(vec3), &vertexBuffer[0]));

	//
	// Set the vertex color attrib array
	//
	OPENGL_CMD(glEnableVertexAttribArray(shaderVertexColor->vColor));
	OPENGL_CMD(glVertexAttribPointer(shaderVertexColor->vColor, 4, GL_FLOAT, false, sizeof(vec4), &colorBuffer[0]));

	//
	// Draw quad
	//
	OPENGL_CMD(glDrawArrays(oglPrimitive, 0, count));

	//
	// Disable arrays after draw
	//
	OPENGL_CMD(glDisableVertexAttribArray(shaderVertexColor->vPosition));
	OPENGL_CMD(glDisableVertexAttribArray(shaderVertexColor->vColor));



}


void RenderSystem::print(const wchar_t *txt, vec2 position, vec4 color, float alignx, float aligny) {

	AABB aabb = gameResources->font->computeBounds(txt);

	vec3 center = vec3(0);

	center.x = -aabb.max_box.x + lerp(0.0f, aabb.max_box.x - aabb.min_box.x, 1.0f - alignx);
	center.y = -aabb.max_box.y + lerp(0.0f, aabb.max_box.y - aabb.min_box.y, 1.0f - aligny);


	GLShaderFont *shaderfont = gameResources->font->getShader();
	shaderfont->enable();
	shaderfont->setMatrix(
		getMVP() *
		translate(vec3(position, 0.0f)) *
		scale(vec3(0.75f)) *
		translate(center)
	);

	gameResources->font->print(color, txt);


}
