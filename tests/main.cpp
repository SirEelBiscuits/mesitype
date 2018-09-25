#include "../mesitype.h"
#include "tee/tee.hpp"

#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <regex>

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
	auto z = Mesi::Kilograms(5);

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
	auto w = Mesi::RationalType<float, 1, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1>(2);
	auto x = Mesi::Meters(3);
	auto y = Mesi::Seconds(4);
	auto z = Mesi::Kilograms(5);

	Tee_SubTest(test_dividing_scales_down) {
		assert(static_cast<float>(w / 2) == 1.f);
		assert(static_cast<float>(x / 2) == 1.5f);
		assert(static_cast<float>(y / 2) == 2.f);
		assert(static_cast<float>(z / 10) == 0.5f);
	}

	Tee_SubTest(test_dividing_matches_multiplying_by_the_inverse) {
		assert(w / 2 == (1/2.f) * w);
		assert(x / 2 == (1/2.f) * x);
		assert(y / 3 == (1/3.f) * y);
		assert(z / 10 == (1/10.f) * z);
	}

	Tee_SubTest(test_dividing_by_one_is_identity) {
		assert(w / 1 == w);
		assert(x / 1 == x);
		assert(y / 1.0 == y);
		assert(z / 1.0f == z);
	}

	Tee_SubTest(test_dividion_inverts_scaling) {
		assert( (w / 2) * 2 == w);
		assert( (x / 2) * 2 == x);
		assert( (y / 7) * 7 == y);
		assert( (z / 5) * 5 == z);
	}
}

Tee_Test(type_info_tests) {
	Tee_SubTest(test_meters_indicators_match_units) {
		auto s = Mesi::Scalar(1);
		auto m = Mesi::Meters(1);
		auto m2 = Mesi::MetersSq(1);
		auto m1_2 = Mesi::RationalType<float, 1, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1>(1);

		auto mUnit = std::regex(R"(m |m$)");
		auto m2Unit = std::regex(R"(m\^)");
		auto m1_2Unit = std::regex(R"(m\^\([^)]*\))");

		assert(s.getUnit().compare("") == 0);
		assert(std::regex_search(m.getUnit(), mUnit));
		assert(std::regex_search(m2.getUnit(), m2Unit));
		assert(std::regex_search(m1_2.getUnit(), m1_2Unit));
	}

	Tee_SubTest(test_seconds_indicators_match_units) {
		auto S = Mesi::Scalar(1);
		auto s = Mesi::Seconds(1);
		auto s2 = Mesi::SecondsSq(1);
		auto s1_2 = Mesi::RationalType<float, 0, 1, 1, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1>(1);

		auto sUnit = std::regex(R"(s |s$)");
		auto s2Unit = std::regex(R"(s\^)");
		auto s1_2Unit = std::regex(R"(s\^\([^)]*\))");

		assert(S.getUnit().compare("") == 0);
		assert(std::regex_search(s.getUnit(), sUnit));
		assert(std::regex_search(s2.getUnit(), s2Unit));
		assert(std::regex_search(s1_2.getUnit(), s1_2Unit));
	}

	Tee_SubTest(test_kilos_indicators_match_units) {
		auto s = Mesi::Scalar(1);
		auto kg = Mesi::Kilograms(1);
		auto kg2 = Mesi::KilogramsSq(1);
		auto kg1_2 = Mesi::RationalType<float, 0, 1, 0, 1, 1, 2, 0, 1, 0, 1, 0, 1, 0, 1>(1);

		auto kgUnit = std::regex(R"(kg |kg$)");
		auto kg2Unit = std::regex(R"(kg\^)");
		auto kg1_2Unit = std::regex(R"(kg\^\([^)]*\))");

		assert(s.getUnit().compare("") == 0);
		assert(std::regex_search(kg.getUnit(), kgUnit));
		assert(std::regex_search(kg2.getUnit(), kg2Unit));
		assert(std::regex_search(kg1_2.getUnit(), kg1_2Unit));
	}

	Tee_SubTest(test_all_unit_indicators_match_units) {
		auto mskg = Mesi::Type<float, 1, 1, 1>(1).getUnit();
		auto m2s2kg2A1_2 = Mesi::RationalType<float, 2, 1, 2, 1, 2, 1, 1, 2, 0, 1, 0, 1, 0, 1>(1).getUnit();

		auto mUnit = std::regex(R"(m |m$)");
		auto m2Unit = std::regex(R"(m\^)");
		auto sUnit = std::regex(R"(s |s$)");
		auto s2Unit = std::regex(R"(s\^)");
		auto kgUnit = std::regex(R"(kg |kg$)");
		auto kg2Unit = std::regex(R"(kg\^)");
		auto A1_2Unit = std::regex(R"(A\^\([^)]*\))");

		assert(std::regex_search(mskg, mUnit));
		assert(std::regex_search(mskg, sUnit));
		assert(std::regex_search(mskg, kgUnit));

		assert(std::regex_search(m2s2kg2A1_2, m2Unit));
		assert(std::regex_search(m2s2kg2A1_2, s2Unit));
		assert(std::regex_search(m2s2kg2A1_2, kg2Unit));
		assert(std::regex_search(m2s2kg2A1_2, A1_2Unit));
	}
}

Tee_Test(test_multiplicative_behaviour) {
		auto m = Mesi::Meters(2);
		auto s = Mesi::Seconds(3);
		auto kg = Mesi::Seconds(5);
		auto A1_2 = Mesi::RationalType<float, 0, 1, 0, 1, 0, 1, 1, 2, 0, 1, 0, 1, 0, 1>(1);
		auto mskgA1_2 = m * s * kg * A1_2;

	Tee_SubTest(test_multiplying_values_consistent_with_floats) {
		assert(static_cast<float>(mskgA1_2) == 30);
	}

	Tee_SubTest(test_multiplying_units_is_commutative) {
		assert(m * s == s * m);
		assert(s * kg == kg * s);
		assert(kg * m == m * kg);
		assert(kg * A1_2 == A1_2 * kg);
	}

	Tee_SubTest(test_multiplying_units_is_associative) {
		assert((m * s) * kg == m * (s * kg));
		assert((m * s) * A1_2 == m * (s * A1_2));
	}

	Tee_SubTest(test_inversion_multiplies_to_unity) {
		assert((1 / mskgA1_2) * mskgA1_2 == Mesi::Scalar(1));
		assert(mskgA1_2 / mskgA1_2 == Mesi::Scalar(1));
	}

	Tee_SubTest(test_inversion_methods_match) {
		assert(1 / mskgA1_2 == mskgA1_2 / mskgA1_2 / mskgA1_2);
	}
}

Tee_Test(test_assignment_operators) {
	auto m = Mesi::Meters(1);

	Tee_SubTest(test_assignment) {
		m = Mesi::Meters(2);
		assert(m.val == 2);
	}

	Tee_SubTest(test_add_assignment) {
		m += Mesi::Meters(1);
		assert(m.val == 3);
	}

	Tee_SubTest(test_sub_assignment) {
		m -= Mesi::Meters(2);
		assert(m.val == 1);
	}

	Tee_SubTest(test_multiply_assignment) {
		m *= 4;
		assert(m.val == 4);
	}

	Tee_SubTest(test_divide_assignment) {
		m /= 2;
		assert(m.val == 2);
	}

	Tee_SubTest(test_multiply_scalar_assignment) {
		m *= Mesi::Scalar(3);
		assert(m.val == 6);
	}

	Tee_SubTest(test_divide_scalar_assignment) {
		m /= Mesi::Scalar(2);
		assert(m.val == 3);
	}
}

Tee_Test(test_comparison_operators) {
	auto m = Mesi::Meters(2);

	Tee_SubTest(test_comparison_equals) {
		assert(m == Mesi::Meters(2));
		assert(m == m);
		assert((m == m + m) == false);
	}

	Tee_SubTest(test_comaprison_not_equals) {
		assert(m != Mesi::Meters(3));
		assert((m != m) == false);
	}

	Tee_SubTest(test_less_than) {
		assert(m < m + m);
		assert((m < m) == false);
		assert((m + m < m) == false);
	}

	Tee_SubTest(test_less_equal_than) {
		assert(m <= m + m);
		assert(m <= m);
		assert((m + m <= m) == false);
	}

	Tee_SubTest(test_greater_than) {
		assert((m > m + m) == false);
		assert((m > m) == false);
		assert(m + m > m);
	}

	Tee_SubTest(test_greater_equal_than) {
		assert((m >= m + m) == false);
		assert(m >= m);
		assert(m + m >= m);
	}
}

Tee_Test(test_literal_overloads) {
	using namespace Mesi::Literals;

	assert(Mesi::Meters(1) == 1_m);
	assert(Mesi::MetersSq(1) == 1_m2);
	assert(Mesi::Seconds(1) == 1_s);
	assert(Mesi::SecondsSq(1) == 1_s2);
	assert(Mesi::Kilograms(1) == 1_kg);
	assert(Mesi::KilogramsSq(1) == 1_kg2);
	assert(Mesi::Newtons(1) == 1_n);
	assert(Mesi::NewtonsSq(1) == 1_n2);
}

int main() {
	int successes;
	vector<string> fails;
	tie(successes, fails) = Tee::RunAllTests();
	for(auto f : fails)
		cout << f;
	cout << successes << " successes, "
		<< fails.size() << " failures." << endl;
	return fails.size() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
