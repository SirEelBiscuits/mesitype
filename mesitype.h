#pragma once

#include <string>
#include <ratio>
#include <limits>

namespace Mesi {
	namespace _internal {
		template<typename t_ratio, intmax_t t_exponent_denominator, typename t_power_of_ten>
		struct Scale
		{
		private:
			template<typename T, typename p>
			struct PowerOfTenValue
			{
				static T value()
				{
					static T v = pow(T(10), T(p::num)/T(p::den));
					return v;
				}
			};

			template<typename T, intmax_t num>
			struct PowerOfTenValue<T, std::ratio<num,1>>
			{
			private:
				static constexpr T calculate_value() {
					T ret = 1;
					for(intmax_t i = 0; i < num; i++)
					{
						ret *= T(10);
					}
					for(intmax_t i = 0; i > num; i--)
					{
						ret /= T(10);
					}
					return ret;
				}
			public:
				static constexpr T value()
				{
					constexpr T v = calculate_value();
					return v;
				}
			};
			template<typename T, typename r, intmax_t e>
			struct RatioValue
			{
				static T value()
				{
					static T v = pow(T(r::num)/T(r::den), T(1)/T(e));
					return v;
				}
			};
			template<typename T, typename r>
			struct RatioValue<T, r, 1>
			{
				static constexpr T value()
				{
					constexpr T v = T(r::num)/T(r::den);
					return v;
				}
			};
			template<typename T, typename ratio, intmax_t exponent_denominator, typename power_of_ten>
			static T calculate_value()
			{
				return RatioValue<T, ratio, exponent_denominator>::value() * PowerOfTenValue<T, power_of_ten>::value();
			}
		public:
			using ratio = t_ratio;
			using power_of_ten = t_power_of_ten;
			static constexpr intmax_t exponent_denominator = t_exponent_denominator;
			
			template<typename T>
			static T value()
			{
				static T v = calculate_value<T, ratio, exponent_denominator, power_of_ten>();
				return v;
			}

			using Inverse = Scale<std::ratio<ratio::den, ratio::num>, exponent_denominator, std::ratio<-power_of_ten::num, power_of_ten::den>>;
			
		};
		using ScaleOne = Scale<std::ratio<1,1>, 1, std::ratio<0,1>>;
		template<typename t_s1, typename t_s2>
		struct ScaleMultiply
		{
		private:
			struct Helper
			{
				template<intmax_t base, intmax_t pow>
				struct Exp
				{
					static_assert((pow == 0 || (std::numeric_limits<intmax_t>::max() / base / Exp<base, pow-1>::value) != 0), "pow must not overflow");
					static constexpr intmax_t value = base * Exp<base, pow-1>::value;
				};

				template<intmax_t base>
				struct Exp<base, 0>
				{
					static constexpr intmax_t value = 1;
				};

				template<intmax_t a, intmax_t b>
				struct Mul
				{
					static_assert(std::numeric_limits<intmax_t>::max() / a / b, "a*b must not overflow");
					static constexpr intmax_t value = a*b;
				};

				using p_ratio = std::ratio<
					Mul<Exp<t_s1::ratio::num, t_s2::exponent_denominator>::value, Exp<t_s2::ratio::num, t_s1::exponent_denominator>::value>::value,
					Mul<Exp<t_s1::ratio::den, t_s2::exponent_denominator>::value, Exp<t_s2::ratio::den, t_s1::exponent_denominator>::value>::value>;

				constexpr Helper()
				: p_num(p_ratio::num), p_den(p_ratio::den), p_power_of_ten(0), p_exp_den(t_s1::exponent_denominator * t_s2::exponent_denominator)
				{
					for(intmax_t d = 2; d < p_exp_den; d++)
					{
						while(p_exp_den % d == 0)
						{
							// This is a factor of the exponent's denominator, try to take the d-th root of numerator and denominator
							intmax_t rn = root(p_num, d);
							intmax_t rd = root(p_den, d);
							if(rn && rd)
							{
								// The d-th root of both is integer, so use these values going forward
								p_exp_den /= d;
								p_num = rn;
								p_den = rd;
							}
							else
							{
								// Can't take the d-th root, continue looking for other factors
								break;
							}
						}
					}

					while((p_num % 10) == 0)
					{
						p_num /= 10;
						p_power_of_ten++;
					}
					while((p_den % 10) == 0)
					{
						p_den /= 10;
						p_power_of_ten--;
					}
				}

				intmax_t p_num;
				intmax_t p_den;
				intmax_t p_power_of_ten;
				intmax_t p_exp_den;

				constexpr intmax_t root(intmax_t base, intmax_t r)
				{
					intmax_t ret = 1;
					while(pow(ret, r) < base)
					{
						ret++;
					}
					if(pow(ret, r) == base)
					{
						return ret;
					}
					else
					{
						return 0;
					}
				}

				constexpr intmax_t pow(intmax_t base, intmax_t exp)
				{
					intmax_t ret = 1;
					while(exp > 0)
					{
						ret *= base;
						exp--;
					}
					return ret;
				}
			};

		public:
			using Scale = _internal::Scale<std::ratio<Helper().p_num, Helper().p_den>, Helper().p_exp_den, std::ratio_add<std::ratio_add<typename t_s1::power_of_ten, typename t_s2::power_of_ten>, std::ratio<Helper().p_power_of_ten, Helper().p_exp_den>>>;
		};

		template<typename t_scale, typename t_power>
		using ScalingPower = ScaleMultiply<
			Scale<typename t_scale::ratio, t_scale::exponent_denominator * t_power::den, std::ratio_multiply<typename t_scale::power_of_ten, t_power>>,
			Scale<std::ratio<1,1>, t_power::num, std::ratio<0,1>>>;
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
		typename t_scale>
	struct RationalTypeReduced
	{
		using BaseType = T;
	private:
		using Zero = std::ratio<0,1>;
		using One = std::ratio<1,1>;
	public:
		using ScalarType = RationalTypeReduced<T, Zero, Zero, Zero, Zero, Zero, Zero, Zero, _internal::ScaleOne>;

		template<typename t_scale_ratio, intmax_t t_scale_exponent_denominator, typename t_scale_10_to_the>
		using Scale = RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, typename _internal::ScaleMultiply<t_scale, _internal::Scale<t_scale_ratio, t_scale_exponent_denominator, t_scale_10_to_the>>::Scale>;

		template<intmax_t t_scale_by>
		using Multiply = Scale<std::ratio<t_scale_by, 1>, 1, std::ratio<0,1>>;

		template<intmax_t t_scale_by>
		using Divide = Scale<std::ratio<1, t_scale_by>, 1, std::ratio<0,1>>;

		template<intmax_t t_pow>
		using ScaleByTenToThe = Scale<std::ratio<1,1>, 1, std::ratio<t_pow,1>>;

		template<typename t_pow>
		using Pow = RationalTypeReduced<T,
			  std::ratio_multiply<t_m, t_pow>,
			  std::ratio_multiply<t_s, t_pow>,
			  std::ratio_multiply<t_kg, t_pow>,
			  std::ratio_multiply<t_A, t_pow>,
			  std::ratio_multiply<t_K, t_pow>,
			  std::ratio_multiply<t_mol, t_pow>,
			  std::ratio_multiply<t_cd, t_pow>,
			  typename _internal::ScalingPower<t_scale, t_pow>::Scale>;

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

		template<typename t_scale2>
		explicit constexpr operator RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, t_scale2>() const {
			using Scale = typename _internal::ScaleMultiply<t_scale, typename t_scale2::Inverse>::Scale;
			T nv = val * Scale::template value<T>();

			return RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, t_scale2>(nv);
		}

		/**
		 * getUnit will get a SI-style unit string for this class
		 */
		static std::string getUnit() {
			static std::string s_unitString("");
			if( s_unitString.size() > 0 )
				return s_unitString;

			if( t_scale::ratio::num != 1 )
			{
				s_unitString += " * ";
				if( t_scale::exponent_denominator != 1 && t_scale::ratio::den != 1 )
				{
					s_unitString += "(";
				}
				s_unitString += std::to_string(t_scale::ratio::num);
				if( t_scale::ratio::den != 1 )
				{
					s_unitString += "/" + std::to_string(t_scale::ratio::den);
				}
				if( t_scale::exponent_denominator != 1 )
				{
					if( t_scale::ratio::den != 1 )
					{
						s_unitString += ")";
					}
					s_unitString += "^(1/";
					s_unitString += std::to_string(t_scale::exponent_denominator);
					s_unitString += ")";
				}
				s_unitString += " ";
			}

			if( t_scale::power_of_ten::num != 0 )
			{
				s_unitString += "* 10^";
				if(t_scale::power_of_ten::den != 1)
				{
					s_unitString += "(";
				}
				s_unitString += std::to_string(t_scale::power_of_ten::num);
				if(t_scale::power_of_ten::den != 1)
				{
					s_unitString += "/" + std::to_string(t_scale::power_of_ten::den) + ")";
				}
				s_unitString += " ";
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

	template<typename T, typename t_m, typename t_s, typename t_kg, typename t_A, typename t_K, typename t_mol, typename t_cd, typename t_ratio, intmax_t t_exponent_denominator, typename t_power_of_ten>
	using RationalType = RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, typename _internal::Scale<t_ratio, t_exponent_denominator, t_power_of_ten>>;

#define TYPE_A_FULL_PARAMS typename t_m, typename t_s, typename t_kg, typename t_A, typename t_K, typename t_mol, typename t_cd, typename t_scale
#define TYPE_A_PARAMS t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, t_scale
#define TYPE_B_FULL_PARAMS typename t_m2, typename t_s2, typename t_kg2, typename t_A2, typename t_K2, typename t_mol2, typename t_cd2, typename t_scale2
#define TYPE_B_PARAMS t_m2, t_s2, t_kg2, t_A2, t_K2, t_mol2, t_cd2, t_scale2
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
		using Scale = typename _internal::ScaleMultiply<t_scale, t_scale2>::Scale;
#define ADD_FRAC(TP) using TP = std::ratio_add<t_##TP, t_##TP##2>;
		ALL_UNITS(ADD_FRAC)
#undef ADD_FRAC
		return RationalTypeReduced<T, m, s, kg, A, K, mol, cd, Scale>(left.val * right.val);
	}

	template<typename T, TYPE_A_FULL_PARAMS, TYPE_B_FULL_PARAMS>
	constexpr auto operator/(
		RationalTypeReduced<T, TYPE_A_PARAMS> const& left,
		RationalTypeReduced<T, TYPE_B_PARAMS> const& right
	) {
		using Scale = typename _internal::ScaleMultiply<t_scale, typename t_scale2::Inverse>::Scale;
#define SUB_FRAC(TP) using TP = std::ratio_subtract<t_##TP, t_##TP##2>;
		ALL_UNITS(SUB_FRAC)
#undef SUB_FRAC
		return RationalTypeReduced<T, m, s, kg, A, K, mol, cd, Scale>(left.val / right.val);
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

	template<typename T, intmax_t t_m, intmax_t t_s, intmax_t t_kg, intmax_t t_A=0, intmax_t t_K=0, intmax_t t_mol=0, intmax_t t_cd=0, typename t_ratio = std::ratio<1,1>, typename t_pref = std::ratio<0,1>, intmax_t t_exponent_denominator=1>
	using Type = RationalType<T, std::ratio<t_m, 1>, std::ratio<t_s, 1>, std::ratio<t_kg, 1>, std::ratio<t_A, 1>, std::ratio<t_K, 1>, std::ratio<t_mol, 1>, std::ratio<t_cd, 1>, t_ratio, t_exponent_denominator, t_pref>;

#ifndef MESI_LITERAL_TYPE
#	define MESI_LITERAL_TYPE float
#endif

	template<intmax_t t_power, typename T>
	using Prefix = typename T::template ScaleByTenToThe<t_power>;

	template<typename T> using Deca  = Prefix<1, T>;
	template<typename T> using Hecto = Prefix<2, T>;
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
	using Grams     = Milli<Kilograms>;
	using Tonnes    = Kilo<Kilograms>;
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
