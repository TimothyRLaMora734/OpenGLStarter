#ifndef RenderSystem__H
#define RenderSystem__H

#ifdef _WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4309)
#pragma warning(disable:4018)
#pragma warning(disable:4305)
#endif

#include <wchar.h>

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>

class RenderSystem {

private:

	// for cache MVP calculation
	mat4 lastmodel, lastprojection, returnedMVP;

	RenderSystem();

public:

	GLShaderTextureColor *shader;
	GLShaderVertexColor *shaderVertexColor;

	TransformStack<mat4> projection;
	TransformStack<mat4> model;

	/*
	vec2 dimension,
		dimension_inv;
	*/

	static RenderSystem *getSingleton();
	
	virtual ~RenderSystem();

	void releaseGLResources();

	//void pushProjetionAndConfigureParallel();
	//void pushProjetionAndConfigurePerpective(float aspectX);
	//void popProjection();

	//void setupViewport(int w, int h);

	mat4 &getMVP();


	void drawTexture_center(GLTexture *texture, const vec2 &p);

	void drawTexture(GLTexture *texture, const AABB &aabb);
	void drawTexture(GLTexture *texture, GLuint oglPrimitive, const vec3 *vertexBuffer, const vec2 *uvBuffer, int count);
	void drawColor(GLuint oglPrimitive, const vec3 *vertexBuffer, const vec4 *colorBuffer, int count);
	void drawAABB_2D(const AABB &aabb, const vec4 &color);

	void drawAABB_Lines(const AABB &aabb, const vec4 &color);
	void drawAABB_Cube(const AABB &aabb, const vec4 &color);

	void print(GLFont *font, const wchar_t *txt, vec2 position, vec4 color, float alignx, float aligny, float size);
	void print(GLFont *font, const char *txt, vec2 position, vec4 color, float alignx, float aligny, float size);

	void print(GLFont *font, const wchar_t *txt, vec2 position, vec4 color);
	void print(GLFont *font, const char *txt, vec2 position, vec4 color);

	void print(GLFont *font, const wchar_t *txt, vec2 position, vec4 color, float size);
	void print(GLFont *font, const char *txt, vec2 position, vec4 color, float size);

	void drawTexture(GLTexture *texture, vec4 color, GLuint oglPrimitive, const vec3 *vertexBuffer, const vec2 *uvBuffer, int count);

};


#endif
