#ifndef Property__H
#define Property__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

template <typename T>
class Property {

	// avoid copy, using copy constructors
	Property(const Property&) {}
	void operator=(const Property&) {}

public:
    
    BEGIN_DECLARE_DELEGATE(PropertyBaseEvent, Property<T> *prop) CALL_PATTERN(prop) END_DECLARE_DELEGATE;

	T oldValue;
	T value;

	PropertyBaseEvent OnChange;


	Property(const T &defaultValue) {
		oldValue = defaultValue;
		value = defaultValue;
	}

	Property() {
		oldValue = T();
		value = T();
	}

	void operator=(const T &v) {
		if (
			value != v
			//memcmp(&v, &value, sizeof(T)) != 0
			) {
			oldValue = value;
			value = v;
			OnChange.call(this);
		}
	}

	operator T() const {
		return value;
	}
};


#endif
