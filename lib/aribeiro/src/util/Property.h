#ifndef Property__H
#define Property__H

#include <aribeiro/common.h>
#include <aribeiro/Delegate.h>

/*
 property declaration:
 
 
 Property<vec2> Size;
 
 Size = vec2(1.0f);
 
 // you can listen to property changes
 
 void MyListeningFunction(Property<vec2> *prop) {
    vec2 vOld = prop->oldValue;
    vec2 vNew = prop->value;
    //you can rollback the modification.
    //  The next listeners will receive the old value as new to set...
    prop->rollback();
 }
 
 Size.OnChange.add(MyListeningFunction);
 
 
 */

namespace aRibeiro {

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
        
        void rollback() {
            value = oldValue;
        }

        void operator=(const T &v) {
            if (
                value != v
                //memcmp(&v, &value, sizeof(T)) != 0
                ) {
                oldValue = value;
                value = v;
                OnChange(this);
            }
        }

        operator T() const {
            return value;
        }
    };
    
}

#endif
