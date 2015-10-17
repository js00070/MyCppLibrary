#include <iostream>
#include "../MemPool.h"
#include <vector>

struct test
{
	int a[16];
};

std::vector<test*> testlist;

int main()
{
	zl::MemPool<test> alloc;
	for (int i = 0; i < 10000; ++i)
	{
		testlist.push_back(alloc.allocate());
	}
	return 0;
}