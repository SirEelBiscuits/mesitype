#include <string>

// TODO add copyright notice?

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
struct MesiType {
	T val;

	constexpr MesiType()
	{}

	constexpr explicit MesiType(T const in)
		:val(in)
	{}

	constexpr MesiType(MesiType const& in)
		:val(in.val)
	{}


	/**
	 * getUnit will get a SI-style unit string for this class
	 */
	static std::string getUnit() {
		static std::string s_unitString("");
		if( s_unitString.size() > 0 )
			return  s_unitString;

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
		return s_unitString.substr(0, s_unitString.size() - 1);
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

	MesiType<T, t_m, t_s, t_kg>& operator /=(T const& rhs) {
		return (*this) = (*this) / rhs;
	}
};

/*
 * Arithmatic operators for combining SI values.
 */

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator+(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val + right.val);
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator-(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val - right.val);
}

template<typename T, int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2>
constexpr MesiType<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2> operator*(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m2, t_s2, t_kg2> const& right
) {
	return MesiType<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2>(left.val * right.val);
}

template<typename T, int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2>
constexpr MesiType<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2> operator/(
		MesiType<T, t_m, t_s, t_kg> const& left,
		MesiType<T, t_m2, t_s2, t_kg2> const& right
) {
	return MesiType<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2>(left.val / right.val);
}

/*
 * Unary operators, to help with literals (and general usage!)
 */

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator-(
		MesiType<T, t_m, t_s, t_kg> const& op
) {
	return MesiType<T, t_m, t_s, t_kg>(-op.val);
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator+(
		MesiType<T, t_m, t_s, t_kg> const& op
) {
	return op;
}

/*
 * Scalers by non-SI values (allows things like 2 * 3._m
 */

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator*(
		MesiType<T, t_m, t_s, t_kg> const& left,
		T const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val * right);
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator*(
		T const & left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return right * left;
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator/(
		MesiType<T, t_m, t_s, t_kg> const& left,
		T const& right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val / right);
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, -t_m, -t_s, -t_kg> operator/(
		T const & left,
		MesiType<T, t_m, t_s, t_kg> const& right
) {
	return MesiType<T, -t_m, -t_s, -t_kg>( left * right.val );
}

/*
 * Comparison operators
 */
template<typename T, int t_m, int t_s, int t_kg>
constexpr bool operator==(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left.val == right.val;
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr bool operator<(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left.val < right.val;
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr bool operator!=(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return !(right == left);
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr bool operator<=(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left < right || left == right;
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr bool operator>(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return right < left;
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr bool operator>=(
	MesiType<T, t_m, t_s, t_kg> const& left,
	MesiType<T, t_m, t_s, t_kg> const& right
) {
	return left > right || left == right;
}

/*
 * Literal operators, to allow quick creation of basic types
 * Note
 */

#ifndef MESI_LITERAL_TYPE
#	define MESI_LITERAL_TYPE float
#endif

constexpr MesiType<MESI_LITERAL_TYPE, 1, 0, 0> operator "" _m(long double arg) {
	return MesiType<MESI_LITERAL_TYPE, 1, 0, 0>(arg);
}

constexpr MesiType<MESI_LITERAL_TYPE, 0, 1, 0> operator "" _s(long double arg) {
	return MesiType<MESI_LITERAL_TYPE, 0, 1, 0>(arg);
}

constexpr MesiType<MESI_LITERAL_TYPE, 0, 0, 1> operator "" _kg(long double arg) {
	return MesiType<MESI_LITERAL_TYPE, 0, 0, 1>(arg);
}

constexpr MesiType<MESI_LITERAL_TYPE, 1, 0, 0> operator "" _m(unsigned long long arg) {
	return MesiType<MESI_LITERAL_TYPE, 1, 0, 0>(arg);
}

constexpr MesiType<MESI_LITERAL_TYPE, 0, 1, 0> operator "" _s(unsigned long long arg) {
	return MesiType<MESI_LITERAL_TYPE, 0, 1, 0>(arg);
}

constexpr MesiType<MESI_LITERAL_TYPE, 0, 0, 1> operator "" _kg(unsigned long long arg) {
	return MesiType<MESI_LITERAL_TYPE, 0, 0, 1>(arg);
}
