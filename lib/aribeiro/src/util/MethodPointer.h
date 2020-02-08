#ifndef MethodPointer__H
#define Delegate__H

#include "Delegate.h"

/*

Example:

//////////////////////////
// returning int Example
//////////////////////////

DefineMethodPointer(MethodSumPtr, int, int a, int b) ReturnMethodCall(a,b)

int FunctionExample(int a, int b) {
	return a + b;
}

MethodSumPtr methodPtr = MethodSumPtr(&FunctionExample);

int c = 0;
if (methodPtr != NULL)
	c = methodPtr(1, 2);

//////////////////////////
// returning void Example
//////////////////////////

DefineMethodPointer(MethodSumVoidPtr, void, int a, int b) VoidMethodCall(a,b)

void FunctionExample(int a, int b) {
	...
}

MethodSumVoidPtr methodPtr = MethodSumVoidPtr(&FunctionExample);

if (methodPtr != NULL)
	methodPtr(1, 2);

*/

namespace aRibeiro {

#define DefineMethodPointer(className,returnType,...) \
	typedef returnType (DelegateFriendObject::*className##Method) (__VA_ARGS__); \
	typedef returnType (*className##Function) (__VA_ARGS__); \
	class className { \
		DelegateFriendObject* obj; \
		className##Method methodPtr; \
		className##Function functionPtr; \
	public: \
		className ( ){ \
			this->obj = NULL; \
			this->methodPtr = NULL; \
			this->functionPtr = NULL; \
		} \
		template <typename T> \
		className ( T* obj, returnType(T::*methodPtr) (__VA_ARGS__) ){ \
			this->obj = static_cast<DelegateFriendObject*>((void*)obj); \
			this->methodPtr = (className##Method) methodPtr; \
			this->functionPtr = NULL; \
		} \
		className ( returnType (*functionPtr) (__VA_ARGS__) ){ \
			this->obj = NULL; \
			this->methodPtr = NULL; \
			this->functionPtr = functionPtr; \
		} \
		bool operator==(void* ptr) { \
			if (ptr == NULL) \
				return this->obj == NULL && this->methodPtr == NULL && this->functionPtr == NULL; \
			return false; \
		} \
		bool operator!=(void* ptr) { \
			if (ptr == NULL) \
				return this->obj != NULL || this->methodPtr != NULL || this->functionPtr != NULL; \
			return false; \
		} \
		returnType operator() ( __VA_ARGS__ ) {


#define ReturnMethodCall( ... ) \
			if (obj != NULL) \
				return (obj->*(methodPtr))(__VA_ARGS__); \
			else \
				return (functionPtr)(__VA_ARGS__); \
		} \
	};

#define VoidMethodCall( ... ) \
			if (obj != NULL) \
				(obj->*(methodPtr))(__VA_ARGS__); \
			else \
				(functionPtr)(__VA_ARGS__); \
		} \
	};

}

#endif
