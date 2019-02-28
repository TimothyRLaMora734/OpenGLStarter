#ifndef GLPointRenderer_H
#define GLPointRenderer_H

#include <vector>
#include <aribeiro/aribeiro.h>

using namespace aRibeiro;


class PointRenderer {

public:

	std::vector<vec3> data;

	PointRenderer(int initialNumberOfVertices) :data(initialNumberOfVertices) {

	}

	void draw(GLShaderColor *shader) {

		const int COORDS_PER_POS = 3;
		const int STRUCTURE_STRIDE_BYTES = sizeof(vec3);

		OPENGL_CMD(glEnableVertexAttribArray(GLShaderColor::vPosition));
		OPENGL_CMD(glVertexAttribPointer(GLShaderColor::vPosition, COORDS_PER_POS, GL_FLOAT, false, STRUCTURE_STRIDE_BYTES, &data[0]));
		OPENGL_CMD(glDrawArrays(GL_POINTS, 0, data.size()));
		OPENGL_CMD(glDisableVertexAttribArray(GLShaderColor::vPosition));

	}

};


#endif
