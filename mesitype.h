#pragma once

#include <string>
#include <ratio>

namespace Mesi {
	namespace _internal {
		template<intmax_t t_num>
		struct NumReduce
		{
		private:
			template<intmax_t N, intmax_t M>
			struct H
			{
				static constexpr intmax_t num = N;
				static constexpr intmax_t power = 0;
			};
			template<intmax_t N>
			struct H<N, 0>
			{
				static constexpr intmax_t num = H<N/10,(N/10)%10>::num;
				static constexpr intmax_t power = 1 + H<N/10,(N/10)%10>::power;
			};
		public:
			static constexpr intmax_t num = H<t_num, t_num%10>::num;
			static constexpr intmax_t power = H<t_num, t_num%10>::power;
		};
		template<typename t_ratio, intmax_t t_power>
		struct RatioReduce
		{
			using num = NumReduce<t_ratio::num>;
			using den = NumReduce<t_ratio::den>;
			using ratio = typename std::ratio<num::num, den::num>;
			static constexpr intmax_t power = t_power + num::power - den::power;
		};
	}
/* Utility macro for applying another macro to all known units, for internal use only */
#define ALL_UNITS(op) op(m) op(s) op(kg) op(A) op(K) op(mol) op(cd)

	/**
	 * @brief Main class to store SI types
	 *
	 * @param T storage type parameter
	 * @param t_m number of dimensions of meters as a rational number, e.g., t_m == std::ratio<2,1> => m^2
	 * @param t_s similar to t_m, but seconds
	 * @param t_kg similar to t_m, but kilograms
	 * @param t_A similar to t_m, but amperes
	 * @param t_K similar to t_m, but Kelvin
	 * @param t_mol similar to t_m, but moles
	 * @param t_cd similar to t_m, but candela
	 * @param t_ratio defines a scaling factor, i.e. 1,1000 for 1/1000 == milli-
	 * @param t_power_of_10 does the same as t_ratio, but as a power of 10.
	 *        Using 3 here would be the same as a kilo- prefix.
	 *
	 * This class is to enforce compile-time checking, and where possible,
	 * compile-time calculation of SI values using constexpr.
	 *
	 * Note: MESI_LITERAL_TYPE may be defined to set the storage type
	 * used by the operator literal overloads
	 *
	 * Note: t_ratio should have any powers of 10 factored out into
	 * t_power_of_10. To do this automatically, use RationalType<>
	 *
	 * @author Jameson Thatcher (a.k.a. SirEel)
	 *
	 */
	template<typename T,
		typename t_m, typename t_s, typename t_kg, typename t_A, typename t_K, typename t_mol, typename t_cd,
		typename t_ratio, intmax_t t_power_of_10>
	struct RationalTypeReduced
	{
		using BaseType = T;
	private:
		using Zero = std::ratio<0,1>;
		using One = std::ratio<1,1>;
	public:
		using ScalarType = RationalTypeReduced<T, Zero, Zero, Zero, Zero, Zero, Zero, Zero, One, 0>;

		template<typename t_scale_ratio, intmax_t t_scale_10_to_the = 0>
		using Scale = RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd,
		      typename _internal::RatioReduce<std::ratio_multiply<t_ratio, t_scale_ratio>, t_power_of_10+t_scale_10_to_the>::ratio, 
		      _internal::RatioReduce<std::ratio_multiply<t_ratio, t_scale_ratio>, t_power_of_10+t_scale_10_to_the>::power>; 

		template<intmax_t t_scale>
		using Multiply = Scale<std::ratio<t_scale, 1>>;

		template<intmax_t t_scale>
		using Divide = Scale<std::ratio<1, t_scale>>;

		template<intmax_t t_pow>
		using ScaleByTenToThe = Scale<std::ratio<1,1>, t_pow>;

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
		
		template<typename t_ratio2, intmax_t t_power_of_102>
		explicit constexpr operator RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, t_ratio2, t_power_of_102>() const {
			T nv = val;
			using ratio = std::ratio_divide<t_ratio, t_ratio2>;
			intmax_t pref_diff = t_power_of_10 - t_power_of_102;
			while(pref_diff > 0)
			{
				nv *= 10;
				pref_diff--;
			}
			while(pref_diff < 0)
			{
				nv /= 10;
				pref_diff++;
			}
			nv = nv * ratio::num / ratio::den;
			return RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, t_ratio2, t_power_of_102>(nv);
		}

		/**
		 * getUnit will get a SI-style unit string for this class
		 */
		static std::string getUnit() {
			static std::string s_unitString("");
			if( s_unitString.size() > 0 )
				return s_unitString;

			if( t_power_of_10 != 0 )
			{
				s_unitString += " * 10^" + std::to_string(static_cast<long long>(t_power_of_10)) + " ";
			}

#define DIM_TO_STRING(TP) if( t_##TP ::num == 1 && t_##TP ::den == 1 ) s_unitString += std::string(#TP) + " "; else if( t_##TP ::num != 0 && t_##TP ::den == 1) s_unitString += std::string(#TP) + "^" + std::to_string(static_cast<long long>(t_##TP ::num)) + " "; else if(t_##TP ::num != 0) s_unitString += std::string(#TP) + "^(" + std::to_string(static_cast<long long>(t_##TP ::num)) + "/" + std::to_string(static_cast<long long>(t_##TP ::den)) + ") ";
			ALL_UNITS(DIM_TO_STRING)
#undef DIM_TO_STRING
			
			s_unitString = s_unitString.substr(0, s_unitString.size() - 1);
			return s_unitString;
		}

		constexpr RationalTypeReduced& operator+=(
			RationalTypeReduced const& rhs
		) {
			return (*this) = (*this) + rhs;
		}

		constexpr RationalTypeReduced& operator-=(
			RationalTypeReduced const& rhs
		) {
			return (*this) = (*this) - rhs;
		}

		constexpr RationalTypeReduced& operator*=(T const& rhs) {
			return (*this) = (*this) * rhs;
		}

		constexpr RationalTypeReduced& operator/=(T const& rhs) {
			return (*this) = (*this) / rhs;
		}

		constexpr RationalTypeReduced& operator*=(ScalarType const& rhs) {
			return (*this) = (*this) * rhs;
		}

		constexpr RationalTypeReduced& operator/=(ScalarType const& rhs) {
			return (*this) = (*this) / rhs;
		}
	};

	template<typename T, typename t_m, typename t_s, typename t_kg, typename t_A, typename t_K, typename t_mol, typename t_cd, typename t_ratio, intmax_t t_power_of_10>
	using RationalType = RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, typename _internal::RatioReduce<t_ratio, t_power_of_10>::ratio, _internal::RatioReduce<t_ratio, t_power_of_10>::power>;

#define TYPE_A_FULL_PARAMS typename t_m, typename t_s, typename t_kg, typename t_A, typename t_K, typename t_mol, typename t_cd, typename t_ratio, intmax_t t_power_of_10
#define TYPE_A_PARAMS t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, t_ratio, t_power_of_10
#define TYPE_B_FULL_PARAMS typename t_m2, typename t_s2, typename t_kg2, typename t_A2, typename t_K2, typename t_mol2, typename t_cd2, typename t_ratio2, intmax_t t_power_of_102
#define TYPE_B_PARAMS t_m2, t_s2, t_kg2, t_A2, t_K2, t_mol2, t_cd2, t_ratio2, t_power_of_102
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
#define ADD_FRAC(TP) using TP = std::ratio_add<t_##TP, t_##TP##2>;
		ALL_UNITS(ADD_FRAC)
#undef ADD_FRAC
		return RationalType<T, m, s, kg, A, K, mol, cd, std::ratio_multiply<t_ratio, t_ratio2>, t_power_of_10+t_power_of_102>(left.val * right.val);
	}

	template<typename T, TYPE_A_FULL_PARAMS, TYPE_B_FULL_PARAMS>
	constexpr auto operator/(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_B_PARAMS> const& right
	) {
#define SUB_FRAC(TP) using TP = std::ratio_subtract<t_##TP, t_##TP##2>;
		ALL_UNITS(SUB_FRAC)
#undef SUB_FRAC
		return RationalType<T, m, s, kg, A, K, mol, cd, std::ratio_divide<t_ratio,t_ratio2>, t_power_of_10-t_power_of_102>(left.val / right.val);
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
		using Scalar = typename RationalTypeReduced<T, TYPE_A_PARAMS>::ScalarType;
		return left / Scalar(T(right));
	}

	template<typename T, TYPE_A_FULL_PARAMS, typename S>
	constexpr auto operator/(
		S const & left,
		RationalTypeReduced<T, TYPE_A_PARAMS> const& right
	) {
		using Scalar = typename RationalTypeReduced<T, TYPE_A_PARAMS>::ScalarType;
		return Scalar(T(left)) / right;
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
#undef ALL_UNITS

	/*
	 * Readable names for common types
	 */

	template<typename T, intmax_t t_m, intmax_t t_s, intmax_t t_kg, intmax_t t_A=0, intmax_t t_K=0, intmax_t t_mol=0, intmax_t t_cd=0, typename t_ratio = std::ratio<1,1>, intmax_t t_pref = 0>
	using Type = RationalType<T, std::ratio<t_m, 1>, std::ratio<t_s, 1>, std::ratio<t_kg, 1>, std::ratio<t_A, 1>, std::ratio<t_K, 1>, std::ratio<t_mol, 1>, std::ratio<t_cd, 1>, t_ratio, t_pref>;

#ifndef MESI_LITERAL_TYPE
#	define MESI_LITERAL_TYPE float
#endif

	using Scalar    = Type<MESI_LITERAL_TYPE, 0, 0, 0>;
	using Meters    = Type<MESI_LITERAL_TYPE, 1, 0, 0>;
	using Seconds   = Type<MESI_LITERAL_TYPE, 0, 1, 0>;
	using Kilograms = Type<MESI_LITERAL_TYPE, 0, 0, 1>;
	using Amperes   = Type<MESI_LITERAL_TYPE, 0, 0, 0, 1>;
	using Kelvin    = Type<MESI_LITERAL_TYPE, 0, 0, 0, 0, 1>;
	using Moles     = Type<MESI_LITERAL_TYPE, 0, 0, 0, 0, 0, 1>;
	using Candela   = Type<MESI_LITERAL_TYPE, 0, 0, 0, 0, 0, 0, 1>;
	using Minutes   = Seconds::Multiply<60>;
	using Hours     = Minutes::Multiply<60>;
	using Newtons   = decltype(Meters{} * Kilograms{} / Seconds{} / Seconds{});
	using NewtonsSq = decltype(Newtons{} * Newtons{});
	using MetersSq  = decltype(Meters{} * Meters{});
	using MetersCu  = decltype(Meters{} * MetersSq{});
	using SecondsSq = decltype(Seconds{} * Seconds{});
	using KilogramsSq = decltype(Kilograms{} * Kilograms{});
	using Hertz     = decltype(Scalar{} / Seconds{});
	using Pascals   = decltype(Newtons{} / MetersSq{});
	using Joules    = decltype(Newtons{} * Meters{});
	using Watts     = decltype(Joules{} / Seconds{});
	using Coulombs  = decltype(Amperes{} * Seconds{});
	using Volts     = decltype(Watts{} / Amperes{});
	using Farads    = decltype(Coulombs{} / Volts{});
	using Ohms      = decltype(Volts{} / Amperes{});
	using Siemens   = decltype(Amperes{} / Volts{});
	using Webers    = decltype(Volts{} * Seconds{});
	using Tesla     = decltype(Webers{} / MetersSq{});
	using Henry     = decltype(Webers{} / Amperes{});

	template<intmax_t t_power, typename T>
	using Prefix = typename T::template ScaleByTenToThe<t_power>;

	template<typename T> using Deca  = Prefix<3, T>;
	template<typename T> using Hecto = Prefix<3, T>;
	template<typename T> using Kilo  = Prefix<3, T>;
	template<typename T> using Mega  = Prefix<6, T>;
	template<typename T> using Giga  = Prefix<9, T>;
	template<typename T> using Tera  = Prefix<12, T>;
	template<typename T> using Peta  = Prefix<15, T>;
	template<typename T> using Exa   = Prefix<18, T>;
	template<typename T> using Zetta = Prefix<21, T>;
	template<typename T> using Yotta = Prefix<24, T>;
	
	template<typename T> using Deci  = Prefix<-1, T>;
	template<typename T> using Centi = Prefix<-2, T>;
	template<typename T> using Milli = Prefix<-3, T>;
	template<typename T> using Micro = Prefix<-6, T>;
	template<typename T> using Nano  = Prefix<-9, T>;
	template<typename T> using Pico  = Prefix<-12, T>;
	template<typename T> using Femto = Prefix<-15, T>;
	template<typename T> using Atto  = Prefix<-18, T>;
	template<typename T> using Zepto = Prefix<-21, T>;
	template<typename T> using Yocto = Prefix<-24, T>;

	namespace Literals {
	/*
	 * Literal operators, to allow quick creation of basic types
	 * Note that this defaults to the type set below, if no other is set
	 * before calling!
	 *
	 * These are all lowercase, as identifiers beginning with
	 * _[A-Z] are reserved.
	 */
#define LITERAL_TYPE(T, SUFFIX) \
			constexpr auto operator "" SUFFIX(long double arg) { return T(arg); } \
			constexpr auto operator "" SUFFIX(unsigned long long arg) { return T(arg); }

		LITERAL_TYPE(Mesi::Meters, _m)
		LITERAL_TYPE(Mesi::MetersSq, _m2)
		LITERAL_TYPE(Mesi::MetersCu, _m3)
		LITERAL_TYPE(Mesi::Seconds, _s)
		LITERAL_TYPE(Mesi::SecondsSq, _s2)
		LITERAL_TYPE(Mesi::Kilograms, _kg)
		LITERAL_TYPE(Mesi::KilogramsSq, _kg2)
		LITERAL_TYPE(Mesi::Newtons, _n)
		LITERAL_TYPE(Mesi::NewtonsSq, _n2)
		LITERAL_TYPE(Mesi::Hertz, _hz)
		LITERAL_TYPE(Mesi::Amperes, _a)
		LITERAL_TYPE(Mesi::Kelvin, _k)
		LITERAL_TYPE(Mesi::Moles, _mol)
		LITERAL_TYPE(Mesi::Candela, _cd)
		LITERAL_TYPE(Mesi::Pascals, _pa)
		LITERAL_TYPE(Mesi::Joules, _j)
		LITERAL_TYPE(Mesi::Watts, _w)
		LITERAL_TYPE(Mesi::Coulombs, _c)
		LITERAL_TYPE(Mesi::Volts, _v)
		LITERAL_TYPE(Mesi::Farads, _f)
		LITERAL_TYPE(Mesi::Ohms, _ohm)
		LITERAL_TYPE(Mesi::Siemens, _siemens)
		LITERAL_TYPE(Mesi::Webers, _wb)
		LITERAL_TYPE(Mesi::Tesla, _t)
		LITERAL_TYPE(Mesi::Henry, _h)
#undef LITERAL_TYPE
	}
}
