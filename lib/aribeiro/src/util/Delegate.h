#ifndef Delegate__H
#define Delegate__H

//#include "Object.h"
//#include <aribeiro/aribeiro.h>
#include <aribeiro/common.h>
#include <vector>

/*
delegate declaration: 

BEGIN_DECLARE_DELEGATE(ClassName, <Parameters List Method Sign> ) CALL_PATTERN (<Parameters List Method Call>) END_DECLARE_DELEGATE;

Example:

BEGIN_DECLARE_DELEGATE(ViewportDelegate, Viewport* viewport ) CALL_PATTERN (viewport) END_DECLARE_DELEGATE;

ViewportDelegate OnViewport;

//on code
OnViewport.add(&func);
OnViewport.call(viewport);


*/

namespace aRibeiro {

	class DelegateDummyObjectT {};

#define BEGIN_DECLARE_DELEGATE(className, ... ) \
	typedef void (DelegateDummyObjectT::*className##Method) (__VA_ARGS__); \
	typedef void (*className##Function) (__VA_ARGS__); \
	class className{ \
		struct DelegateEntry{ \
			DelegateDummyObjectT* mObjPtr; \
			className##Method mMethodPtr; \
			className##Function mFunctionPtr; \
			int functionID; \
			DelegateEntry(){ mObjPtr = NULL; mMethodPtr = NULL; mFunctionPtr = NULL; functionID = 0;} \
			DelegateEntry(DelegateDummyObjectT* objptr, className##Method methodPtr){ mObjPtr = objptr; mMethodPtr = methodPtr; mFunctionPtr = NULL;functionID = 1;} \
			DelegateEntry(className##Function functionPtr){ mObjPtr = NULL; mMethodPtr = NULL; mFunctionPtr = functionPtr;functionID = 2;} \
			bool operator==(const DelegateEntry& v)const{ \
				if ( (functionID == 1) && mObjPtr != NULL && v.mObjPtr != NULL) \
					return mObjPtr == v.mObjPtr && mMethodPtr == v.mMethodPtr;  \
				else if ( (functionID == 2) && mFunctionPtr != NULL && v.mFunctionPtr != NULL) \
					return mFunctionPtr == v.mFunctionPtr; \
				else \
					return false; \
			} \
		}; \
		std::vector<DelegateEntry> mDelegateEntry; \
	public: \
		void add(DelegateDummyObjectT* objPtr, className##Method methodPtr){ \
			DelegateEntry entry (objPtr,methodPtr); \
			for (size_t i=0;i<mDelegateEntry.size();i++) \
				if (mDelegateEntry[i] == entry) \
					return; \
		    mDelegateEntry.push_back(entry); \
        } \
		template <typename T> \
		void add(void* objPtr, void (T::*methodPtr) (__VA_ARGS__) ){ \
			add(static_cast<DelegateDummyObjectT*>(objPtr), (className##Method) methodPtr) ; \
        } \
		void add(className##Function functionPtr){ \
			DelegateEntry entry (functionPtr); \
			for (size_t i=0;i<mDelegateEntry.size();i++) \
				if (mDelegateEntry[i] == entry) \
					return; \
		    mDelegateEntry.push_back(entry); \
        } \
		void remove(DelegateDummyObjectT* objPtr, className##Method methodPtr){ \
			DelegateEntry entry (objPtr,methodPtr ); \
			std::vector<DelegateEntry>::iterator it = mDelegateEntry.begin(); \
			for (; it != mDelegateEntry.end(); it++) \
				if ((*it) == entry){ \
					mDelegateEntry.erase(it); \
					return; \
				} \
        } \
		template <typename T> \
		void remove(void* objPtr, void (T::*methodPtr) (__VA_ARGS__) ){ \
			remove( static_cast<DelegateDummyObjectT*>(objPtr), (className##Method) methodPtr); \
		} \
		void remove(className##Function functionPtr){ \
			DelegateEntry entry (functionPtr ); \
			std::vector<DelegateEntry>::iterator it = mDelegateEntry.begin(); \
			for (; it != mDelegateEntry.end(); it++) \
				if ((*it) == entry){ \
					mDelegateEntry.erase(it); \
					return; \
				} \
        } \
		void removeAll(){ \
			mDelegateEntry.clear(); \
		} \
		void call(__VA_ARGS__){ 

#define CALL_PATTERN( ... ) \
			for (size_t i=0;i<mDelegateEntry.size();i++) \
				if (mDelegateEntry[i].functionID == 1) \
					(mDelegateEntry[i].mObjPtr->*(mDelegateEntry[i].mMethodPtr))(__VA_ARGS__); \
				else if (mDelegateEntry[i].functionID == 2) \
					(mDelegateEntry[i].mFunctionPtr)(__VA_ARGS__);



#define END_DECLARE_DELEGATE ; \
		} \
	}


}


#endif
