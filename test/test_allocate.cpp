#include "../allocate.h"
#include <iostream>

using namespace yc;

int main() {
	allocator<int> alloc;

	allocator<int>::pointer p = alloc.allocate(10);

	uninitialized_fill(p, p + 10, 1);

	for (int i = 0; i < 10; ++i)
		std::cout << *(p + i) << std::endl;
	alloc.construct(p, 10);
	std::cout << *p << std::endl;
	alloc.destroy(p);

	alloc.deallocate(p);

	return 0;
}