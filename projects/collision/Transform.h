#ifndef Transform__H
#define Transform__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

class TrianglesModel;
class Transform;

class Node {
	std::vector<Transform*> children;
	Transform* parent;

	//private copy constructores, to avoid copy...
	Node(const Node& v);
	void operator=(const Node& v);

protected:
	Node();

public:
	Transform* addChild(Transform* transform);
	Transform* removeChild(int index);
	Transform* removeChild(Transform * transform);
	std::vector<Transform*> &getChildren();
	Transform *getParent();
	void setParent(Transform *prnt);
	bool isRoot();

	virtual ~Node();
};

class Transform : public Node {

	//private copy constructores, to avoid copy...
	Transform(const Transform& v);
	void operator=(const Transform& v);


	bool dirty_matrixRotation;
	mat4 matrixBaseRotation;

	bool dirty_matrix;
	mat4 matrix;
	mat4 matrixParent;
	mat4 matrixResult;
	bool needToRecomputeMatrix;

	bool dirty_matrix_it;
	mat4 matrix_it;
	mat4 matrix_itParent;
	mat4 matrix_itResult;
	bool needToRecomputeMatrixIT;

	bool dirty_matrix_inv;
	mat4 matrix_inv;
	mat4 matrix_invParent;
	mat4 matrix_invResult;
	bool needToRecomputeMatrixInv;

	void updateBaseRotation();
	void OnLocalPositionScaleChange(Property<vec3> *prop);
	void OnLocalEulerChange(Property<vec3> *prop);
	void OnLocalRotationChange(Property<quat> *prop);

public:
	Property<vec3> localPosition;
	Property<vec3> localEuler;
	Property<quat> localRotation;
	Property<vec3> scale;

	vec3 forward();
	vec3 backward();
	vec3 right();
	vec3 left();
	vec3 up();
	vec3 down();

	Transform();

	mat4 &computeMatrix();
	mat4 &computeMatrixIT();
	mat4 &computeMatrixInv();

	void setPosition(const vec3 &pos);
	vec3 getPosition();

	void setRotation(const quat &rot);
	quat getRotation();

	TrianglesModel *model;
};


#endif