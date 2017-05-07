#pragma once

#include <string>

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
	template<typename T, int t_m, int t_s, int t_kg>
	struct Type {
		using BaseType = T;
		T val;

		constexpr Type()
		{}

		constexpr explicit Type(T const in)
			:val(in)
		{}

		constexpr Type(Type const& in)
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

			if( t_m == 1 )
				s_unitString += "m ";
			else if( t_m != 0 )
				s_unitString += "m^"
					+ std::to_string(static_cast<long long>(t_m)) + " ";
			if( t_s == 1 )
				s_unitString += "s ";
			else if( t_s != 0 )
				s_unitString += "s^"
					+ std::to_string(static_cast<long long>(t_s)) + " ";
			if( t_kg == 1 )
				s_unitString += "kg ";
			else if( t_kg != 0 )
				s_unitString += "kg^"
					+ std::to_string(static_cast<long long>(t_kg)) + " ";
			s_unitString = s_unitString.substr(0, s_unitString.size() - 1);
			return s_unitString;
		}

		Type<T, t_m, t_s, t_kg>& operator+=(
			Type<T, t_m, t_s, t_kg> const& rhs
		) {
			return (*this) = (*this) + rhs;
		}

		Type<T, t_m, t_s, t_kg>& operator-=(
			Type<T, t_m, t_s, t_kg> const& rhs
		) {
			return (*this) = (*this) - rhs;
		}

		Type<T, t_m, t_s, t_kg>& operator*=(T const& rhs) {
			return (*this) = (*this) * rhs;
		}

		Type<T, t_m, t_s, t_kg>& operator/=(T const& rhs) {
			return (*this) = (*this) / rhs;
		}

		Type<T, t_m, t_s, t_kg>& operator*=(Type<T, 0, 0, 0> const& rhs) {
			return (*this) = (*this) * rhs;
		}

		Type<T, t_m, t_s, t_kg>& operator/=(Type<T, 0, 0, 0> const& rhs) {
			return (*this) = (*this) / rhs;
		}
	};

	/*
	 * Readable names for common types
	 */

#ifndef MESI_LITERAL_TYPE
#	define MESI_LITERAL_TYPE float
#endif

	using Scalar    = Type<MESI_LITERAL_TYPE, 0, 0, 0>;
	using Meters    = Type<MESI_LITERAL_TYPE, 1, 0, 0>;
	using MetersSq  = Type<MESI_LITERAL_TYPE, 2, 0, 0>;
	using Seconds   = Type<MESI_LITERAL_TYPE, 0, 1, 0>;
	using SecondsSq = Type<MESI_LITERAL_TYPE, 0, 2, 0>;
	using Kilos     = Type<MESI_LITERAL_TYPE, 0, 0, 1>;
	using KilosSq   = Type<MESI_LITERAL_TYPE, 0, 0, 2>;
	using Newtons   = Type<MESI_LITERAL_TYPE, 1, -2, 1>;
	using NewtonsSq = Type<MESI_LITERAL_TYPE, 2, -4, 2>;

	/*
	 * Arithmatic operators for combining SI values.
	 */

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr Type<T, t_m, t_s, t_kg> operator+(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return Type<T, t_m, t_s, t_kg>(left.val + right.val);
	}

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr Type<T, t_m, t_s, t_kg> operator-(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return Type<T, t_m, t_s, t_kg>(left.val - right.val);
	}

	template<typename T,
	 	int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2
	>
	constexpr Type<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2> operator*(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m2, t_s2, t_kg2> const& right
	) {
		return Type<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2>(
			left.val * right.val
		);
	}

	template<typename T,
	 	int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2
	>
	constexpr Type<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2> operator/(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m2, t_s2, t_kg2> const& right
	) {
		return Type<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2>(
			left.val / right.val
		);
	}

	/*
	 * Unary operators, to help with literals (and general usage!)
	 */

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr Type<T, t_m, t_s, t_kg> operator-(
		Type<T, t_m, t_s, t_kg> const& op
	) {
		return Type<T, t_m, t_s, t_kg>(-op.val);
	}

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr Type<T, t_m, t_s, t_kg> operator+(
			Type<T, t_m, t_s, t_kg> const& op
	) {
		return op;
	}

	/*
	 * Scalers by non-SI values (allows things like 2 * 3._m
	 */

	template<typename T, int t_m, int t_s, int t_kg, typename S>
	constexpr Type<T, t_m, t_s, t_kg> operator*(
		Type<T, t_m, t_s, t_kg> const& left,
		S const& right
	) {
		return Type<T, t_m, t_s, t_kg>(left.val * right);
	}

	template<typename T, int t_m, int t_s, int t_kg, typename S>
	constexpr Type<T, t_m, t_s, t_kg> operator*(
		S const & left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return right * left;
	}

	template<typename T, int t_m, int t_s, int t_kg, typename S>
	constexpr Type<T, t_m, t_s, t_kg> operator/(
		Type<T, t_m, t_s, t_kg> const& left,
		S const& right
	) {
		return Type<T, t_m, t_s, t_kg>(left.val / right);
	}

	template<typename T, int t_m, int t_s, int t_kg, typename S>
	constexpr Type<T, -t_m, -t_s, -t_kg> operator/(
		S const & left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return Type<T, -t_m, -t_s, -t_kg>( left * right.val );
	}

	/*
	 * Comparison operators
	 */
	template<typename T, int t_m, int t_s, int t_kg>
	constexpr bool operator==(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return left.val == right.val;
	}

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr bool operator<(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return left.val < right.val;
	}

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr bool operator!=(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return !(right == left);
	}

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr bool operator<=(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return left < right || left == right;
	}

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr bool operator>(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return right < left;
	}

	template<typename T, int t_m, int t_s, int t_kg>
	constexpr bool operator>=(
		Type<T, t_m, t_s, t_kg> const& left,
		Type<T, t_m, t_s, t_kg> const& right
	) {
		return left > right || left == right;
	}

	/*
	 * Literal operators, to allow quick creation of basic types
	 * Note that this defaults to the type set below, if no other is set
	 * before calling!
	 */

#pragma push_macro("_N")
#undef _N

	constexpr Meters operator "" _m(long double arg) {
		return Meters(arg);
	}

	constexpr Seconds operator "" _s(long double arg) {
		return Seconds(arg);
	}

	constexpr Kilos operator "" _kg(long double arg) {
		return Kilos(arg);
	}

	constexpr Newtons operator "" _N(long double arg) {
		return Newtons(arg);
	}

	constexpr Meters operator "" _m(unsigned long long arg) {
		return Meters(arg);
	}

	constexpr Seconds operator "" _s(unsigned long long arg) {
		return Seconds(arg);
	}

	constexpr Kilos operator "" _kg(unsigned long long arg) {
		return Kilos(arg);
	}

	constexpr Newtons operator "" _N(unsigned long long arg) {
		return Newtons(arg);
	}

	constexpr MetersSq operator "" _m2(long double arg) {
		return MetersSq(arg);
	}

	constexpr SecondsSq operator "" _s2(long double arg) {
		return SecondsSq(arg);
	}

	constexpr KilosSq operator "" _kg2(long double arg) {
		return KilosSq(arg);
	}

	constexpr NewtonsSq operator "" _N2(long double arg) {
		return NewtonsSq(arg);
	}

	constexpr MetersSq operator "" _m2(unsigned long long arg) {
		return MetersSq(arg);
	}

	constexpr SecondsSq operator "" _s2(unsigned long long arg) {
		return SecondsSq(arg);
	}

	constexpr KilosSq operator "" _kg2(unsigned long long arg) {
		return KilosSq(arg);
	}

	constexpr NewtonsSq operator "" _N2(unsigned long long arg) {
		return NewtonsSq(arg);
	}

#pragma pop_macro("_N")

}

