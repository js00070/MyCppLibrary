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

	template<typename >
	class MemPool : virtual public Interface
	{

	};
}

#endif
