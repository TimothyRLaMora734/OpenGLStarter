#ifndef transform_stack__H
#define transform_stack__H

#include <vector>

namespace aRibeiro {

	template <typename T>
	class TransformStack {

	private:

		std::vector<T> _stack;

		//private copy constructores, to avoid copy...
		TransformStack(const TransformStack& v) {}
		void operator=(const TransformStack& v) {}

	public:

		T top;

		int size() {
			return _stack.size();
		}

		TransformStack() :_stack(0) {
			top = T();// mat4::IdentityMatrix;
		}

		void push() {
			_stack.push_back(top);
		}

		void pop() {
			if (_stack.size() > 0) {
				top = _stack[_stack.size() - 1];
				_stack.pop_back();
			}
			//else
				//printf("error to pop element...\n");
		}
        
        void operator=(const T &v) {
            top = v;
        }
        
        operator T() const {
            return top;
        }

	};

}
#endif
