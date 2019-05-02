#include "Transform.h"


int stat_num_visited;
int stat_num_recalculated;
int stat_draw_recalculated;


Transform::Transform(const Transform& v) :
    Parent(this,&Transform::getParent, &Transform::setParent),

	LocalPosition(this, &Transform::getLocalPosition, &Transform::setLocalPosition),
	LocalEuler(this, &Transform::getLocalEuler, &Transform::setLocalEuler),
	LocalRotation(this, &Transform::getLocalRotation, &Transform::setLocalRotation),
	LocalScale(this, &Transform::getLocalScale, &Transform::setLocalScale),

	Position(this, &Transform::getPosition, &Transform::setPosition),
	Euler(this, &Transform::getEuler, &Transform::setEuler),
	Rotation(this, &Transform::getRotation, &Transform::setRotation),
	Scale(this, &Transform::getScale, &Transform::setScale),
    Name(this,&Transform::getName,&Transform::setName) {}
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
    
    //removeMapName(node);
    
    return node;
}
Transform* Transform::removeChild(Transform * transform) {
    for (int i=0;i< children.size();i++)
        if (children[i] == transform) {
            children.erase(children.begin() + i);
            transform->parent = NULL;
            transform->visited = false;
            
            //removeMapName(transform);
            
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
    
    //insertMapName(transform);
    
    return transform;
}

//std::vector<Transform*> &Transform::getChildren() {
//    return children;
//}

int Transform::getChildCount() {
    return children.size();
}

Transform* Transform::getChildAt(int i) {
    if (i>=0 && i<children.size())
        return children[i];
    return NULL;
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
    //parent = prnt;
    if (prnt != NULL)
        prnt->addChild(this);
    
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
    
    /*
     Unity Euler angles are: yxz
     
    localRotation =
    quatFromAxisAngle(vec3(0.0, 1.0, 0.0), localEuler.y) *
    quatFromAxisAngle(vec3(1.0, 0.0, 0.0), localEuler.x) *
    quatFromAxisAngle(vec3(0.0, 0.0, 1.0), localEuler.z);
     */
    
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
        
        //localMatrixInverse = inv( getLocalMatrix() );
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
    if (parent != NULL && !parent->isRoot())
        setLocalPosition( toVec3(parent->getMatrixInverse() * toPtn4(pos)) );
    else
        setLocalPosition( pos );
}

vec3 Transform::getPosition(bool useVisitedFlag) {
    return toVec3(getMatrix(useVisitedFlag)[3]);
}

vec3 Transform::getPosition() {
	return getPosition(false);
}

void Transform::setEuler(const vec3 &rot) {
    if (parent != NULL && parent->isRoot())
        setLocalEuler( rot );
    
    quat q = extractQuat(parent->getMatrixInverse());
    setLocalRotation( q * quatFromEuler(rot.x, rot.y, rot.z) );
}

vec3 Transform::getEuler() {
    return getEuler(false);
}

vec3 Transform::getEuler(bool useVisitedFlag = false) {
    if (parent != NULL && parent->isRoot())
        return localEuler;
    
    vec3 euler;
    extractEuler(getMatrix(useVisitedFlag), &euler.x, &euler.y, &euler.z);
    return euler;
}

void Transform::setRotation(const quat &rot) {
    if (parent != NULL && parent->isRoot())
        setLocalRotation( rot );
        
    quat q = extractQuat(parent->getMatrixInverse());
    setLocalRotation( q * rot );
}

quat Transform::getRotation() {
    return getRotation(false);
}

quat Transform::getRotation(bool useVisitedFlag) {
    if (parent != NULL && parent->isRoot())
        return localRotation;
    
    quat q = extractQuat(getMatrix(useVisitedFlag));
    return q;
}

void Transform::setScale(const vec3 &s) {
    if (parent != NULL && parent->isRoot())
        setLocalScale( s );
    
    mat4 &m = parent->getMatrixInverse();
    setLocalScale( vec3( length( toVec3(m[0]) ) * s.x, length( toVec3(m[1]) ) * s.y, length( toVec3(m[2]) ) * s.z ) );
}

vec3 Transform::getScale() {
    return getScale(false);
}

vec3 Transform::getScale(bool useVisitedFlag) {
    if (parent != NULL && parent->isRoot())
        return localScale;
    
    mat4 &m = getMatrix(useVisitedFlag);
    return vec3(
                length( vec3(m.a1,m.a2,m.a3) ),
                length( vec3(m.b1,m.b2,m.b3) ),
                length( vec3(m.c1,m.c2,m.c3) ) );
}

void Transform::lookAtRightHanded(const Transform* to, const vec3 &worldUp) {
    vec3 lookVector = (vec3)to->Position - (vec3)Position;
    Rotation = quatLookAtRotation(lookVector, worldUp);
}

void Transform::lookAtLeftHanded(const Transform* to, const vec3 &worldUp) {
    vec3 lookVector = (vec3)to->Position - (vec3)Position;
    Rotation = quatLookAtRotationLH(lookVector, worldUp);
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
        
        OnVisited(this);
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


mat4& Transform::worldToLocalMatrix(bool useVisitedFlag) {
    return getMatrixInverse(useVisitedFlag);
}

mat4& Transform::localToWorldMatrix(bool useVisitedFlag) {
    return getMatrix(useVisitedFlag);
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

/*
void Transform::insertMapName(Transform *node){
    
    name2children[node->name].push_back(node);
    
}

void Transform::removeMapName(Transform *node){
    std::map<std::string,std::vector<Transform*> >::iterator it;
    it = name2children.find(node->name);
    if (it != name2children.end()){
        std::vector<Transform*> &vector = it->second;
        for(int j=0;j<vector.size();j++){
            if (vector[j] == node){
                vector.erase(vector.begin()+j);
                break;
            }
        }
        if (vector.size() == 0)
            name2children.erase(it);
    }
}
*/

Component* Transform::addComponent(Component*c){
    components.push_back(c);
    c->transform = this;
    c->attachToTransform(this);
    return c;
}

Component* Transform::removeComponent(Component*c){
    for(int i=0;i<components.size();i++) {
        if (components[i] == c){
            components.erase(components.begin()+i);
            Transform *t = c->transform;
            c->transform = NULL;
            if (t != NULL)
                c->detachFromTransform(t);
            return c;
        }
    }
    return NULL;
}

Component* Transform::removeComponentAt(int i) {
    if (i>=0&&i<components.size()){
        Component * result = components[i];
        components.erase(components.begin()+i);
        Transform *t = result->transform;
        result->transform = NULL;
        if (t != NULL)
            result->detachFromTransform(t);
        return result;
    }
    return NULL;
}

Component* Transform::findComponent(ComponentType t)const{
    for(int i=0;i<components.size();i++) {
        if (components[i]->getType() == t){
            return components[i];
        }
    }
    return NULL;
}

std::vector<Component*> Transform::findComponents(ComponentType t) const{
    std::vector<Component*> result;
    for(int i=0;i<components.size();i++) {
        if (components[i]->getType() == t){
            result.push_back(components[i]);
        }
    }
    return result;
}

int Transform::getComponentCount()const{
    return components.size();
}
Component* Transform::getComponentAt(int i){
    if (i>=0&&i<components.size())
        return components[i];
    return NULL;
}


Component* Transform::findComponentInChildren(ComponentType t)const{
    Component* result = NULL;
    for(int i=0;i<children.size();i++){
        result = children[i]->findComponent(t);
        if (result != NULL)
            return result;
        result = children[i]->findComponentInChildren(t);
        if (result != NULL)
            return result;
    }
    return result;
}

std::vector<Component*> Transform::findComponentsInChildren(ComponentType t)const{
    std::vector<Component*> result;
    std::vector<Component*> parcialResult;
    for(int i=0;i<children.size();i++){
        parcialResult = children[i]->findComponents(t);
        if (parcialResult.size()>0)
            result.insert(result.end(), parcialResult.begin(), parcialResult.end());
        parcialResult = children[i]->findComponentsInChildren(t);
        if (parcialResult.size()>0)
            result.insert(result.end(), parcialResult.begin(), parcialResult.end());
    }
    return result;
}

void Transform::setName(const std::string &p){
    if (name == p)
        return;
    
    //if (this->parent != NULL)
        //this->parent->removeMapName(this);
    name = p;
    //if (this->parent != NULL)
        //this->parent->insertMapName(this);
}
const std::string& Transform::getName()const{
    return name;
}

//VirtualProperty<std::string> Name;

Transform * Transform::findTransformByName(const std::string &name, int maxLevel) {
    if (this->name == name)
        return this;
    if (maxLevel > 0){
        Transform * result;
        for(int i=0;i<children.size();i++){
            result = children[i]->findTransformByName(name,maxLevel-1);
            if (result != NULL)
                return result;
        }
    }
    return NULL;
}


std::vector<Transform*> Transform::findTransformsByName(const std::string &name, int maxLevel) {
    std::vector<Transform*> result;
    std::vector<Transform*> parcialResult;
    
    if (this->name == name)
        result.push_back(this);
    
    if (maxLevel > 0){
        for(int i=0;i<children.size();i++){
            parcialResult = children[i]->findTransformsByName(name,maxLevel-1);
            if (parcialResult.size()>0)
                result.insert(result.end(), parcialResult.begin(), parcialResult.end());
        }
    }
    
    return result;
}


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
    Parent(this,&Transform::getParent, &Transform::setParent),
	LocalPosition(this,&Transform::getLocalPosition,&Transform::setLocalPosition),
	LocalEuler(this, &Transform::getLocalEuler, &Transform::setLocalEuler),
	LocalRotation(this, &Transform::getLocalRotation, &Transform::setLocalRotation),
	LocalScale(this, &Transform::getLocalScale, &Transform::setLocalScale),

	Position(this, &Transform::getPosition, &Transform::setPosition),
	Euler(this, &Transform::getEuler, &Transform::setEuler),
	Rotation(this, &Transform::getRotation, &Transform::setRotation),
	Scale(this, &Transform::getScale, &Transform::setScale),

    Name(this,&Transform::getName,&Transform::setName)
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
    
    //model = NULL;
    
    visited = true;
    
    renderMVP = mat4::IdentityMatrix;
    renderMV = mat4::IdentityMatrix;
    renderMVIT = mat4::IdentityMatrix;
    renderMVInv = mat4::IdentityMatrix;
    renderViewProjection = mat4::IdentityMatrix;
    
    renderDirty = true;
}

Transform::~Transform(){
	SharedPointerDatabase::getInstance()->notifyDeletion(this);
}

