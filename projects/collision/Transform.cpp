#include "Transform.h"


int stat_num_visited;
int stat_num_recalculated;
int stat_draw_recalculated;


Transform::Transform(const Transform& v) :

	LocalPosition(this, &Transform::getLocalPosition, &Transform::setLocalPosition),
	LocalEuler(this, &Transform::getLocalEuler, &Transform::setLocalEuler),
	LocalRotation(this, &Transform::getLocalRotation, &Transform::setLocalRotation),
	LocalScale(this, &Transform::getLocalScale, &Transform::setLocalScale),

	Position(this, &Transform::getPosition, &Transform::setPosition),
	Euler(this, &Transform::getEuler, &Transform::setEuler),
	Rotation(this, &Transform::getRotation, &Transform::setRotation),
	Scale(this, &Transform::getScale, &Transform::setScale) {}
void Transform::operator=(const Transform& v) {}

///////////////////////////////////////////////////////
//
//
//
// Node Hierarchy structure and operations
//
//
//
///////////////////////////////////////////////////////

Transform* Transform::removeChild(int index) {
    if (index >= children.size() || index < 0)
    {
        fprintf(stderr, "Trying to remove a child that is not in the list...\n");
        exit(-1);
        return NULL;
    }
    Transform* node = children[index];
    children.erase(children.begin() + index);
    return node;
}
Transform* Transform::removeChild(Transform * transform) {
    for (int i=0;i< children.size();i++)
        if (children[i] == transform) {
            children.erase(children.begin() + i);
            transform->parent = NULL;
            transform->visited = false;
            return transform;
        }
    fprintf(stderr,"Trying to remove a child that is not in the scene...\n");
    exit(-1);
    return NULL;
}

Transform* Transform::addChild(Transform * transform) {
    if (transform->parent != NULL)
        transform->parent->removeChild(transform);
    transform->parent = this;
    transform->visited = false;
    children.push_back(transform);
    return transform;
}

std::vector<Transform*> &Transform::getChildren() {
    return children;
}

Transform *Transform::getParent() {
    return parent;
}

void Transform::setParent(Transform *prnt) {
    if (prnt == NULL) {
        fprintf(stderr,"Trying to set parent to NULL...\n");
        exit(-1);
    }
    if (parent != NULL)
        parent->removeChild(this);
    parent = prnt;
    if (parent != NULL)
        parent->addChild(prnt);
    
}

bool Transform::isRoot() {
    return parent == NULL;
}
///////////////////////////////////////////////////////
//
//
//
// Transform default graph operations
//
//
//
///////////////////////////////////////////////////////
void Transform::updateLocalRotationBase() {
    if (!localRotationBaseDirty)
        return;
    localRotationBase = toMat4(localRotation);
    localRotationBaseDirty = false;
}

vec3 Transform::getLocalPosition()const{
    return localPosition;
}
vec3 Transform::getLocalEuler()const{
    return localEuler;
}
quat Transform::getLocalRotation()const{
    return localRotation;
}
vec3 Transform::getLocalScale()const{
    return localScale;
}

void Transform::setLocalPosition( const vec3 &p ){
    if (localPosition == p)
        return;
    
    localPosition = p;
    
    //if (!localRotationBaseDirty) localRotationBaseDirty = true;
    if (!localMatrixDirty) localMatrixDirty = true;
    if (!localMatrixInverseTransposeDirty) localMatrixInverseTransposeDirty = true;
    if (!localMatrixInverseDirty) localMatrixInverseDirty = true;
}
void Transform::setLocalEuler( const vec3 &e){
    if (localEuler == e)
        return;
    
    localEuler = e;
    if (localEuler.x < 0)
        localEuler.x += ceil( - localEuler.x / DEG2RAD(360.0f)) * DEG2RAD(360.0f);
    if (localEuler.x > DEG2RAD(360.0f))
        localEuler.x = fmod(localEuler.x, DEG2RAD(360.0f));
    
    if (localEuler.y < 0)
        localEuler.y += ceil(-localEuler.y / DEG2RAD(360.0f)) * DEG2RAD(360.0f);
    if (localEuler.y > DEG2RAD(360.0f))
        localEuler.y = fmod(localEuler.y, DEG2RAD(360.0f));
    
    if (localEuler.z < 0)
        localEuler.z += ceil(-localEuler.z / DEG2RAD(360.0f)) * DEG2RAD(360.0f);
    if (localEuler.z > DEG2RAD(360.0f))
        localEuler.z = fmod(localEuler.z, DEG2RAD(360.0f));
    
    localRotation = quatFromEuler(localEuler.x, localEuler.y, localEuler.z);
    
    if (!localRotationBaseDirty) localRotationBaseDirty = true;
    if (!localMatrixDirty) localMatrixDirty = true;
    if (!localMatrixInverseTransposeDirty) localMatrixInverseTransposeDirty = true;
    if (!localMatrixInverseDirty) localMatrixInverseDirty = true;
}
void Transform::setLocalRotation( const quat &q ){
    if (localRotation == q)
        return;
    
    localRotation = q;
    
    extractEuler(localRotation, &localEuler.x, &localEuler.y, &localEuler.z);
    
    if (!localRotationBaseDirty) localRotationBaseDirty = true;
    if (!localMatrixDirty) localMatrixDirty = true;
    if (!localMatrixInverseTransposeDirty) localMatrixInverseTransposeDirty = true;
    if (!localMatrixInverseDirty) localMatrixInverseDirty = true;
}
void Transform::setLocalScale(const vec3 &s){
    if (localScale == s)
        return;
    
    localScale = s;
    
    //if (!localRotationBaseDirty) localRotationBaseDirty = true;
    if (!localMatrixDirty) localMatrixDirty = true;
    if (!localMatrixInverseTransposeDirty) localMatrixInverseTransposeDirty = true;
    if (!localMatrixInverseDirty) localMatrixInverseDirty = true;
}


mat4 & Transform::getLocalMatrix(){
    if (localMatrixDirty) {
        //
        // compute the matrix transform
        //
        updateLocalRotationBase();
        
        localMatrix = localRotationBase;
        
        localMatrix.a1 *= localScale.x;
        localMatrix.a2 *= localScale.x;
        localMatrix.a3 *= localScale.x;
        
        localMatrix.b1 *= localScale.y;
        localMatrix.b2 *= localScale.y;
        localMatrix.b3 *= localScale.y;
        
        localMatrix.c1 *= localScale.z;
        localMatrix.c2 *= localScale.z;
        localMatrix.c3 *= localScale.z;
        
        localMatrix.d1 = localPosition.x;
        localMatrix.d2 = localPosition.y;
        localMatrix.d3 = localPosition.z;
        
        localMatrixDirty = false;
        matrixDirty = true;
        //renderDirty = true;
    }
    return localMatrix;
}

mat4 & Transform::getLocalMatrixInverseTranspose(){
    if (localMatrixInverseTransposeDirty){
        updateLocalRotationBase();
        
        localMatrixInverseTranspose = localRotationBase;
        
        vec3 t_inv = -localPosition;
        localMatrixInverseTranspose.a4 = localMatrixInverseTranspose.a1 * t_inv.x + localMatrixInverseTranspose.a2 * t_inv.y + localMatrixInverseTranspose.a3 * t_inv.z;
        localMatrixInverseTranspose.b4 = localMatrixInverseTranspose.b1 * t_inv.x + localMatrixInverseTranspose.b2 * t_inv.y + localMatrixInverseTranspose.b3 * t_inv.z;
        localMatrixInverseTranspose.c4 = localMatrixInverseTranspose.c1 * t_inv.x + localMatrixInverseTranspose.c2 * t_inv.y + localMatrixInverseTranspose.c3 * t_inv.z;
        
        localMatrixInverseTranspose[0] *= 1.0f / localScale.x;
        localMatrixInverseTranspose[1] *= 1.0f / localScale.y;
        localMatrixInverseTranspose[2] *= 1.0f / localScale.z;
        
        localMatrixInverseTransposeDirty = false;
        matrixInverseTransposeDirty = true;
        //renderDirty = true;
    }
    return localMatrixInverseTranspose;
}

mat4 & Transform::getLocalMatrixInverse(){
    if (localMatrixInverseDirty) {
        updateLocalRotationBase();
        
        localMatrixInverse = transpose(localRotationBase);
        
        vec3 s_inv = 1.0f / localScale;
        
        localMatrixInverse.a1 *= s_inv.x;
        localMatrixInverse.a2 *= s_inv.y;
        localMatrixInverse.a3 *= s_inv.z;
        
        localMatrixInverse.b1 *= s_inv.x;
        localMatrixInverse.b2 *= s_inv.y;
        localMatrixInverse.b3 *= s_inv.z;
        
        localMatrixInverse.c1 *= s_inv.x;
        localMatrixInverse.c2 *= s_inv.y;
        localMatrixInverse.c3 *= s_inv.z;
        
        vec3 t_inv = -localPosition;
        
        localMatrixInverse.d1 = localMatrixInverse.a1*t_inv.x + localMatrixInverse.b1*t_inv.y + localMatrixInverse.c1*t_inv.z;
        localMatrixInverse.d2 = localMatrixInverse.a2*t_inv.x + localMatrixInverse.b2*t_inv.y + localMatrixInverse.c2*t_inv.z;
        localMatrixInverse.d3 = localMatrixInverse.a3*t_inv.x + localMatrixInverse.b3*t_inv.y + localMatrixInverse.c3*t_inv.z;
        
        localMatrixInverseDirty = false;
        matrixInverseDirty = true;
        //renderDirty = true;
    }
    return localMatrixInverse;
}



///////////////////////////////////////////////////////
//
//
//
// Transform Global Ops...
//
//
//
///////////////////////////////////////////////////////
mat4& Transform::getMatrix(bool useVisitedFlag){
    
    if (useVisitedFlag && visited)
        return matrix;
    
    stat_num_recalculated++;
    
    mat4 &localM = getLocalMatrix();
    Transform* parent = getParent();
    if (parent != NULL && !parent->isRoot()) {
        mat4 &aux = parent->getMatrix(useVisitedFlag);
        if (matrixParent != aux) {
            matrixParent = aux;
            matrixDirty = true;
        }
        if (matrixDirty) {
            matrix = matrixParent * localM;
            matrixDirty = false;
            //renderDirty = true;
        }
        return matrix;
    } else {
        if (matrixDirty) {
            matrix = localM;
            matrixDirty = false;
            //renderDirty = true;
        }
        return matrix;
    }
}

mat4& Transform::getMatrixInverseTranspose(bool useVisitedFlag){
    
    if (useVisitedFlag && visited)
        return matrixInverseTranspose;
    
    mat4 &localM = getLocalMatrixInverseTranspose();
    Transform* parent = getParent();
    if (parent != NULL && !parent->isRoot()) {
        mat4 &aux = parent->getMatrixInverseTranspose(useVisitedFlag);
        if (matrixInverseTransposeParent != aux) {
            matrixInverseTransposeParent = aux;
            matrixInverseTransposeDirty = true;
        }
        if (matrixInverseTransposeDirty) {
            matrixInverseTranspose = matrixInverseTransposeParent * localM;
            matrixInverseTransposeDirty = false;
            //renderDirty = true;
        }
        return matrixInverseTranspose;
    } else{
        if (matrixInverseTransposeDirty) {
            matrixInverseTranspose = localM;
            matrixInverseTransposeDirty = false;
            //renderDirty = true;
        }
        return matrixInverseTranspose;
    }
}

mat4& Transform::getMatrixInverse(bool useVisitedFlag){
    
    if (useVisitedFlag && visited)
        return matrixInverse;
    
    mat4 &localM = getLocalMatrixInverse();
    Transform* parent = getParent();
    if (parent != NULL && !parent->isRoot()) {
        mat4 &aux = parent->getMatrixInverse(useVisitedFlag);
        if (matrixInverseParent != aux) {
            matrixInverseParent = aux;
            matrixInverseDirty = true;
        }
        if (matrixInverseDirty) {
            matrixInverse = localM * matrixInverseParent;
            matrixInverseDirty = false;
            //renderDirty = true;
        }
        return matrixInverse;
    } else {
        if (matrixInverseDirty) {
            matrixInverse = localM;
            matrixInverseDirty = false;
            //renderDirty = true;
        }
        return matrixInverse;
    }
}


void Transform::setPosition(const vec3 &pos) {
    setLocalPosition( toVec3(getMatrixInverse() * toVec4(pos - localPosition)) );
}

vec3 Transform::getPosition(bool useVisitedFlag) {
    return toVec3(getMatrix(useVisitedFlag)[3]);
}

vec3 Transform::getPosition() {
	return toVec3(getMatrix(false)[3]);
}

void Transform::setEuler(const vec3 &rot) {
	quat q = extractQuat(getMatrixInverse());
	setLocalRotation(q * quatFromEuler(rot.x, rot.y, rot.z) * inv(localRotation));
}

vec3 Transform::getEuler() {
	vec3 euler;
	extractEuler(getMatrix(false), &euler.x, &euler.y, &euler.z);
	return euler;
}

vec3 Transform::getEuler(bool useVisitedFlag = false) {
	vec3 euler;
	extractEuler(getMatrix(useVisitedFlag), &euler.x, &euler.y, &euler.z);
	return euler;
}

void Transform::setRotation(const quat &rot) {
    quat q = extractQuat(getMatrixInverse());
    setLocalRotation(  q * rot * inv( localRotation ) );
}

quat Transform::getRotation() {
	quat q = extractQuat(getMatrix(false));
	return q;
}

quat Transform::getRotation(bool useVisitedFlag) {
    quat q = extractQuat(getMatrix(useVisitedFlag));
    return q;
}

void Transform::setScale(const vec3 &s) {
    mat4 &m = getMatrixInverse();
    vec3 newScale = vec3( length( toVec3(m[0]) ) * s.x, length( toVec3(m[1]) ) * s.y, length( toVec3(m[2]) ) * s.z );
    setLocalScale( newScale );
}

vec3 Transform::getScale() {
	mat4 &m = getMatrix(false);
	return vec3(length(toVec3(m[0])), length(toVec3(m[1])), length(toVec3(m[2])));
}

vec3 Transform::getScale(bool useVisitedFlag) {
    mat4 &m = getMatrix(useVisitedFlag);
    return vec3( length( toVec3(m[0]) ), length( toVec3(m[1]) ), length( toVec3(m[2]) ) );
}


///////////////////////////////////////////////////////
//
//
//
// Transform Render Ops...
//
//
//
///////////////////////////////////////////////////////
void Transform::resetVisited(bool forceMarkFalse){
    if (forceMarkFalse ||
        localMatrixDirty ||
        localMatrixInverseTransposeDirty ||
        localMatrixInverseDirty ||
        matrixDirty ||
        matrixInverseTransposeDirty ||
        matrixInverseDirty){
        visited = false;
        forceMarkFalse = true;
        renderDirty = true;
    }
    for (int i = 0; i < children.size(); i++) {
        children[i]->resetVisited(forceMarkFalse);
    }
}

void Transform::preComputeTransforms(){
    if (!visited) {
        if (!isRoot()){
            getMatrix(true);
            getMatrixInverseTranspose(true);
            getMatrixInverse(true);
        }
        visited = true;
        stat_num_visited++;
    }
    for (int i = 0; i < children.size(); i++) {
        children[i]->preComputeTransforms();
    }
}

void Transform::computeRenderMatrix(const mat4 &viewProjection,
                         const mat4 &view,
                         const mat4 &viewIT,
                         const mat4 &viewInv,
                         mat4 *mvp,
                         mat4 *mv,
                         mat4 *mvIT,
                         mat4 *mvInv){
    if (renderDirty || viewProjection != renderViewProjection){
        
        stat_draw_recalculated++;
        
        renderViewProjection = viewProjection;
        renderMVP = viewProjection * getMatrix(true);
        renderMV = view * getMatrix(true);
        renderMVIT = viewIT * getMatrixInverseTranspose(true);
        renderMVInv = getMatrixInverse(true) * viewInv;
        
        renderDirty = false;
    }
    
    *mvp = renderMVP;
    *mv = renderMV;
    *mvIT = renderMVIT;
    *mvInv = renderMVInv;
}

///////////////////////////////////////////////////////
//
//
//
// Object References...
//
//
//
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//
//
//
// Default Constructor
//
//
//
///////////////////////////////////////////////////////

Transform::Transform():

	LocalPosition(this,&Transform::getLocalPosition,&Transform::setLocalPosition),
	LocalEuler(this, &Transform::getLocalEuler, &Transform::setLocalEuler),
	LocalRotation(this, &Transform::getLocalRotation, &Transform::setLocalRotation),
	LocalScale(this, &Transform::getLocalScale, &Transform::setLocalScale),

	Position(this, &Transform::getPosition, &Transform::setPosition),
	Euler(this, &Transform::getEuler, &Transform::setEuler),
	Rotation(this, &Transform::getRotation, &Transform::setRotation),
	Scale(this, &Transform::getScale, &Transform::setScale)
{
    //hierarchy ops
    parent = NULL;
    //transform
    localPosition = vec3(0);
    localEuler = vec3(0);
    localRotation = quat();
    localScale = vec3(1.0f);
    
    localRotationBase = mat4::IdentityMatrix;
    localMatrix = mat4::IdentityMatrix;
    localMatrixInverseTranspose = mat4::IdentityMatrix;
    localMatrixInverse = mat4::IdentityMatrix;
    
    localRotationBaseDirty = false;
    localMatrixDirty = false;
    localMatrixInverseTransposeDirty = false;
    localMatrixInverseDirty = false;
    
    matrixDirty = false;
    matrix = mat4::IdentityMatrix;
    matrixParent = mat4::IdentityMatrix;
    matrixInverseTransposeDirty = false;
    matrixInverseTranspose = mat4::IdentityMatrix;
    matrixInverseTransposeParent = mat4::IdentityMatrix;
    matrixInverseDirty = false;
    matrixInverse = mat4::IdentityMatrix;
    matrixInverseParent = mat4::IdentityMatrix;
    
    model = NULL;
    
    visited = true;
    
    renderMVP = mat4::IdentityMatrix;
    renderMV = mat4::IdentityMatrix;
    renderMVIT = mat4::IdentityMatrix;
    renderMVInv = mat4::IdentityMatrix;
    renderViewProjection = mat4::IdentityMatrix;
    
    renderDirty = true;
}

Transform::~Transform(){
    
}

