#include "mesitype.h"

#include <iostream>

int main() {
	auto constexpr foo = MesiType<float, 0, 0, 1>(1.f);

	constexpr auto xyzzy = MesiType<float, 0, 0, 1>(1.f);
//	constexpr auto foo = 1._m;
	constexpr auto bar = 2_m;
	constexpr auto baz = 1._m - bar;

	std::cout << foo.val << foo.getUnit() << std::endl;
	std::cout << baz.val << baz.getUnit() << std::endl;
	std::cout << xyzzy.val << xyzzy.getUnit() << std::endl;
}
