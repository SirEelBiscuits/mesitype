#include "mesitype.h"

#include <iostream>

int main() {
	constexpr auto xyzzy = MesiType<float, 0, 0, 1>(1);
	constexpr auto foo = 1._m;
	constexpr auto bar = 2._m;
	constexpr auto baz = 1._m + bar;
	std::cout << foo.val << foo.getUnit() << std::endl;
	std::cout << baz.val << baz.getUnit() << std::endl;
	std::cout << xyzzy.val << xyzzy.getUnit() << std::endl;
}
