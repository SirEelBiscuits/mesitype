#pragma once

#include <string>

// TODO add copyright notice?

//#define MESI_COMPATIBILITY

#ifdef MESI_COMPATIBILITY
#	define CONSTEXPR
#else
# define CONSTEXPR constexpr
#endif

/**
 * @brief Main class to store SI types
 *
 * @param T storage type parameter
 * @param t_m number of dimensions of meters, e.g., t_m == 2 => m^2
 * @param t_s similar to t_m, but seconds
 * @param t_kg similar to t_m, but kilograms
 *
 * This class is to enforce compile-time checking, and where possible,
 * compile-time calculation of SI values using CONSTEXPR.
 *
 * Note: MESI_LITERAL_TYPE may be defined to set the storage type
 * used by the operator literal overloads
 *
 * @author Jameson Thatcher (a.k.a. SirEel)
 *
 */
template<typename T, int t_m, int t_s, int t_kg>
struct MesiType {
	using BaseType = T;
	T val;

	CONSTEXPR MesiType()
	{}

	CONSTEXPR explicit MesiType(T const in)
		:val(in)
	{}

	CONSTEXPR MesiType(MesiType const& in)
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

	MesiType<T, t_m, t_s, t_kg>& operator+=(MesiType<T, t_m, t_s, t_kg> const& rhs) {
		return (*this) = (*this) + rhs;
	}

	MesiType<T, t_m, t_s, t_kg>& operator-=(MesiType<T, t_m, t_s, t_kg> const& rhs) {
		return (*this) = (*this) - rhs;
	}

	MesiType<T, t_m, t_s, t_kg>& operator*=(T const& rhs) {
		return (*this) = (*this) * rhs;
	}

	MesiType<T, t_m, t_s, t_kg>& operator/=(T const& rhs) {
		return (*this) = (*this) / rhs;
	}

	MesiType<T, t_m, t_s, t_kg>& operator*=(MesiType<T, 0, 0, 0> const& rhs) {
		return (*this) = (*this) * rhs;
	}

	MesiType<T, t_m, t_s, t_kg>& operator/=(MesiType<T, 0, 0, 0> const& rhs) {
		return (*this) = (*this) / rhs;
	}
};

/*
 * Arithmatic operators for combining SI values.
 */

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, t_m, t_s, t_kg> operator+(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val + right.val);
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, t_m, t_s, t_kg> operator-(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val - right.val);
}

template<typename T, int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2>
CONSTEXPR MesiType<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2> operator*(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m2, t_s2, t_kg2> const& right
) {
	return MesiType<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2>(left.val * right.val);
}

template<typename T, int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2>
CONSTEXPR MesiType<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2> operator/(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m2, t_s2, t_kg2> const& right
) {
	return MesiType<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2>(left.val / right.val);
}

/*
 * Unary operators, to help with literals (and general usage!)
 */

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, t_m, t_s, t_kg> operator-(
		MesiType<T, t_m, t_s, t_kg> const& op
) {
	return MesiType<T, t_m, t_s, t_kg>(-op.val);
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, t_m, t_s, t_kg> operator+(
		MesiType<T, t_m, t_s, t_kg> const& op
) {
	return op;
}

/*
 * Scalers by non-SI values (allows things like 2 * 3._m
 */

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, t_m, t_s, t_kg> operator*(
		MesiType<T, t_m, t_s, t_kg> const& left,
		T const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val * right);
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, t_m, t_s, t_kg> operator*(
		T const & left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return right * left;
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, t_m, t_s, t_kg> operator/(
		MesiType<T, t_m, t_s, t_kg> const& left,
		T const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val / right);
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR MesiType<T, -t_m, -t_s, -t_kg> operator/(
		T const & left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return MesiType<T, -t_m, -t_s, -t_kg>( left * right.val );
}

/*
 * Comparison operators
 */
template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR bool operator==(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left.val == right.val;
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR bool operator<(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left.val < right.val;
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR bool operator!=(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return !(right == left);
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR bool operator<=(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left < right || left == right;
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR bool operator>(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return right < left;
}

template<typename T, int t_m, int t_s, int t_kg>
CONSTEXPR bool operator>=(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left > right || left == right;
}

/*
 * Literal operators, to allow quick creation of basic types
 * Note that this defaults to the type set below, if no other is set before calling!
 */

#ifndef MESI_LITERAL_TYPE
#	define MESI_LITERAL_TYPE float
#endif

namespace Mesi {
<<<<<<< HEAD
	using Scalar    = MesiType<MESI_LITERAL_TYPE, 0, 0, 0>;
	using Meters    = MesiType<MESI_LITERAL_TYPE, 1, 0, 0>;
	using MetersSq  = MesiType<MESI_LITERAL_TYPE, 2, 0, 0>;
	using Seconds   = MesiType<MESI_LITERAL_TYPE, 0, 1, 0>;
	using SecondsSq = MesiType<MESI_LITERAL_TYPE, 0, 2, 0>;
	using Kilos     = MesiType<MESI_LITERAL_TYPE, 0, 0, 1>;
	using KilosSq   = MesiType<MESI_LITERAL_TYPE, 0, 0, 2>;
	using Newtons   = MesiType<MESI_LITERAL_TYPE, 1, -2, 1>;
	using NewtonsSq = MesiType<MESI_LITERAL_TYPE, 2, -4, 2>;
}

#ifdef MESI_COMPATIBILITY
#   define METERS(x)    Mesi::Meters(x)
#   define METERS2(x)   Mesi::MetersSq(x)
#   define SECONDS(x)   Mesi::Seconds(x)
#   define SECONDS2(x)  Mesi::SecondsSq(x)
#   define KILOS(x)     Mesi::Kilos(x)
#   define KILOS2(x)    Mesi::KilosSq(x)
#   define NEWTONS(x)   Mesi::Newtons(x)
#   define NEWTONS2(x)  Mesi::NewtonsSq(x)
#   define SCALAR(x)    Mesi::Scalar(x)
#else
#   define METERS(x)    x ## _m
#   define METERS2(x)   x ## _m2
#   define SECONDS(x)   x ## _s
#   define SECONDS2(x)  x ## _s2
#   define KILOS(x)     x ## _kg
#   define KILOS2(x)    x ## _kg2
#   define NEWTONS(x)   x ## _N
#   define NEWTONS2(x)  x ## _N2
#   define SCALAR(x)    Mesi::Scalar(x)

CONSTEXPR Mesi::Meters operator "" _m(long double arg) {
  return Mesi::Meters(arg);
}

CONSTEXPR Mesi::Seconds operator "" _s(long double arg) {
  return Mesi::Seconds(arg);
}

CONSTEXPR Mesi::Kilos operator "" _kg(long double arg) {
  return Mesi::Kilos(arg);
}

CONSTEXPR Mesi::Newtons operator "" _N(long double arg) {
  return Mesi::Newtons(arg);
}

CONSTEXPR Mesi::Meters operator "" _m(unsigned long long arg) {
  return Mesi::Meters(arg);
}

CONSTEXPR Mesi::Seconds operator "" _s(unsigned long long arg) {
  return Mesi::Seconds(arg);
}

CONSTEXPR Mesi::Kilos operator "" _kg(unsigned long long arg) {
  return Mesi::Kilos(arg);
}

CONSTEXPR Mesi::Newtons operator "" _N(unsigned long long arg) {
  return Mesi::Newtons(arg);
}

CONSTEXPR Mesi::Meters operator "" _m2(long double arg) {
  return Mesi::MetersSq(arg);
}

CONSTEXPR Mesi::Seconds operator "" _s2(long double arg) {
  return Mesi::SecondsSq(arg);
}

CONSTEXPR Mesi::Kilos operator "" _kg2(long double arg) {
  return Mesi::KilosSq(arg);
}

CONSTEXPR Mesi::Newtons operator "" _N2(long double arg) {
  return Mesi::NewtonsSq(arg);
}

CONSTEXPR Mesi::Meters operator "" _m2(unsigned long long arg) {
  return Mesi::MetersSq(arg);
}

CONSTEXPR Mesi::Seconds operator "" _s2(unsigned long long arg) {
  return Mesi::SecondsSq(arg);
}

CONSTEXPR Mesi::Kilos operator "" _kg2(unsigned long long arg) {
  return Mesi::KilosSq(arg);
}

CONSTEXPR Mesi::Newtons operator "" _N2(unsigned long long arg) {
  return Mesi::NewtonsSq(arg);
}

#endif
