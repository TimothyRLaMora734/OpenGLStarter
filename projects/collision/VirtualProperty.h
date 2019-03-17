#ifndef Virtual_Property__H
#define Virtual_Property__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

template <typename T>
class VirtualProperty {

	// avoid copy, using copy constructors
	VirtualProperty(const VirtualProperty&) {}
	void operator=(const VirtualProperty&) {}
	
	DelegateFriendObject* thiz;
	T (DelegateFriendObject::*getConst)(void) const;
	T(DelegateFriendObject::*getNormal)(void);
	void (DelegateFriendObject::*set)(const T&);
	void (DelegateFriendObject::*setConst)(const T&) const;
	
public:

	template <typename C>
	VirtualProperty(C* obj, T (C::*get)(void) const,  void (C::*set)(const T&)) {
		this->thiz = (DelegateFriendObject*)(void*)obj;
		this->getConst = (T(DelegateFriendObject::*)(void) const)get;
		this->set = (void (DelegateFriendObject::*)(const T&))set;
		this->getNormal = NULL;
		this->setConst = NULL;
	}

	template <typename C>
	VirtualProperty(C* obj, T(C::*get)(void), void (C::*set)(const T&)) {
		this->thiz = (DelegateFriendObject*)(void*)obj;
		this->getNormal = (T(DelegateFriendObject::*)(void))get;
		this->set = (void (DelegateFriendObject::*)(const T&))set;
		this->getConst = NULL;
		this->setConst = NULL;
	}

	template <typename C>
	VirtualProperty(C* obj, T(C::*get)(void) const, void (C::*set)(const T&) const) {
		this->thiz = (DelegateFriendObject*)(void*)obj;
		this->getConst = (T(DelegateFriendObject::*)(void) const)get;
		this->set = NULL;
		this->getNormal = NULL;
		this->setConst = (void (DelegateFriendObject::*)(const T&) const)set;
	}

	template <typename C>
	VirtualProperty(C* obj, T(C::*get)(void), void (C::*set)(const T&) const) {
		this->thiz = (DelegateFriendObject*)(void*)obj;
		this->getNormal = (T(DelegateFriendObject::*)(void))get;
		this->set = NULL;
		this->getConst = NULL;
		this->setConst = (void (DelegateFriendObject::*)(const T&) const)set;
	}

	void operator=(const T &param) const {
		if (setConst != NULL)
			(thiz->*setConst)(param);
		else
			(thiz->*set)(param);
	}

	operator T() const {
		if (getConst != NULL)
			return (thiz->*getConst)();
		else
			return (thiz->*getNormal)();
	}

};

#endif
