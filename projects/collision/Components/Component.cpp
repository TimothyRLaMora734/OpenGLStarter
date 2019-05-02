#include "Component.h"

#include "../Transform.h"

//std::vector<Component *> componentList;
StartEventManager* StartEventManager::getInstance(){
    static StartEventManager startEventManager;
    return &startEventManager;
}

void StartEventManager::registerNewComponent(Component *c){
    componentList.push_back(c);
}
void StartEventManager::unregisterComponent(Component *c){
    for(int i=componentList.size()-1;i>=0;i--){
        if (componentList[i] == c){
            componentList.erase(componentList.begin()+i);
            return;
        }
    }
}
void StartEventManager::processAllComponentsWithTransform(){
    for(int i=componentList.size()-1;i>=0;i--){
        if (componentList[i]->transform != NULL){
            componentList[i]->start();
            componentList.erase(componentList.begin()+i);
        }
    }
}





Component::Component(const Component& v){}
void Component::operator=(const Component& v){}

Component::Component(ComponentType type){
    this->type = type;
    transform = NULL;
    //mStartCalled = false;
    StartEventManager::getInstance()->registerNewComponent(this);
}

ComponentType Component::getType() const {
    return type;
}

Component::~Component() {
	SharedPointerDatabase::getInstance()->notifyDeletion(this);
    StartEventManager::getInstance()->unregisterComponent(this);
}

void Component::start(){
}

//void Component::callStartOnce() {
//    if (mStartCalled)
//        return;
//    mStartCalled = true;
//    start();
//}

void Component::attachToTransform(Transform *t) {
    
}

void Component::detachFromTransform(Transform *t) {
    
}
