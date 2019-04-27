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
    
    //get
    typedef T (DelegateFriendObject::*VirtualProperty_get_pattern1)(void);
    typedef const T& (DelegateFriendObject::*VirtualProperty_get_pattern2)(void);
    typedef T (DelegateFriendObject::*VirtualProperty_get_pattern3)(void) const;
    typedef const T& (DelegateFriendObject::*VirtualProperty_get_pattern4)(void) const;
    
    
    VirtualProperty_get_pattern1 get_pattern1;
    VirtualProperty_get_pattern2 get_pattern2;
    VirtualProperty_get_pattern3 get_pattern3;
    VirtualProperty_get_pattern4 get_pattern4;
    
    //set
    typedef void (DelegateFriendObject::*VirtualProperty_set_pattern1)(const T&);
    typedef void (DelegateFriendObject::*VirtualProperty_set_pattern2)(T);
	//typedef void (DelegateFriendObject::*VirtualProperty_set_pattern2)(const T&) const;
    
    VirtualProperty_set_pattern1 set_pattern1;
    VirtualProperty_set_pattern2 set_pattern2;
    //VirtualProperty_set_pattern2 set_pattern2;
    
    void initializePointers(){
        get_pattern1 = NULL;
        get_pattern2 = NULL;
        get_pattern3 = NULL;
        get_pattern4 = NULL;
        
        set_pattern1 = NULL;
        //set_pattern2 = NULL;
    }
	
public:
    
    template <typename C>
    VirtualProperty(C* obj, T(C::*get)(void), void (C::*set)(const T&)) {
        this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern1 = (VirtualProperty_get_pattern1)get;
        set_pattern1 = (VirtualProperty_set_pattern1)set;
    }
    
    template <typename C>
    VirtualProperty(C* obj, const T& (C::*get)(void),  void (C::*set)(const T&)) {
        this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern2 = (VirtualProperty_get_pattern2)get;
        set_pattern1 = (VirtualProperty_set_pattern1)set;
    }
    
	template <typename C>
	VirtualProperty(C* obj, T (C::*get)(void) const,  void (C::*set)(const T&)) {
		this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern3 = (VirtualProperty_get_pattern3)get;
        set_pattern1 = (VirtualProperty_set_pattern1)set;
	}
    
    template <typename C>
    VirtualProperty(C* obj, const T& (C::*get)(void) const,  void (C::*set)(const T&)) {
        this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern4 = (VirtualProperty_get_pattern4)get;
        set_pattern1 = (VirtualProperty_set_pattern1)set;
    }
    
    
    template <typename C>
    VirtualProperty(C* obj, T(C::*get)(void), void (C::*set)(T) ) {
        this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern1 = (VirtualProperty_get_pattern1)get;
        set_pattern2 = (VirtualProperty_set_pattern2)set;
    }
    
    template <typename C>
    VirtualProperty(C* obj, const T& (C::*get)(void),  void (C::*set)(T) ) {
        this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern2 = (VirtualProperty_get_pattern2)get;
        set_pattern2 = (VirtualProperty_set_pattern2)set;
    }
    
    template <typename C>
    VirtualProperty(C* obj, T (C::*get)(void) const,  void (C::*set)(T) ) {
        this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern3 = (VirtualProperty_get_pattern3)get;
        set_pattern2 = (VirtualProperty_set_pattern2)set;
    }
    
    template <typename C>
    VirtualProperty(C* obj, const T& (C::*get)(void) const,  void (C::*set)(T) ) {
        this->thiz = (DelegateFriendObject*)(void*)obj;
        initializePointers();
        get_pattern4 = (VirtualProperty_get_pattern4)get;
        set_pattern2 = (VirtualProperty_set_pattern2)set;
    }
    

	void operator=(const T &param) const {
		if (set_pattern1 != NULL)
			(thiz->*set_pattern1)(param);
		else if (set_pattern2 != NULL)
			(thiz->*set_pattern2)(param);
	}
    /*
    void operator=(T param) const {
        if (set_pattern1 != NULL)
            (thiz->*set_pattern1)(param);
        else if (set_pattern2 != NULL)
            (thiz->*set_pattern2)(param);
    }
     */

	operator T() const {
		if (get_pattern1 != NULL)
			return (thiz->*get_pattern1)();
        else if (get_pattern2 != NULL)
            return (thiz->*get_pattern2)();
        else if (get_pattern3 != NULL)
            return (thiz->*get_pattern3)();
        else if (get_pattern4 != NULL)
            return (thiz->*get_pattern4)();
        return T();
	}
    
    bool operator==(const T &param) const {
        return (T)(*this) == param;
    }
    
    bool operator!=(const T &param) const {
        return (T)(*this) != param;
    }

};

#endif
