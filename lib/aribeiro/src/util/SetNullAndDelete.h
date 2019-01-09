#ifndef SetNullAndDelete__H
#define SetNullAndDelete__H

#include <stdlib.h>

//
// Templates
//
namespace aRibeiro {

	template <typename C>
	static void setNullAndDelete(C *&obj) {
		if (obj != NULL) {
			C * aux = obj;
			obj = NULL;
			delete aux;
		}
	}

	template <typename C>
	static void setNullAndDeleteArray(C *&obj) {
		if (obj != NULL) {
			C * aux = obj;
			obj = NULL;
			delete[]aux;
		}
	}

}

#endif

