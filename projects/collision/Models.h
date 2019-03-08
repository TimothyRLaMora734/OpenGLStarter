#ifndef Models__H__
#define Models__H__

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

class GeneralVertexAttrib {
public:
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	GeneralVertexAttrib() {

	}

	GeneralVertexAttrib(vec3 position,
		vec2 uv,
		vec3 normal,
		vec3 tangent,
		vec3 binormal) {
		this->position = position;
		this->uv = uv;
		this->normal = normal;
		this->tangent = tangent;
		this->binormal = binormal;
	}
};

class TrianglesModel {
public:
	std::vector<GeneralVertexAttrib> vertices;
	std::vector<unsigned short> indices;

	void setLayoutPointers(GLint positionLayout) {
		OPENGL_CMD(glEnableVertexAttribArray(positionLayout));
		OPENGL_CMD(glVertexAttribPointer(positionLayout, 3, GL_FLOAT, false, sizeof(GeneralVertexAttrib), &vertices[0].position));
	}

	void draw() {
		OPENGL_CMD(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]));
	}

	void unsetLayoutPointers(GLint positionLayout) {
		OPENGL_CMD(glDisableVertexAttribArray(positionLayout));
	}
};


static inline TrianglesModel *CreateBox(const vec3 &dimension) {
	TrianglesModel *result = new TrianglesModel();

	vec3 halfDim = dimension * 0.5f;
	//
	// TOP
	//
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, 1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, 1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, 1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, 1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	
	//
	// BOTTOM
	//
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, -halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, -1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, -halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, -1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, -halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, -1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, -halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(0, -1, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));

	//
	// RIGHT
	//
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, -halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, -halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));

	//
	// LEFT
	//
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(-1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(-1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, -halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(-1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, -halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(-1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));

	//
	// FRONT
	//
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, -halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, -halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, halfDim.y, halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));


	//
	// BACK
	//
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(halfDim.x, -halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	result->vertices.push_back(GeneralVertexAttrib(
		vec3(-halfDim.x, -halfDim.y, -halfDim.z),// position;
		vec2(0),// uv;
		vec3(1, 0, 0),// normal;
		vec3(0),// tangent;
		vec3(0)// binormal;
	));
	

	//
	// Construct Index
	//
	for (int i = 0; i < result->vertices.size(); i += 4) {
		result->indices.push_back(i);
		result->indices.push_back(i+1);
		result->indices.push_back(i+2);

		result->indices.push_back(i);
		result->indices.push_back(i + 2);
		result->indices.push_back(i + 3);
	}

	return result;
}

static inline TrianglesModel *CreateSphere(int sectorCount, int stackCount, float radius) {
	TrianglesModel *result = new TrianglesModel();

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;

			vec3 p = vec3(x, z, -y);
			vec3 normal = vec3(nx, nz, -ny);
			vec3 tangent = normalize(cross(vec3(0, 1, 0), normal));
			vec3 binormal = normalize(cross(normal, tangent));
			GeneralVertexAttrib va(
				p,// position;
				vec2(s, t),// uv;
				normal,// normal;
				tangent,// tangent;
				binormal//vec3(0),// binormal;
			);

			result->vertices.push_back(va);
		}
	}

	int k1, k2;
	int i = 0;

	//
	// Fix tangent and binormal from poles
	//
	k1 = i * (sectorCount + 1);     // beginning of current stack
	k2 = k1 + sectorCount + 1;      // beginning of next stack

	for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
	{
		// k1+1 => k2 => k2+1
		int index1 = k1 + 1;
		int index2 = k2;
		int index3 = k2 + 1;

		result->vertices[index1].tangent = normalize(result->vertices[index2].tangent + result->vertices[index3].tangent);
		result->vertices[index1].binormal = normalize(cross(result->vertices[index1].normal, result->vertices[index1].tangent));
	}

	i = (stackCount - 1);
	k1 = i * (sectorCount + 1);     // beginning of current stack
	k2 = k1 + sectorCount + 1;      // beginning of next stack

	for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
	{
		// k1+1 => k2 => k2+1
		int index1 = k1;
		int index2 = k2;
		int index3 = k1 + 1;

		result->vertices[index2].tangent = normalize(result->vertices[index1].tangent + result->vertices[index3].tangent);
		result->vertices[index2].binormal = normalize(cross(result->vertices[index2].normal, result->vertices[index2].tangent));
	}


	for (i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				result->indices.push_back(k1);
				result->indices.push_back(k2);
				result->indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				result->indices.push_back(k1 + 1);
				result->indices.push_back(k2);
				result->indices.push_back(k2 + 1);
			}
		}
	}

	return result;
}

#endif
