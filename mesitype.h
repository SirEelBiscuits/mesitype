#pragma once

#include <string>
#include <ratio>

namespace Mesi {
	/**
	 * @brief Main class to store SI types
	 *
	 * @param T storage type parameter
	 * @param t_m number of dimensions of meters, e.g., t_m == 2 => m^2
	 * @param t_s similar to t_m, but seconds
	 * @param t_kg similar to t_m, but kilograms
	 *
	 * This class is to enforce compile-time checking, and where possible,
	 * compile-time calculation of SI values using constexpr.
	 *
	 * Note: MESI_LITERAL_TYPE may be defined to set the storage type
	 * used by the operator literal overloads
	 *
	 * @author Jameson Thatcher (a.k.a. SirEel)
	 *
	 */
	template<typename T,
		intmax_t t_m_n, intmax_t t_m_d,
		intmax_t t_s_n, intmax_t t_s_d,
		intmax_t t_kg_n, intmax_t t_kg_d,
		intmax_t t_A_n, intmax_t t_A_d,
		intmax_t t_K_n, intmax_t t_K_d,
		intmax_t t_mol_n, intmax_t t_mol_d,
		intmax_t t_cd_n, intmax_t t_cd_d>
	struct RationalTypeReduced
	{
		using BaseType = T;
		using ScalarType = RationalTypeReduced<T, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1>;

		static_assert(std::ratio<t_m_n, t_m_d>::num == t_m_n);
		static_assert(std::ratio<t_s_n, t_s_d>::num == t_s_n);
		static_assert(std::ratio<t_kg_n, t_kg_d>::num == t_kg_n);
		static_assert(std::ratio<t_A_n, t_A_d>::num == t_A_n);
		static_assert(std::ratio<t_K_n, t_K_d>::num == t_K_n);
		static_assert(std::ratio<t_mol_n, t_mol_d>::num == t_mol_n);
		static_assert(std::ratio<t_cd_n, t_cd_d>::num == t_cd_n);

		T val;

		constexpr RationalTypeReduced()
		{}

		constexpr explicit RationalTypeReduced(T const in)
			:val(in)
		{}

		constexpr RationalTypeReduced(RationalTypeReduced const& in)
			:val(in.val)
		{}

		explicit operator T() const {
			return val;
		}

		/**
		 * getUnit will get a SI-style unit string for this class
		 */
		static std::string getUnit() {
			static std::string s_unitString("");
			if( s_unitString.size() > 0 )
				return s_unitString;
#define DIM_TO_STRING(TP, NAME) if( t_##TP##_n == 1 && t_##TP##_d == 1 ) s_unitString += std::string(#NAME) + " "; else if( t_##TP##_n != 0 && t_##TP##_d == 1) s_unitString += std::string(#NAME) + "^" + std::to_string(static_cast<long long>(t_##TP##_n)) + " "; else s_unitString += std::string(#NAME) + "^(" + std::to_string(static_cast<long long>(t_##TP##_n)) + "/" + std::to_string(static_cast<long long>(t_##TP##_d)) + ") ";

			DIM_TO_STRING(m, "m");
			DIM_TO_STRING(s, "s");
			DIM_TO_STRING(kg, "kg");
			DIM_TO_STRING(A, "A");
			DIM_TO_STRING(K, "K");
			DIM_TO_STRING(mol, "mol");
			DIM_TO_STRING(cd, "cd");

#undef DIM_TO_STRING
			
			s_unitString = s_unitString.substr(0, s_unitString.size() - 1);
			return s_unitString;
		}

		RationalTypeReduced& operator+=(
			RationalTypeReduced const& rhs
		) {
			return (*this) = (*this) + rhs;
		}

		RationalTypeReduced& operator-=(
			RationalTypeReduced const& rhs
		) {
			return (*this) = (*this) - rhs;
		}

		RationalTypeReduced& operator*=(T const& rhs) {
			return (*this) = (*this) * rhs;
		}

		RationalTypeReduced& operator/=(T const& rhs) {
			return (*this) = (*this) / rhs;
		}

		RationalTypeReduced& operator*=(ScalarType const& rhs) {
			return (*this) = (*this) * rhs;
		}

		RationalTypeReduced& operator/=(ScalarType const& rhs) {
			return (*this) = (*this) / rhs;
		}
	};

	template<typename T,
		intmax_t t_m_n, intmax_t t_m_d,
		intmax_t t_s_n, intmax_t t_s_d,
		intmax_t t_kg_n, intmax_t t_kg_d,
		intmax_t t_A_n, intmax_t t_A_d,
		intmax_t t_K_n, intmax_t t_K_d,
		intmax_t t_mol_n, intmax_t t_mol_d,
		intmax_t t_cd_n, intmax_t t_cd_d>
	using RationalType = RationalTypeReduced<T,
		std::ratio<t_m_n, t_m_d>::num, std::ratio<t_m_n, t_m_d>::den,
		std::ratio<t_s_n, t_s_d>::num, std::ratio<t_s_n, t_s_d>::den,
		std::ratio<t_kg_n, t_kg_d>::num, std::ratio<t_kg_n, t_kg_d>::den,
		std::ratio<t_A_n, t_A_d>::num, std::ratio<t_A_n, t_A_d>::den,
		std::ratio<t_K_n, t_K_d>::num, std::ratio<t_K_n, t_K_d>::den,
		std::ratio<t_mol_n, t_mol_d>::num, std::ratio<t_mol_n, t_mol_d>::den,
		std::ratio<t_cd_n, t_cd_d>::num, std::ratio<t_cd_n, t_cd_d>::den>;

#define TYPE_A_FULL_PARAMS intmax_t t_m_n, intmax_t t_m_d, intmax_t t_s_n, intmax_t t_s_d, intmax_t t_kg_n, intmax_t t_kg_d, intmax_t t_A_n, intmax_t t_A_d, intmax_t t_K_n, intmax_t t_K_d, intmax_t t_mol_n, intmax_t t_mol_d, intmax_t t_cd_n, intmax_t t_cd_d
#define TYPE_A_PARAMS t_m_n, t_m_d, t_s_n, t_s_d, t_kg_n, t_kg_d, t_A_n, t_A_d, t_K_n, t_K_d, t_mol_n, t_mol_d, t_cd_n, t_cd_d
#define TYPE_B_FULL_PARAMS intmax_t t_m_n2, intmax_t t_m_d2, intmax_t t_s_n2, intmax_t t_s_d2, intmax_t t_kg_n2, intmax_t t_kg_d2, intmax_t t_A_n2, intmax_t t_A_d2, intmax_t t_K_n2, intmax_t t_K_d2, intmax_t t_mol_n2, intmax_t t_mol_d2, intmax_t t_cd_n2, intmax_t t_cd_d2
#define TYPE_B_PARAMS t_m_n2, t_m_d2, t_s_n2, t_s_d2, t_kg_n2, t_kg_d2, t_A_n2, t_A_d2, t_K_n2, t_K_d2, t_mol_n2, t_mol_d2, t_cd_n2, t_cd_d2
	/*
	 * Arithmatic operators for combining SI values.
	 */
	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr auto operator+(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return RationalTypeReduced<T, TYPE_A_PARAMS>(left.val + right.val);
	}

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr auto operator-(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return RationalTypeReduced<T, TYPE_A_PARAMS>(left.val - right.val);
	}

	template<typename T, TYPE_A_FULL_PARAMS, TYPE_B_FULL_PARAMS>
	constexpr auto operator*(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_B_PARAMS> const& right
	) {
#define ADD_FRAC(TP) using TP = std::ratio_add<std::ratio<t_##TP##_n, t_##TP##_d>, std::ratio<t_##TP##_n2, t_##TP##_d2>>
		ADD_FRAC(m);
		ADD_FRAC(kg);
		ADD_FRAC(s);
		ADD_FRAC(A);
		ADD_FRAC(K);
		ADD_FRAC(mol);
		ADD_FRAC(cd);
		return RationalType<T, m::num, m::den, s::num, s::den, kg::num, kg::den, A::num, A::den, K::num, K::den, mol::num, mol::den, cd::num, cd::den>(left.val * right.val);
#undef ADD_FRAC
	}

	template<typename T, TYPE_A_FULL_PARAMS, TYPE_B_FULL_PARAMS>
	constexpr auto operator/(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_B_PARAMS> const& right
	) {
#define SUB_FRAC(TP) using TP = std::ratio_subtract<std::ratio<t_##TP##_n, t_##TP##_d>, std::ratio<t_##TP##_n2, t_##TP##_d2>>
		SUB_FRAC(m);
		SUB_FRAC(kg);
		SUB_FRAC(s);
		SUB_FRAC(A);
		SUB_FRAC(K);
		SUB_FRAC(mol);
		SUB_FRAC(cd);
		return RationalType<T, m::num, m::den, s::num, s::den, kg::num, kg::den, A::num, A::den, K::num, K::den, mol::num, mol::den, cd::num, cd::den>(left.val / right.val);
#undef SUB_FRAC
	}

	/*
	 * Unary operators, to help with literals (and general usage!)
	 */

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr auto operator-(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& op
	) {
		return RationalTypeReduced<T, TYPE_A_PARAMS>(-op.val);
	}

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr auto operator+(
			RationalTypeReduced<T, TYPE_A_PARAMS> const& op
	) {
		return RationalTypeReduced<T, TYPE_A_PARAMS>(op);
	}

	/*
	 * Scalers by non-SI values (allows things like 2 * 3._m
	 */

	template<typename T, TYPE_A_FULL_PARAMS, typename S>
	constexpr auto operator*(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		S const& right
	) {
		return RationalTypeReduced<T, TYPE_A_PARAMS>(left.val * right);
	}

	template<typename T, TYPE_A_FULL_PARAMS, typename S>
	constexpr auto operator*(
		S const & left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return right * left;
	}

	template<typename T, TYPE_A_FULL_PARAMS, typename S>
	constexpr auto operator/(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		S const& right
	) {
		return RationalTypeReduced<T, TYPE_A_PARAMS>(left.val / right);
	}

	template<typename T, TYPE_A_FULL_PARAMS, typename S>
	constexpr auto operator/(
		S const & left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return RationalTypeReduced<T, -t_m_n, t_m_d, -t_s_n, t_s_d, -t_kg_n, t_kg_d, -t_A_n, t_A_d, -t_K_n, t_K_d, -t_mol_n, t_mol_d, -t_cd_n, t_cd_d>( left / right.val );
	}

	/*
	 * Comparison operators
	 */
	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr bool operator==(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return left.val == right.val;
	}

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr bool operator<(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return left.val < right.val;
	}

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr bool operator!=(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return !(right == left);
	}

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr bool operator<=(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return left < right || left == right;
	}

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr bool operator>(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return right < left;
	}

	template<typename T, TYPE_A_FULL_PARAMS>
	constexpr bool operator>=(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		return left > right || left == right;
	}

#undef TYPE_A_FULL_PARAMS
#undef TYPE_A_PARAMS
#undef TYPE_B_FULL_PARAMS
#undef TYPE_B_PARAMS

	/*
	 * Readable names for common types
	 */

	template<typename T, intmax_t t_m, intmax_t t_s, intmax_t t_kg, intmax_t t_A=0, intmax_t t_K=0, intmax_t t_mol=0, intmax_t t_cd=0>
	using Type = RationalType<T, t_m, 1, t_s, 1, t_kg, 1, t_A, 1, t_K, 1, t_mol, 1, t_cd, 1>;

#ifndef MESI_LITERAL_TYPE
#	define MESI_LITERAL_TYPE float
#endif

	using Scalar    = Type<MESI_LITERAL_TYPE, 0, 0, 0>;
	using Meters    = Type<MESI_LITERAL_TYPE, 1, 0, 0>;
	using MetersSq  = Type<MESI_LITERAL_TYPE, 2, 0, 0>;
	using Seconds   = Type<MESI_LITERAL_TYPE, 0, 1, 0>;
	using SecondsSq = Type<MESI_LITERAL_TYPE, 0, 2, 0>;
	using Hertz     = Type<MESI_LITERAL_TYPE, 0, -1, 0>;
	using Kilos     = Type<MESI_LITERAL_TYPE, 0, 0, 1>;
	using KilosSq   = Type<MESI_LITERAL_TYPE, 0, 0, 2>;
	using Newtons   = Type<MESI_LITERAL_TYPE, 1, -2, 1>;
	using NewtonsSq = Type<MESI_LITERAL_TYPE, 2, -4, 2>;
	using Amperes   = Type<MESI_LITERAL_TYPE, 0, 0, 0, 1>;
	using Kelvin    = Type<MESI_LITERAL_TYPE, 0, 0, 0, 0, 1>;
	using Moles     = Type<MESI_LITERAL_TYPE, 0, 0, 0, 0, 0, 1>;
	using Candela   = Type<MESI_LITERAL_TYPE, 0, 0, 0, 0, 0, 0, 1>;
	using Pascals   = decltype(Newtons(1)/MetersSq(1));
	using Joules    = decltype(Newtons(1)*Meters(1));
	using Watts     = decltype(Joules(1)/Seconds(1));
	using Coulombs  = decltype(Amperes(1)*Seconds(1));
	using Volts     = decltype(Watts(1)/Amperes(1));
	using Farads    = decltype(Coulombs(1)/Volts(1));
	using Ohm       = decltype(Volts(1)/Amperes(1));
	using Siemens   = decltype(Amperes(1)/Volts(1));
	using Webers    = decltype(Volts(1)*Seconds(1));
	using Tesla     = decltype(Webers(1)/MetersSq(1));
	using Henry     = decltype(Webers(1)/Amperes(1));

	namespace Literals {
	/*
	 * Literal operators, to allow quick creation of basic types
	 * Note that this defaults to the type set below, if no other is set
	 * before calling!
	 */
#pragma push_macro("_N")
#undef _N

		constexpr Mesi::Meters operator "" _m(long double arg) {
			return Mesi::Meters(arg);
		}

		constexpr Mesi::Seconds operator "" _s(long double arg) {
			return Mesi::Seconds(arg);
		}

		constexpr Mesi::Kilos operator "" _kg(long double arg) {
			return Mesi::Kilos(arg);
		}

		constexpr Mesi::Newtons operator "" _N(long double arg) {
			return Mesi::Newtons(arg);
		}

		constexpr Mesi::Meters operator "" _m(unsigned long long arg) {
			return Mesi::Meters(arg);
		}

		constexpr Mesi::Seconds operator "" _s(unsigned long long arg) {
			return Mesi::Seconds(arg);
		}

		constexpr Mesi::Kilos operator "" _kg(unsigned long long arg) {
			return Mesi::Kilos(arg);
		}

		constexpr Mesi::Newtons operator "" _N(unsigned long long arg) {
			return Mesi::Newtons(arg);
		}

		constexpr Mesi::MetersSq operator "" _m2(long double arg) {
			return Mesi::MetersSq(arg);
		}

		constexpr Mesi::SecondsSq operator "" _s2(long double arg) {
			return Mesi::SecondsSq(arg);
		}

		constexpr Mesi::KilosSq operator "" _kg2(long double arg) {
			return Mesi::KilosSq(arg);
		}

		constexpr Mesi::NewtonsSq operator "" _N2(long double arg) {
			return Mesi::NewtonsSq(arg);
		}

		constexpr Mesi::MetersSq operator "" _m2(unsigned long long arg) {
			return Mesi::MetersSq(arg);
		}

		constexpr Mesi::SecondsSq operator "" _s2(unsigned long long arg) {
			return Mesi::SecondsSq(arg);
		}

		constexpr Mesi::KilosSq operator "" _kg2(unsigned long long arg) {
			return Mesi::KilosSq(arg);
		}

		constexpr Mesi::NewtonsSq operator "" _N2(unsigned long long arg) {
			return Mesi::NewtonsSq(arg);
		}
	}
#pragma pop_macro("_N")
}
