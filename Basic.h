#ifndef ZZY_BASIC
#define ZZY_BASIC

#ifdef _MSC_VER
#define ZZY_MSVC
#else
#define ZZY_GCC
#endif

#if defined ZZY_MSVC
#include <intrin.h>
#elif defined ZZY_GCC
#include <x86intrin.h>
#include <stdint.h>
#include <stddef.h>
#include <wchar.h>
#endif

namespace zl
{
#ifdef ZZY_MSVC
#define INCRC(x)	(_InterlockedIncrement((volatile long*)(x)))
#define DECRC(x)	(_InterlockedDecrement((volatile long*)(x)))
#elif defined ZZY_GCC
#define INCRC(x)	(__sync_add_and_fetch(x, 1))
#define DECRC(x)	(__sync_sub_and_fetch(x, 1))
#endif

	/// <summary>Base type of all not copyable class.</summary>
	class NotCopyable
	{
	private:
		NotCopyable(const NotCopyable&) {}
		NotCopyable& operator=(const NotCopyable&){
			return *this;
		}
	public:
		NotCopyable() {};
	};

	/// <summary>Base type of all errors. An error is an exception that you are not allowed to catch. Raising it means there is a fatal error in the code.</summary>
	class Error
	{
	private:
		const wchar_t* description;
	public:
		Error(const wchar_t* _description){
			description = _description;
		}
		const wchar_t* Description() const{
			return description;
		}
	};

#ifndef NDEBUG
#define CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw zl::Error(DESCRIPTION);}while(0)
#endif
#ifdef NDEBUG
#define CHECK_ERROR(CONDITION,DESCRIPTION)
#endif

	/// <summary>Base type of all classes</summary>
	class Object
	{
	public:
		virtual ~Object() {}
	};
	/// <summary>Base type of all interfaces. All interface types are encouraged to be virtual inherited.</summary>
	class Interface : private NotCopyable
	{
	public:
		virtual ~Interface() {}
	};
}

#endif
