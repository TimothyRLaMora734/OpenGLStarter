#include "Component.h"

#include "../Transform.h"

Component::Component(const Component& v){}
void Component::operator=(const Component& v){}

Component::Component(ComponentType type){
    this->type = type;
    transform = NULL;
    mStartCalled = false;
}

ComponentType Component::getType() const {
    return type;
}

Component::~Component() {
	SharedPointerDatabase::getInstance()->notifyDeletion(this);
}

void Component::start(){
}

void Component::callStartOnce() {
    if (mStartCalled)
        return;
    mStartCalled = true;
    start();
}

