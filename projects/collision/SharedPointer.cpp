#include "SharedPointer.h"

SharedPointerBase::SharedPointerBase() {
	reference = NULL;
}

SharedPointerBase::~SharedPointerBase() {
	setNULL();
}

bool SharedPointerBase::isNULL() {
	return reference == NULL;
}


void SharedPointerBase::setNULL(bool unregister) {
	if (unregister)
		SharedPointerDatabase::getInstance()->unregisterPointer(this, reference);
	reference = NULL;
}


SharedPointerDatabase* SharedPointerDatabase::getInstance() {
	static SharedPointerDatabase db;
	return &db;
}

void SharedPointerDatabase::notifyDeletion(void* ref) {
	std::map <void *, std::vector<SharedPointerBase*> >::iterator it = refs.find(ref);
	if (it != refs.end()) {
		std::vector<SharedPointerBase*> &pointers = it->second;
		for (int i = 0; i < pointers.size(); i++)
			pointers[i]->setNULL(false);
		refs.erase(it);
	}
}

void SharedPointerDatabase::registerPointer(SharedPointerBase *pointer, void *ref) {
	if (ref == NULL || pointer == NULL)
		return;
	refs[ref].push_back(pointer);
}

void SharedPointerDatabase::unregisterPointer(SharedPointerBase *pointer, void *ref) {
	if (ref == NULL || pointer == NULL)
		return;
	std::map <void *, std::vector<SharedPointerBase*> >::iterator it = refs.find(ref);
	if (it != refs.end()) {
		std::vector<SharedPointerBase*> &pointers = it->second;
		for (int i = 0; i < pointers.size(); i++) {
			if (pointers[i] == pointer) {
				pointers.erase(pointers.begin() + i);

				if (pointers.size() == 0)
					refs.erase(it);
				return;
			}
		}
	}
}
