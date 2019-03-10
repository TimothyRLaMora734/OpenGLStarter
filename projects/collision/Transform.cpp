#include "Transform.h"

void Transform::updateBaseRotation() {
	if (!dirty_matrixRotation)
		return;
	matrixBaseRotation = toMat4(localRotation);
	dirty_matrixRotation = false;
}

void Transform::OnLocalPositionScaleChange(Property<vec3> *prop) {
	if (!dirty_matrixRotation)
		dirty_matrixRotation = true;
	if (!dirty_matrix)
		dirty_matrix = true;
	if (!dirty_matrix_it)
		dirty_matrix_it = true;
	if (!dirty_matrix_inv)
		dirty_matrix_inv = true;
}

void Transform::OnLocalEulerChange(Property<vec3> *prop) {
	if (!dirty_matrixRotation)
		dirty_matrixRotation = true;
	if (!dirty_matrix)
		dirty_matrix = true;
	if (!dirty_matrix_it)
		dirty_matrix_it = true;
	if (!dirty_matrix_inv)
		dirty_matrix_inv = true;

	if (prop->value.x < 0)
		prop->value.x += ceil( - prop->value.x / DEG2RAD(360.0f)) * DEG2RAD(360.0f);
	if (prop->value.x > DEG2RAD(360.0f))
		prop->value.x = fmod(prop->value.x, DEG2RAD(360.0f));

	if (prop->value.y < 0)
		prop->value.y += ceil(-prop->value.y / DEG2RAD(360.0f)) * DEG2RAD(360.0f);
	if (prop->value.y > DEG2RAD(360.0f))
		prop->value.y = fmod(prop->value.y, DEG2RAD(360.0f));

	if (prop->value.z < 0)
		prop->value.z += ceil(-prop->value.z / DEG2RAD(360.0f)) * DEG2RAD(360.0f);
	if (prop->value.z > DEG2RAD(360.0f))
		prop->value.z = fmod(prop->value.z, DEG2RAD(360.0f));


	localRotation.value = quatFromEuler(prop->value.x, prop->value.y, prop->value.z);
}

void Transform::OnLocalRotationChange(Property<quat> *prop) {
	if (!dirty_matrixRotation)
		dirty_matrixRotation = true;
	if (!dirty_matrix)
		dirty_matrix = true;
	if (!dirty_matrix_it)
		dirty_matrix_it = true;
	if (!dirty_matrix_inv)
		dirty_matrix_inv = true;

	extractEuler(localRotation, &localEuler.value.x, &localEuler.value.y, &localEuler.value.z);
}

vec3 Transform::forward() {
	updateBaseRotation();
	return toVec3(-matrixBaseRotation[2]);
}
vec3 Transform::backward() {
	updateBaseRotation();
	return toVec3(matrixBaseRotation[2]);
}
vec3 Transform::right() {
	updateBaseRotation();
	return toVec3(matrixBaseRotation[0]);
}
vec3 Transform::left() {
	updateBaseRotation();
	return toVec3(-matrixBaseRotation[0]);
}
vec3 Transform::up() {
	updateBaseRotation();
	return toVec3(matrixBaseRotation[1]);
}
vec3 Transform::down() {
	updateBaseRotation();
	return toVec3(-matrixBaseRotation[1]);
}

Transform::Transform():Node(){
	localPosition = vec3(0);
	localEuler = vec3(0);
	localRotation = quat();
	scale = vec3(1.0f);

	dirty_matrix = false;
	matrix = mat4::IdentityMatrix;

	dirty_matrix_it = false;
	matrix_it = mat4::IdentityMatrix;

	dirty_matrixRotation = false;
	matrixBaseRotation = mat4::IdentityMatrix;

	dirty_matrix_inv = false;
	matrix_inv = mat4::IdentityMatrix;

	localPosition.OnChange.add(this, &Transform::OnLocalPositionScaleChange);
	localEuler.OnChange.add(this, &Transform::OnLocalEulerChange);
	localRotation.OnChange.add(this, &Transform::OnLocalRotationChange);
	scale.OnChange.add(this, &Transform::OnLocalPositionScaleChange);

	needToRecomputeMatrix = true;
	needToRecomputeMatrixIT = true;
	needToRecomputeMatrixInv = true;

	model = NULL;
	projection = NULL;
	view = NULL;
	modelViewProjectionDirty = true;
}

mat4 &Transform::computeMatrixModelViewProjection() {
	mat4 *transform = NULL;

	if (dirty_matrix) {
		//
		// compute the matrix transform
		//
		updateBaseRotation();

		matrix = matrixBaseRotation;

		matrix.a1 *= scale.value.x;
		matrix.a2 *= scale.value.x;
		matrix.a3 *= scale.value.x;

		matrix.b1 *= scale.value.y;
		matrix.b2 *= scale.value.y;
		matrix.b3 *= scale.value.y;

		matrix.c1 *= scale.value.z;
		matrix.c2 *= scale.value.z;
		matrix.c3 *= scale.value.z;

		matrix.d1 = localPosition.value.x;
		matrix.d2 = localPosition.value.y;
		matrix.d3 = localPosition.value.z;

		dirty_matrix = false;
		needToRecomputeMatrix = true;
		modelViewProjectionDirty = true;
	}
	Transform* parent = ((Transform*)getParent());
	if (parent != NULL// && !parent->isRoot()
		) {
		mat4 &aux = parent->computeMatrixModelViewProjection();
		if (matrixParent != aux) {
			matrixParent = aux;
			needToRecomputeMatrix = true;
		}
		if (needToRecomputeMatrix) {
			matrixResult = matrixParent * matrix;
			needToRecomputeMatrix = false;
		}
		transform = &matrixResult;
	}
	else
		transform = &matrix;


	if (isRoot()) {
		if ((projection) != projectionCache || (view) != viewCache) {
			projectionCache = projection;
			viewCache = view;
			viewProjection = projectionCache * viewCache;
			modelViewProjectionDirty = true;
		}
		if (modelViewProjectionDirty)
		{
			modelViewProjection = viewProjection;// *(*transform);
			modelViewProjectionDirty = false;
		}
		return modelViewProjection;
	}
	return (*transform);
}

mat4 &Transform::computeMatrix() {
	if (dirty_matrix) {
		//
		// compute the matrix transform
		//
		updateBaseRotation();

		matrix = matrixBaseRotation;

		matrix.a1 *= scale.value.x;
		matrix.a2 *= scale.value.x;
		matrix.a3 *= scale.value.x;

		matrix.b1 *= scale.value.y;
		matrix.b2 *= scale.value.y;
		matrix.b3 *= scale.value.y;

		matrix.c1 *= scale.value.z;
		matrix.c2 *= scale.value.z;
		matrix.c3 *= scale.value.z;

		matrix.d1 = localPosition.value.x;
		matrix.d2 = localPosition.value.y;
		matrix.d3 = localPosition.value.z;

		dirty_matrix = false;
		needToRecomputeMatrix = true;
		modelViewProjectionDirty = true;
	}
	Transform* parent = ((Transform*)getParent());
	if (parent != NULL && !parent->isRoot()) {
		mat4 &aux = parent->computeMatrix();
		if (matrixParent != aux) {
			matrixParent = aux;
			needToRecomputeMatrix = true;
		}
		if (needToRecomputeMatrix) {
			matrixResult = matrixParent * matrix;
			needToRecomputeMatrix = false;
		}
		return matrixResult;
	} else
		return matrix;
}

mat4 &Transform::computeMatrixIT() {
	if (dirty_matrix_it) {

		updateBaseRotation();

		matrix_it = matrixBaseRotation;

		vec3 t_inv = -localPosition.value;
		matrix_it.a4 = matrix_it.a1 * t_inv.x + matrix_it.a2 * t_inv.y + matrix_it.a3 * t_inv.z;
		matrix_it.b4 = matrix_it.b1 * t_inv.x + matrix_it.b2 * t_inv.y + matrix_it.b3 * t_inv.z;
		matrix_it.c4 = matrix_it.c1 * t_inv.x + matrix_it.c2 * t_inv.y + matrix_it.c3 * t_inv.z;

		matrix_it[0] *= 1.0f / scale.value.x;
		matrix_it[1] *= 1.0f / scale.value.y;
		matrix_it[2] *= 1.0f / scale.value.z;

		dirty_matrix_it = false;
		needToRecomputeMatrixIT = true;
	}

	Transform* parent = ((Transform*)getParent());
	if (parent != NULL && !parent->isRoot()) {
		//return parent->computeMatrixIT() * matrix_it;
		mat4 &aux = parent->computeMatrixIT();
		if (matrix_itParent != aux) {
			matrix_itParent = aux;
			needToRecomputeMatrixIT = true;
		}
		if (needToRecomputeMatrixIT) {
			matrix_itResult = matrix_itParent * matrix_it;
			needToRecomputeMatrixIT = false;
		}
		return matrix_itResult;
	}
	else
		return matrix_it;
}

mat4 &Transform::computeMatrixInv() {

	if (dirty_matrix_inv) {
		updateBaseRotation();

		matrix_inv = transpose(matrixBaseRotation);

		vec3 s_inv = 1.0f / scale;

		matrix_inv.a1 *= s_inv.x;
		matrix_inv.a2 *= s_inv.y;
		matrix_inv.a3 *= s_inv.z;

		matrix_inv.b1 *= s_inv.x;
		matrix_inv.b2 *= s_inv.y;
		matrix_inv.b3 *= s_inv.z;

		matrix_inv.c1 *= s_inv.x;
		matrix_inv.c2 *= s_inv.y;
		matrix_inv.c3 *= s_inv.z;

		vec3 t_inv = -localPosition.value;

		matrix.d1 = matrix.a1*t_inv.x + matrix.b1*t_inv.y + matrix.c1*t_inv.z;
		matrix.d2 = matrix.a2*t_inv.x + matrix.b2*t_inv.y + matrix.c2*t_inv.z;
		matrix.d3 = matrix.a3*t_inv.x + matrix.b3*t_inv.y + matrix.c3*t_inv.z;

		dirty_matrix_inv = false;
		needToRecomputeMatrixInv = true;
	}

	Transform* parent = ((Transform*)getParent());
	if (parent != NULL && !parent->isRoot()) {
		mat4 &aux = parent->computeMatrixInv();
		if (matrix_invParent != aux) {
			matrix_invParent = aux;
			needToRecomputeMatrixInv = true;
		}
		if (needToRecomputeMatrixInv) {
			matrix_invResult = matrix_inv * matrix_invParent;
			needToRecomputeMatrixInv = false;
		}
		return matrix_invResult;
		//return matrix_inv * parent->computeMatrixInv();
	}
	else
		return matrix_inv;
}

void Transform::setPosition(const vec3 &pos) {
	localPosition = toVec3(computeMatrixInv() * toVec4(pos - localPosition.value));
}

vec3 Transform::getPosition() {
	return toVec3(computeMatrix()[3]);
}

void Transform::setRotation(const quat &rot) {
	quat q = extractQuat(computeMatrixInv());
	localRotation = q * rot * inv( localRotation.value );
}

quat Transform::getRotation() {
	quat q = extractQuat(computeMatrix());
	return q;
}

//private copy constructores, to avoid copy...
Transform::Transform(const Transform& v) {

}

void Transform::operator=(const Transform& v) {

}




Node::Node(const Node& v) {}
void Node::operator=(const Node& v) {}

Node::Node() {
	parent = NULL;
	root = (Transform*)this;
}

Node::~Node() {
}

Transform* Node::addChild(Transform * transform) {
	if (transform->parent != NULL)
		transform->parent->removeChild(transform);
	transform->parent = (Transform*)this;
	children.push_back(transform);
	transform->root = root;
	return transform;
}

Transform* Node::removeChild(int index) {
	if (index >= children.size())
	{
		fprintf(stderr, "Trying to remove a child that is not in the list...\n");
		exit(-1);
		return NULL;
	}
	Transform* node = children[index];
	children.erase(children.begin() + index);
	return node;
}
Transform* Node::removeChild(Transform * transform) {
	for (int i=0;i< children.size();i++)
		if (children[i] == transform) {
			children.erase(children.begin() + i);
			transform->parent = NULL;
			transform->root = transform;
			return transform;
		}
	fprintf(stderr,"Trying to remove a child that is not in the scene...\n");
	exit(-1);
	return NULL;
}

std::vector<Transform*> &Node::getChildren() {
	return children;
}

Transform *Node::getParent() {
	return parent;
}

void Node::setParent(Transform *prnt) {
	if (prnt == NULL) {
		fprintf(stderr,"Trying to set parent to NULL...\n");
		exit(-1);
	}
	if (parent != NULL)
		parent->removeChild((Transform*)this);
	parent = prnt;
	parent->addChild(prnt);

}

bool Node::isRoot() {
	return parent == NULL;
}


//
// UpdateTag
//
