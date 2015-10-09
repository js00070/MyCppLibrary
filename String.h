/*************************
AString : Mbcs
WString : Utf-16
*************************/
#ifndef ZZY_WSTRING
#define ZZY_WSTRING

#include <memory.h>
#include "Basic.h"

namespace zl
{
	/// <summary>A type representing a string.</summary>
	/// <typeparam name="T">Type of a character.</typeparam>
	template<typename T>
	class ObjectString : public Object
	{
	private:
		static const T zero;
		mutable T* buffer;
		mutable volatile int* counter; //引用计数
		mutable int length;
		mutable int start;
		mutable int capacity;

		void INC() const
		{
			INCRC(counter);
		}

		void DEC() const
		{
			if (counter)
			{
				if (DECRC(counter) == 0)
				{
					delete[] buffer;
					delete counter;
				}
			}
		}

	public:
		static ObjectString<T> Empty;

		ObjectString() :
			buffer(nullptr), counter(nullptr), length(0), start(0) ,capacity(0) {}

		ObjectString(ObjectString<T>& string) :
			buffer(string.buffer), counter(string.counter), length(string.length), start(string.start), capacity(string.capacity)
		{
			INC();
		}

		ObjectString(ObjectString<T>& string, int _start, int _length) :
			buffer(string.buffer), counter(string.counter), length(_length), start(string.start+_start), capacity(string.capacity)
		{
			INC();
		}

		~ObjectString()
		{
			DEC();
		}
	};

	template<typename T>
	const T ObjectString::zero = 0;
	template<typename T>
	ObjectString<T> ObjectString<T>::Empty = ObjectString<T>();
}

#endif
