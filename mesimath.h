#pragma once
#include "mesitype.h"
namespace std {
#define MESI_TEMPLATE template<typename T, typename t_m, typename t_s, typename t_kg, typename t_A, typename t_K, typename t_mol, typename t_cd, typename t_scale>
#define MESI_TEMPLATE_2 template<typename T, typename t_m, typename t_m2, typename t_s, typename t_s2, typename t_kg, typename t_kg2, typename t_A, typename t_A2, typename t_K, typename t_K2, typename t_mol, typename t_mol2, typename t_cd, typename t_cd2, typename t_scale, typename t_scale2>
#define MESI_TYPE Mesi::RationalTypeReduced<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd, t_scale>
#define MESI_TYPE_2 Mesi::RationalTypeReduced<T, t_m2, t_s2, t_kg2, t_A2, t_K2, t_mol2, t_cd2, t_scale2>
#define MESI_SCALAR Mesi::Type<T, 0, 0, 0>
#define FORWARD_SIMPLE_UNARY(name) MESI_TEMPLATE auto name(MESI_TYPE const &x) { return MESI_TYPE(name(x.val)); }
#define FORWARD_SIMPLE_BINARY(name) MESI_TEMPLATE auto name(MESI_TYPE const &x, MESI_TYPE const &y) { return MESI_TYPE(name(x.val, y.val)); }
#define FORWARD_SCALAR(name) template<typename T> auto name(MESI_SCALAR const &x) { return MESI_SCALAR(name(x.val)); }

FORWARD_SIMPLE_UNARY(abs)
FORWARD_SIMPLE_BINARY(fmax)
FORWARD_SIMPLE_BINARY(fmin)
FORWARD_SIMPLE_BINARY(fdim)
FORWARD_SIMPLE_UNARY(ceil)
FORWARD_SIMPLE_UNARY(floor)
FORWARD_SIMPLE_UNARY(trunc)
FORWARD_SIMPLE_UNARY(round)
FORWARD_SIMPLE_UNARY(nearbyint)
FORWARD_SIMPLE_UNARY(rint)
FORWARD_SCALAR(exp)
FORWARD_SCALAR(exp2)
FORWARD_SCALAR(expm1)
FORWARD_SCALAR(log)
FORWARD_SCALAR(log10)
FORWARD_SCALAR(log1p)
FORWARD_SCALAR(log2)
FORWARD_SCALAR(sin)
FORWARD_SCALAR(cos)
FORWARD_SCALAR(tan)
FORWARD_SCALAR(asin)
FORWARD_SCALAR(acos)
FORWARD_SCALAR(atan)
FORWARD_SCALAR(sinh)
FORWARD_SCALAR(cosh)
FORWARD_SCALAR(tanh)
FORWARD_SCALAR(asinh)
FORWARD_SCALAR(acosh)
FORWARD_SCALAR(atanh)
FORWARD_SCALAR(erf)
FORWARD_SCALAR(erfc)
FORWARD_SCALAR(lgamma)
FORWARD_SCALAR(tgamma)

MESI_TEMPLATE_2
constexpr auto fma(MESI_TYPE const &x, MESI_TYPE_2 const &y, decltype(MESI_TYPE{}*MESI_TYPE_2{}) const &z) {
	return (x*y)+z;
}

template<typename T>
MESI_SCALAR atan2(MESI_SCALAR const &x, MESI_SCALAR const &y) {
	return MESI_SCALAR(atan2(x.val, y.val));
}

MESI_TEMPLATE
auto sqrt(MESI_TYPE const &x) {
	return Mesi::pow<std::ratio<1,2>>(x);
}

MESI_TEMPLATE
auto cbrt(MESI_TYPE const &x) {
	return Mesi::pow<std::ratio<1,3>>(x);
}

MESI_TEMPLATE
auto hypot(MESI_TYPE const &x, MESI_TYPE const &y) {
	return sqrt(x*x+y*y);
}

#undef MESI_TEMPLATE
#undef MESI_TEMPLATE_2
#undef MESI_TYPE
#undef MESI_TYPE_2
#undef MESI_SCALAR
#undef FORWARD_SIMPLE_UNARY
#undef FORWARD_SIMPLE_BINARY
#undef FORWARD_SCALAR
}

