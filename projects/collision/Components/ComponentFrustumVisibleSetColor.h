#ifndef ComponentFrustumVisibleSetColor__H
#define ComponentFrustumVisibleSetColor__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;


#include "Component.h"
#include "ComponentFrustumCulling.h"
#include "ComponentMesh.h"

class ComponentFrustumVisibleSetColor : public Component {
public:

	SharedPointer<ComponentColorMeshVBO> vboPtr;
	SharedPointer<ComponentFrustumCulling> frustumPtr;

	ComponentFrustumVisibleSetColor();
	~ComponentFrustumVisibleSetColor();
	void start();

	void OnBecomeVisible(Component *f, Component *camera);
	void OnBecomeInvisible(Component *f, Component *camera);

};


#endif