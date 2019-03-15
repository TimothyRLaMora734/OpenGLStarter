#ifndef Transform__H
#define Transform__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

class TrianglesModel;

extern int stat_num_visited;
extern int stat_num_recalculated;
extern int stat_draw_recalculated;

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
    std::vector<Transform*> &getChildren();
    Transform *getParent();
    void setParent(Transform *prnt);
    bool isRoot();
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
    
    void setLocalPosition( const vec3 &p );
    void setLocalEuler( const vec3 &e);
    void setLocalRotation( const quat &q );
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
    mat4 matrixInverseTranspose, matrixInverseTransposeParent;
    bool matrixInverseDirty;
    mat4 matrixInverse, matrixInverseParent;
    
public:
    mat4& getMatrix(bool useVisitedFlag = false);
    mat4& getMatrixInverseTranspose(bool useVisitedFlag = false);
    mat4& getMatrixInverse(bool useVisitedFlag = false);
    
    void setPosition(const vec3 &pos);
    vec3 getPosition(bool useVisitedFlag = false);
    void setRotation(const quat &rot);
    quat getRotation(bool useVisitedFlag = false);
    void setScale(const vec3 &s);
    vec3 getScale(bool useVisitedFlag = false);
    
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
    
    mat4 renderMVP,renderMV,renderMVIT,renderMVInv, renderViewProjection;
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
public:
    TrianglesModel *model;
    
    Transform();
    virtual ~Transform();
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
