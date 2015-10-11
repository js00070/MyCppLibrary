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
			if(counter)
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

		static int GetCapacity(const T *_buffer)
		{
			const T *p = _buffer;
			while (*p) ++p;
			return p - _buffer + 1;
		}


		ObjectString(ObjectString<T>& string, int _start, int _length) :
			buffer(string.buffer), counter(string.counter), length(_length), start(string.start + _start), capacity(string.capacity)
		{
			INC();
		}

	public:
		static ObjectString<T> Empty;

		ObjectString() :
			buffer((T*)&zero), counter(nullptr), length(0), start(0) ,capacity(0) {}

		ObjectString(ObjectString<T>& string) :
			buffer(string.buffer), counter(string.counter), length(string.length), start(string.start), capacity(string.capacity)
		{
			INC();
		}

		ObjectString(const T* _buffer, int _length)
		{
			if (_length <= 0)
				ObjectString();
			else
			{
				capacity = _length;
				buffer = new T[capacity];
				length = _length;
				counter = new int(1);
				start = 0;
				memcpy(buffer,_buffer,sizeof(T)*capacity);
			}
		}

		ObjectString(const T* _buffer, bool copy = true)
		{
			if (copy)
			{
				capacity = GetCapacity(_buffer)-1;
				length = capacity;
				buffer = new T[capacity];
				counter = new int(1);
				start = 0;
				memcpy(buffer,_buffer,sizeof(T)*capacity);
			}
			else
			{
				capacity = GetCapacity(_buffer)-1;
				length = capacity;
				buffer = _buffer;
				start = 0;
				counter = new int(1);
			}
		}

		ObjectString(ObjectString<T>&& string) :
			buffer(string.buffer), counter(string.counter),start(string.start), length(string.length), capacity(string.capacity)
		{
			string.buffer = (T*)&zero;
			string.counter = nullptr;
			string.start = 0;
			string.length = 0;
			string.capacity = 0;
		}

		~ObjectString()
		{
			DEC();
		}

		ObjectString<T>& operator=(ObjectString<T>& string)
		{
			if (this != &string)
			{
				DEC();
				buffer = string.buffer;
				counter = string.counter;
				start = string.start;
				length = string.length;
				capacity = string.capacity;
				INC();
			}
			else
				return *this;
		}

		ObjectString<T>& operator=(ObjectString<T>&& string)
		{
			if (this != &string)
			{
				DEC();
				buffer = string.buffer;
				counter = string.counter;
				start = string.start;
				length = string.length;
				capacity = string.capacity;
				string.buffer = (T*)&zero;
				string.counter = nullptr;
				string.length = 0;
				string.start = 0;
				string.capacity = 0;
			}
			else
				return *this;
		}

		static int Compare(ObjectString<T>& lhs, ObjectString<T>& rhs)
		{
			int len = lhs.length < rhs.length ? lhs.length : rhs.length;

		}

	};

	template<typename T>
	const T ObjectString::zero = 0;
	template<typename T>
	ObjectString<T> ObjectString<T>::Empty = ObjectString<T>();
}

#endif
