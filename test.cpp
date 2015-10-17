#include <iostream>
#include "MemPool.h"
#include <vector>
#include <cstdio>
#include <cstdlib>

struct test
{
	int a[1024];

	test()
	{
		for (int i = 0; i < 1024; ++i)
			a[i] = i;
	}
};

std::vector<test*> testlist;

int main()
{
	zl::MemPool<test> alloc;
	for (int i = 0; i < 100; ++i)
	{
		testlist.push_back(alloc.allocate());
	}
	printf("%d\n",alloc.GetBlockCount());
	for (int i = 0; i < 50;++i)
	{
		alloc.deallocate(testlist[i]);
	}
	printf("%d\n", alloc.GetBlockCount());
	system("pause");
	return 0;
}