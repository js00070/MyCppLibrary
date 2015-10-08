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
	public:
		static ObjectString<T> Empty;

		ObjectString()
		{

		}
	};

	template<typename T>
	const T ObjectString::zero = 0;
	template<typename T>
	ObjectString<T> ObjectString<T>::Empty = ObjectString<T>();
}

#endif
