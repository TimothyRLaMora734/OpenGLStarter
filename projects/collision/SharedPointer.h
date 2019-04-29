#ifndef SharedPointer__H
#define SharedPointer__H

#include <stdlib.h>
#include <map>
#include <vector>

class SharedPointerBase {
protected:
	void* reference;
	SharedPointerBase();
public:
	virtual ~SharedPointerBase();
	virtual bool isNULL();
	virtual void setNULL(bool unregister = true);
};

class SharedPointerDatabase {
	std::map <void *, std::vector<SharedPointerBase*> > refs;
public:
	static SharedPointerDatabase* getInstance();

	void notifyDeletion(void* ref);
	void registerPointer(SharedPointerBase *pointer, void *ref);
	void unregisterPointer(SharedPointerBase *pointer, void *ref);

};

template <typename T>
class SharedPointer : public SharedPointerBase {
protected:

public:

	SharedPointer():SharedPointerBase(){
	}
	
	bool operator==(T* v) {
		return v == reference;
	}
	
	void operator=(T* v) {
		SharedPointerDatabase::getInstance()->unregisterPointer(this, reference);
		SharedPointerDatabase::getInstance()->registerPointer(this, (void*)v);
		reference = (void*)v;
	}
	
	operator T*() {
		return (T*)reference;
	}
	
	T* operator->() {
		return (T*)reference;
	}
};

#endif
