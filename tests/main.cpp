#include "../mesitype.h"
#include "tee/tee.hpp"

#include <vector>
#include <string>
#include <tuple>
#include <iostream>

using namespace std;

Tee_Test(test_basic_rules) {
	auto x = Mesi::Meters(3);
	auto y = Mesi::Meters(4);
	auto z = Mesi::Meters(5);

	Tee_SubTest(test_addition) {
		assert(static_cast<float>(x + y) == 7);
	}

	Tee_SubTest(test_addition_is_commutative) {
		assert(x + y == y + x);
	}

	Tee_SubTest(test_adding_zero_is_identity) {
		assert(x + Mesi::Meters(0) == x);
		assert(y + Mesi::Meters(0) == y);
		assert(z + Mesi::Meters(0) == z);
	}

	Tee_SubTest(test_addition_is_associative) {
		assert((x + y) + z == x + (y + z));
	}

	Tee_SubTest(test_negation_is_involuting) {
		assert(x == -(-x));
		assert(y == -(-y));
		assert(z == -(-z));
	}

	Tee_SubTest(test_subtraction_inverts_addition) {
		assert(x - y + y == x);
		assert(y - z + z == y);
	}
}

Tee_Test(test_scalar_rules) {
	auto x = Mesi::Meters(3);
	auto y = Mesi::Seconds(4);
	auto z = Mesi::Kilos(5);

	Tee_SubTest(test_scaling_multiplies) {
		assert(static_cast<float>(2 * x) == 6);
		assert(static_cast<float>(2 * y) == 8);
		assert(static_cast<float>(2 * z) == 10);
	}

	Tee_SubTest(test_scaling_is_commutative) {
		assert(2 * x == x * 2);
		assert(7 * y == y * 7);
		assert(9 * z == z * 9);
	}

	Tee_SubTest(test_scaling_by_one_is_identity) {
		assert(1 * x == x);
		assert(1 * y == y);
		assert(1 * z == z);
	}

	Tee_SubTest(test_scaling_by_zero_is_null) {
		assert(0 * x == x - x);
		assert(0 * y == y - y);
		assert(0 * z == z - z);
	}

	Tee_SubTest(test_scaling_is_associative) {
		assert(2 * (3 * x) == (2 * 3) * x);
		assert(8 * (5 * y) == (8 * 5) * y);
		assert(9 * (4 * z) == (9 * 4) * z);
	}
}

Tee_Test(test_divisor_rules) {
	auto x = Mesi::Meters(3);
	auto y = Mesi::Seconds(4);
	auto z = Mesi::Kilos(5);

	Tee_SubTest(test_dividing_scales_down) {
		assert(static_cast<float>(x / 2) == 1.5f);
		assert(static_cast<float>(y / 2) == 2.f);
		assert(static_cast<float>(z / 10) == 0.5f);
	}

	Tee_SubTest(test_dividing_matches_multiplying_by_the_inverse) {
		assert(x / 2 == (1/2.f) * x);
		assert(y / 3 == (1/3.f) * y);
		assert(z / 10 == (1/10.f) * z);
	}

	Tee_SubTest(test_dividing_by_one_is_identity) {
		assert(x / 1 == x);
		assert(y / 1 == y);
		assert(z / 1 == z);
	}

	Tee_SubTest(test_dividion_inverts_scaling) {
		assert( (x / 2) * 2 == x);
		assert( (y / 7) * 7 == y);
		assert( (z / 5) * 5 == z);
	}
}

int main() {
	int successes;
	vector<string> fails;
	tie(successes, fails) = Tee::RunAllTests();
	for(auto f : fails)
		cout << f;
	cout << successes << " successes, " << fails.size() << " failures." << endl;
	return fails.size() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
