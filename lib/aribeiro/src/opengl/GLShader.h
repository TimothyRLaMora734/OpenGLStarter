#ifndef GL_SHADER_h__
#define GL_SHADER_h__

#include <glew/glew.h> // extensions here
#include <aribeiro/all_math.h>

namespace aRibeiro {

	class GLShader {
	public:

		GLint mProgram;

	protected:

		static GLint loadShader(int type, const char* shaderCode);
		static GLint loadShaderStrings(const char* vertexShaderCode, const  char* fragmentShaderCode);
		static GLint getCurrentShader();

		void LoadShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode);

		GLint getAttribLocation(const char* name);
		GLint getUniformLocation(const char* name);

		void setUniform(int location, int v);
		void setUniform(int location, float v);
		void setUniform(int location, const vec2 &v);
		void setUniform(int location, const vec3 &v);
		void setUniform(int location, const vec4 &v);
		void setUniform(int location, const mat4 &v);

		int getUniformInt(int location);
		float getUniformFloat(int location);
		vec2 getUniformVec2(int location);
		vec3 getUniformVec3(int location);
		vec4 getUniformVec4(int location);
		mat4 getUniformMat4(int location);

	public:

		void enable();
		static void disable();

		GLShader();
		virtual ~GLShader();

		/*
		//
		// for use the glVertexAttribPointer
		//
		virtual void EnableVertexAttribArray(int attribLocation);
		virtual void DisableVertexAttribArray(int attribLocation);
		*/


	};

}

#endif
