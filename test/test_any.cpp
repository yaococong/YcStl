#include "../any.h"
#include <iostream>
#include <string>

using namespace yc;
int main() {
	any_y a = 0;
	std::cout << a.type().name() << std::endl;
	std::cout << any_cast_y<int>(a) << std::endl;

	a = std::string("adg");
	std::cout << a.type().name() << std::endl;
	std::cout << any_cast_y<std::string>(a) << std::endl;

	any_y b = a;
	std::cout << b.type().name() << std::endl;
	std::cout << any_cast_y<std::string>(b) << std::endl;

	return 0;
}