#include "Component.h"

#include "../Transform.h"

Component::Component(const Component& v){}
void Component::operator=(const Component& v){}

Component::Component(ComponentType type){
    this->type = type;
    transform = NULL;
}

ComponentType Component::getType() const {
    return type;
}

Component::~Component() {
}

