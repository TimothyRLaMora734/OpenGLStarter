#ifndef Transform__H
#define Transform__H

#include "VirtualProperty.h"

#include "Components/Component.h"
#include "Components/ComponentMesh.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentLines.h"
#include "Components/ComponentFrustumCulling.h"

#include "Components/ComponentFrustumVisibleSetColor.h"

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

class TrianglesModel;

extern int stat_num_visited;
extern int stat_num_recalculated;
extern int stat_draw_recalculated;

class Transform;

BEGIN_DECLARE_DELEGATE(TransformVisitedEvent, Transform* t) CALL_PATTERN(t) END_DECLARE_DELEGATE;

class Transform {
	Transform(const Transform& v);
	void operator=(const Transform& v);

	///////////////////////////////////////////////////////
	//
	//
	//
	// Node Hierarchy structure and operations
	//
	//
	//
	///////////////////////////////////////////////////////
private:
	std::vector<Transform*> children;
	Transform* parent;

public:

	Transform* removeChild(int index);
	Transform* removeChild(Transform * transform);
	Transform* addChild(Transform * transform);
	//std::vector<Transform*> &getChildren();
    int getChildCount();
    Transform* getChildAt(int);
    
	Transform *getParent();
	void setParent(Transform *prnt);
	bool isRoot();
    
    VirtualProperty<Transform *> Parent;
	///////////////////////////////////////////////////////
	//
	//
	//
	// Transform default graph operations
	//
	//
	//
	///////////////////////////////////////////////////////
private:
	vec3 localPosition;
	vec3 localEuler;
	quat localRotation;
	vec3 localScale;

	bool localRotationBaseDirty;
	mat4 localRotationBase;

	bool localMatrixDirty;
	mat4 localMatrix;
	bool localMatrixInverseTransposeDirty;
	mat4 localMatrixInverseTranspose;
	bool localMatrixInverseDirty;
	mat4 localMatrixInverse;


	void updateLocalRotationBase();
public:

	vec3 getLocalPosition()const;
	vec3 getLocalEuler()const;
	quat getLocalRotation()const;
	vec3 getLocalScale()const;

	void setLocalPosition(const vec3 &p);
	void setLocalEuler(const vec3 &e);
	void setLocalRotation(const quat &q);
	void setLocalScale(const vec3 &s);

	mat4 & getLocalMatrix();
	mat4 & getLocalMatrixInverseTranspose();
	mat4 & getLocalMatrixInverse();

	///////////////////////////////////////////////////////
	//
	//
	//
	// Transform Global Ops...
	//
	//
	//
	///////////////////////////////////////////////////////
private:

	bool matrixDirty;
	mat4 matrix, matrixParent;
	bool matrixInverseTransposeDirty;
    mat4 matrixInverseTranspose;//, matrixInverseTransposeParent;
	bool matrixInverseDirty;
	mat4 matrixInverse, matrixInverseParent;

public:
	mat4& getMatrix(bool useVisitedFlag = false);
	mat4& getMatrixInverseTranspose(bool useVisitedFlag = false);
	mat4& getMatrixInverse(bool useVisitedFlag = false);

	void setPosition(const vec3 &pos);
	vec3 getPosition();
	vec3 getPosition(bool useVisitedFlag);
	void setEuler(const vec3 &rot);
	vec3 getEuler();
	vec3 getEuler(bool useVisitedFlag);
	void setRotation(const quat &rot);
	quat getRotation();
	quat getRotation(bool useVisitedFlag);
	void setScale(const vec3 &s);
	vec3 getScale();
	vec3 getScale(bool useVisitedFlag);
    
    void lookAtRightHanded(const Transform* to, const vec3 &worldUp = vec3(0,1,0));
    void lookAtLeftHanded(const Transform* to, const vec3 &worldUp = vec3(0,1,0));

	///////////////////////////////////////////////////////
	//
	//
	//
	// Transform Render Ops...
	//
	//
	//
	///////////////////////////////////////////////////////
private:

	mat4 renderMVP, renderMV, renderMVIT, renderMVInv, renderViewProjection;
	bool renderDirty;
public:
	bool visited;

	void resetVisited(bool forceMarkFalse = false);
	void preComputeTransforms();
	void computeRenderMatrix(const mat4 &viewProjection,
		const mat4 &view,
		const mat4 &viewIT,
		const mat4 &viewInv,
		mat4 *mvp,
		mat4 *mv,
		mat4 *mvIT,
		mat4 *mvInv);
    
    TransformVisitedEvent OnVisited;
    
	///////////////////////////////////////////////////////
	//
	//
	//
	// Transform Properties
	//
	//
	//
	///////////////////////////////////////////////////////
private:
public:
	VirtualProperty<vec3> LocalPosition;
	VirtualProperty<vec3> LocalEuler;
	VirtualProperty<quat> LocalRotation;
	VirtualProperty<vec3> LocalScale;

	VirtualProperty<vec3> Position;
	VirtualProperty<vec3> Euler;
	VirtualProperty<quat> Rotation;
	VirtualProperty<vec3> Scale;
    
    mat4& worldToLocalMatrix(bool useVisitedFlag=false);
    mat4& localToWorldMatrix(bool useVisitedFlag=false);


	///////////////////////////////////////////////////////
	//
	//
	//
	// Object References...
	//
	//
	//
	///////////////////////////////////////////////////////
private:
    std::vector<Component*> components;
    std::string name;
    //std::map< std::string, std::vector<Transform*> > name2children;
    
    //void insertMapName(Transform *t);
    //void removeMapName(Transform *t);
public:
    Component* addComponent(Component*);
    Component* removeComponent(Component*);
    Component* removeComponentAt(int);
    Component* findComponent(ComponentType)const;
    std::vector<Component*> findComponents(ComponentType)const;
    int getComponentCount()const;
    Component* getComponentAt(int);
    
    Component* findComponentInChildren(ComponentType)const;
    std::vector<Component*> findComponentsInChildren(ComponentType)const;
    
    void setName(const std::string &p);
    const std::string& getName()const;
    VirtualProperty<std::string> Name;
    
    Transform * findTransformByName(const std::string &name, int maxLevel = INT_MAX);
    std::vector<Transform*> findTransformsByName(const std::string &name, int maxLevel = INT_MAX);

	Transform();
	virtual ~Transform();

	SSE2_CLASS_NEW_OPERATOR

};


/*
static void renderExample(Transform *element, BaseCamera *baseCamera) {
	std::vector<Transform*> &children = element->getChildren();
	for (int i = 0; i < children.size(); i++) {
		renderExample(children[i],baseCamera);
	}

	if (element->model != NULL) {
		mat4 mvp;
		mat4 mv;
		mat4 mvIT;
		mat4 mvInv;
		element->computeRenderMatrix(
									 baseCamera->getViewProjection(),
									 baseCamera->getView(),
									 baseCamera->getViewIT(),
									 baseCamera->getViewInv(),
									 &mvp,
									 &mv,
									 &mvIT,
									 &mvInv);

		//
		// Setup shader matrix...
		//

	}
}

static void renderExample() {
	BaseCamera baseCamera;
	Transform *root = new Transform();

	root->resetVisited();
	root->preComputeTransforms();

	renderExample(root, &baseCamera);
}
*/

#endif
