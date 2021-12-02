#include "../any.h"
#include <iostream>
#include <string>

int main() {
	any a = 0;
	std::cout << a.type().name() << std::endl;
	std::cout << any_cast<int>(a) << std::endl;

	a = std::string("adg");
	std::cout << a.type().name() << std::endl;
	std::cout << any_cast<std::string>(a) << std::endl;

	any b = a;
	std::cout << b.type().name() << std::endl;
	std::cout << any_cast<std::string>(b) << std::endl;

	return 0;
}