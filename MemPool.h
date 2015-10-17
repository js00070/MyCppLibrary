#ifndef ZZY_MEMPOOL
#define ZZY_MEMPOOL

#include <new>
#include <type_traits>
#include "Basic.h"

namespace zl
{
	template<typename T,bool is_trivial>
	struct placement_new
	{
		static void construct(T* ptr) {}
	};
	
	template<typename T>
	struct placement_new<T,false>
	{
		static void construct(T* ptr)
		{
			new(ptr)T();
		}
	};


	template<size_t chunkSize,size_t blockSize = 32 * 1024> //32kb
	class _MemPool : virtual public Interface
	{
	private:
		union chunk
		{
			char mem[chunkSize];
			chunk* next;
		};

		struct block
		{
			chunk chunkList[blockSize/chunkSize];
			size_t chunkNum;
			block* prev;
			block* next;

			block()
			{
				chunkNum = blockSize / chunkSize;
				prev = next = nullptr;
				for (int i = 0; i < chunkNum - 1; ++i)
				{
					chunkList[i].next = &chunkList[i + 1];
				}
				chunkList[chunkNum - 1].next = nullptr;
			}

		};

	private:
		mutable chunk* chunkRoot;
		mutable block* blockTail;

	public:
		size_t blockcount;

		_MemPool()
		{
			blockTail = new block();
			blockTail->prev = nullptr;
			blockcount = 1;
			chunkRoot = blockTail->chunkList;
		}

		void* allocate()
		{
			chunk* result;
			if (chunkRoot)
				result = chunkRoot;
			else
			{
				blockTail->next = new block();
				++blockcount;
				blockTail->next->prev = blockTail;
				blockTail = blockTail->next;
				result = blockTail->chunkList;
			}
			chunkRoot = result->next;
			return static_cast<void*>(result);
		}

		void deallocate(void* ptr)
		{
			chunk* tmpch = static_cast<chunk*>(ptr);
			tmpch->next = chunkRoot;
			chunkRoot = tmpch;
		}

		void FreeAll()
		{
			while (blockTail->prev)
			{
				blockTail = blockTail->prev;
				delete blockTail->next;
			}
			delete blockTail;
		}

		~_MemPool()
		{
			FreeAll();
		}
	};

	template<typename T,size_t blockSize = 32 * 1024>
	class MemPool : virtual public Interface
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T* const_pointer;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	private:
		_MemPool<sizeof(value_type),blockSize> mem;

	public:
		MemPool() = default;
		~MemPool() = default;
		
		pointer address(reference x) const
		{}

		const_pointer address(const_reference x) const
		{}

		pointer allocate(size_type n = 1)
		{
			pointer ptr =  static_cast<pointer>(mem.allocate());
			placement_new<value_type, std::is_trivially_constructible<value_type>::value>::construct(ptr);
			return ptr;
		}

		void deallocate(pointer p, size_type n = 1)
		{
			p->~value_type();
			mem.deallocate(static_cast<void*>(p));
		}

		size_t GetBlockCount()
		{
			return mem.blockcount;
		}

	};
}

#endif
