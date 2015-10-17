#ifndef ZZY_MEMPOOL
#define ZZY_MEMPOOL

#include <malloc.h>
#include "Basic.h"

namespace zl
{
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
				chunknum = blockSize / chunkSize;
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
		_MemPool()
		{
			blockTail = new block();
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
			while (blockTail)
			{
				delete blockTail;
				blockTail = blockTail->prev;
			}
		}

		~_MemPool()
		{
			FreeAll();
		}
	};

	template<typename T,size_t blockSize = 32 * 1024>
	class MemPool : virtual public Interface
	{
	private:
		_MemPool<sizeof(T),blockSize> mem;

	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T* const_pointer;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		MemPool() = default;
		~MemPool() = default;
		
		pointer address(reference x) const
		{}

		const_pointer address(const_reference x) const
		{}

		pointer allocate(size_type n = 1)
		{
			return static_cast<pointer>(mem.allocate());
		}

		void deallocate(pointer p, size_type n = 1)
		{
			mem.deallocate(const_cast<void*>(p));
		}



	};
}

#endif
