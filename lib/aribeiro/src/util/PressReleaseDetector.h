#ifndef Press_Release__H
#define Press_Release__H

#include "Delegate.h"

namespace aRibeiro {

	BEGIN_DECLARE_DELEGATE(EventPressReleaseDetector) CALL_PATTERN() END_DECLARE_DELEGATE;

	class PressReleaseDetector {

	public:

		//
		// public state variables
		//
		bool down;
		bool up;
		bool pressed;

		//
		// events
		//
		EventPressReleaseDetector OnDown;
		EventPressReleaseDetector OnUp;

		PressReleaseDetector();
		void setState(bool v);

	};

}
#endif
