#include <string>

/*
 * TODO
 *
 * Unary operators (+ and -)
 * comparison operators (all of them)
 * scale by T
 */

template<typename T, int t_m, int t_s, int t_kg>
struct MesiType {
	T val;

	constexpr MesiType(T in)
		:val(in)
	{}

	constexpr MesiType(MesiType const& in)
		:val(in.val)
	{}

	std::string getUnit() const {
		std::string ret = "";
		if( t_m == 1 )
			ret += "m ";
		else if( t_m != 0 )
			ret += "m^" + std::to_string(t_m) + " ";
		if( t_s == 1 )
			ret += "s ";
		else if( t_s != 0 )
			ret += "s^" + std::to_string(t_s) + " ";
		if( t_kg == 1 )
			ret += "kg ";
		else if( t_kg != 0 )
			ret += "kg^" + std::to_string(t_kg) + " ";
		return ret.substr(0, ret.size() - 1);
	}

	MesiType<T, t_m, t_s, t_kg>& operator+=(MesiType<T, t_m, t_s, t_kg> const& rhs) {
		return (*this) = (*this) + rhs;
	}

	MesiType<T, t_m, t_s, t_kg>& operator-=(MesiType<T, t_m, t_s, t_kg> const& rhs) {
		return (*this) = (*this) - rhs;
	}
};

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator+(
		MesiType<T, t_m, t_s, t_kg> const left,
		MesiType<T, t_m, t_s, t_kg> const right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val + right.val);
}

template<typename T, int t_m, int t_s, int t_kg>
constexpr MesiType<T, t_m, t_s, t_kg> operator-(
		MesiType<T, t_m, t_s, t_kg> const left,
		MesiType<T, t_m, t_s, t_kg> const right
) {
	return MesiType<T, t_m, t_s, t_kg>(left.val - right.val);
}

template<typename T, int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2>
constexpr MesiType<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2> operator*(
		MesiType<T, t_m, t_s, t_kg> const left,
		MesiType<T, t_m2, t_s2, t_kg2> const right
) {
	return MesiType<T, t_m + t_m2, t_s + t_s2, t_kg + t_kg2>(left.val * right.val);
}

template<typename T, int t_m, int t_m2, int t_s, int t_s2, int t_kg, int t_kg2>
constexpr MesiType<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2> operator/(
		MesiType<T, t_m, t_s, t_kg> const left,
		MesiType<T, t_m2, t_s2, t_kg2> const right
) {
	return MesiType<T, t_m - t_m2, t_s - t_s2, t_kg - t_kg2>(left.val / right.val);
}

constexpr MesiType<float, 1, 0, 0> operator "" _m(long double arg) {
	return MesiType<float, 1, 0, 0>(arg);
}

constexpr MesiType<float, 0, 1, 0> operator "" _s(long double arg) {
	return MesiType<float, 0, 1, 0>(arg);
}

constexpr MesiType<float, 0, 0, 1> operator "" _kg(long double arg) {
	return MesiType<float, 0, 0, 1>(arg);
}
