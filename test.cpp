#include "../testsuite/testsuite.h"
#include "mesitype.h"

#include <cstdlib>
#include <ctime>

BEGIN_TEST_DEF(CompileTest) {
	auto Var = 3_m;
	auto Var2 = 3_s;
	auto Var3 = 3_kg;
	auto Var4 = MesiType<float, 1, 1, 1>(1);
	return true || (Var * Var2 * Var3 * Var4).val;
}
END_TEST_DEF(CompileTest);

BEGIN_TEST_DEF(TypeTest) {
	auto dist = 5_m;
	auto time = 20_s;
	MesiType<float, 1, -1, 0> speed = dist/time;
	return speed.val || true;
}
END_TEST_DEF(TypeTest);

BEGIN_TEST_DEF(UnitsTest) {
	auto foo = 1_m * 1_s * 1_kg;
	return true || foo.val;
}
END_TEST_DEF(UnitsTest);

BEGIN_TEST_DEF(ScalerTest) {
	for( int i = 0; i < 100; ++i ) {
		float sp = rand() % 10 + (rand() % 100 / 100.f);
		float s = rand() % 100 + (rand() % 100 / 100.f);
		auto speed = MesiType<float, 1, 1, 0>(sp);
		if( (speed * s - MesiType<float, 1, 1, 0>(sp * s)).val > 0.01f )
			return false;
	}
	return true;
}
END_TEST_DEF(ScalerTest);

BEGIN_TEST_DEF(AbelianTest) {
	for( int i = 0; i < 100; ++i ) {
		auto weight = MesiType<int, 0, 0, 1>(rand() % 100);
		auto time   = MesiType<int, 0, 1, 0>(rand() % 100);

		ASSERT_EQ_OUT( weight * time, time * weight,
				[](decltype(weight*time) x){ std::cerr << x.val; } );
	}
	return true;
}
END_TEST_DEF(AbelianTest);

BEGIN_TEST_DEF(RelationTest) {
	for( int i = 0; i < 100; ++i ) {
		auto t1 = 1_s;
		auto t2 = 1_s;

		t1.val = rand() % 100 + (rand() % 100 / 100.f);
		t2.val = rand() % 100 + (rand() % 100 / 100.f);

		if( !(t1 < t2) )
			ASSERT_GEQ_OUT( t1, t2, [](decltype(t1) x){ std::cerr << x.val; } );
		if( !(t1 > t2) )
			ASSERT_LEQ_OUT( t1, t2, [](decltype(t1) x){ std::cerr << x.val; } );
		if( !(t1 <= t2) )
			ASSERT_GT_OUT( t1, t2, [](decltype(t1) x){ std::cerr << x.val; } );
		if( !(t1 >= t2) )
			ASSERT_LT_OUT( t1, t2, [](decltype(t1) x){ std::cerr << x.val; } );
		if( !(t1 == t2) )
			ASSERT_NEQ_OUT( t1, t2, [](decltype(t1) x){ std::cerr << x.val; } );
		if( !(t1 != t2) )
			ASSERT_EQ_OUT( t1, t2, [](decltype(t1) x){ std::cerr << x.val; } );
	}
	return true;
}
END_TEST_DEF(RelationTest);

int main() {
	srand(time(0));
	return RunAllTests();
}
