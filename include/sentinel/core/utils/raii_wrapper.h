//
// Created by Marco on 29/08/2025.
//

#ifndef RAII_WRAPPER_H
#define RAII_WRAPPER_H

#include <sentinel/common.h>

/*  Templates allow you to write a function or class that works with different data types
 *  The first paramter is the typename with name 'T', common naming
 *  The second parameter is what gets called in the destructor for cleanup, it's a function pointer that takes 'T' and returns void
 *
 *	Note to self:
 *  At the time when I wrote the code for 'process-memory-tools' I implemented a RAII wrapper, but only for processes,
 *  with templates, it can support any type!!!! it's ideal for this project, I think even if it's a bit complex at first.
*/
template<typename T, void(*CleanupFunc)(T)>
class raii_wrapper
{
	public:
		raii_wrapper(T key) : handle(key) {}

		~raii_wrapper() {
			CleanupFunc(handle);
		}

		// conversion operator, let it be used like a normal HANDLE!
		operator HANDLE() const {
			return handle;
		}

		// disable copying (prevent accidental double-cleanup)
		raii_wrapper(const raii_wrapper&) = delete;
		raii_wrapper& operator=(const raii_wrapper&) = delete;

	private:
		T handle;
};



#endif //RAII_WRAPPER_H
