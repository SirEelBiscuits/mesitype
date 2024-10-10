MesiType
========

[![Build Status](https://api.travis-ci.org/SirEelBiscuits/mesitype.svg?branch=master)](https://travis-ci.org/SirEelBiscuits/mesitype)

MEticulous Systeme International TYPEs. For automatic, compile-time checked
handling of units with orthogonal meanings.
Ideally, the system will add zero overhead given a sufficiently smart compiler
compared to using floats (or whatever else), but this has not been checked yet.

Authors
-------

- Jameson Thatcher
- Lynn Ochs

Features
--------
MesiType allows for the combination of any of
[metres](http://en.wikipedia.org/wiki/Metre),
[seconds](http://en.wikipedia.org/wiki/Second), 
[kilograms](http://en.wikipedia.org/wiki/Kilogram),
[Amperes](http://en.wikipedia.org/wiki/Ampere),
[Kelvin](http://en.wikipedia.org/wiki/Kelvin),
[Moles](http://en.wikipedia.org/wiki/Mole_(Unit)), and
[Candela](http://en.wikipedia.org/wiki/Candela),
and any rational exponent thereof.
Operators return appropriate types for the operands, allowing you to, for
example, divide a Meters type by a Seconds type and end up with Meters per
Second.

The types do not support implicit casting to or from any type.
This is by design: the goal of the system is to allow the compiler to detect
and prevent type clashes.
Implicit casts into or out of this type would allow this design goal to be
circumvented, and so all casts are explicit.

SI Prefixes are also supported, along side arbitrary multipliers, to allow you
to create units for non-SI types, in terms of SI types.
Some examples are pre-defined, like Minutes (defined as 60 seconds).

Usage
-----
To create a simple custom type, the syntax is:

```cpp
Mesi::Type<T, t_m, t_s, t_kg, t_A, t_K, t_mol, t_cd>
```
    
where T is the 'storage type', and `t_m`, `t_s`, and `t_kg` are the
(integral) exponents of metres, seconds, and kilos respectively.
The arguments from `t_A` onward are optional, as Amperes, Kelvin, Moles, and
Candela are in less frequent use.

E.g., to create a meters type you might use:

```cpp
Mesi::Type<float, 1, 0, 0>
```
    
and to create a meters squared per kilo type, you might use:

```cpp
Mesi::Type<float, 2, 0, -1>
```

This syntax is quite verbose, so names for common units have been provided.
These include:

* `Meters`
* `Seconds`
* `Kilos`
* `Newtons`
* `Scalar`

except for Scalar, these may also take a Sq suffix for the squared variant.
There is also a `MetersCu` version, as this is often useful.
Many other types have such names, see the source code for details.

Where possible, I strongly recommend using the precise unit names only when
they need to be enforced.
For intermediate storage, auto is shorter, safer, and far more sensible.
That said, how you use it is your business.

Operator overloads are also provided for the named types.
These use float for the T argument by default, but this is configurable -
see the source for details.
These can be invoked as any of:

* `_m`
* `_s`
* `_kg`
* `_n`

etc. Some of the suffixes should contain uppercase letters, but all names
starting with `_` followed by a capital letter are reserved at all scopes.
As a result we've opted to use lower-case letters for all units.

To make these available, do:

```cpp
using namespace Mesi::Literals;
```

at your preferred scope.
These overloads are `constexpr`, and so should incur no runtime cost.
Additionally to create hybrid units, using the above with * and / should
also have no cost as these are `constexpr` functions as well.

Example:

```cpp
auto speed = 4_m / 1_s;
```

will create a speed of 4 m/s.

### Prefixes

SI prefixes like kilo-, mega-, milli- and so forth are available in the form:

```cpp
Mesi::Kilo<Mesi::Meters>
```

Additionally you can use the `Multiply<N>`, `Divide<N>` and `ScaleByTenToThe<N>`
types inside `Mesi::Type` to change the multiplier or SI prefix.

Example:

```cpp
Mesi::Kilo<Mesi::Meters>
Mesi::Meters::ScaleByTenToThe<3>
```

The above two types are equivalent.
Most common cases for using this functionality have been provided for you (e.g.,
the SI prefixes, the Minutes and Hours types).
If you need to make your own types, refer to the definitions of these types in
the source code.

Advanced Usage
--------------

It is sometimes convenient to define non-integral exponents of units directly.
This is available too, via `Mesi::RationalType`.
`RationalType` takes `std::ratio` arguments for its powers rather than integers.
`RationalType` has all the features of the integral version (the integral
version is in fact a simplified interface to the rational version).

You can also use `Type::Pow` to raise a type to any rational power. This
template expects a `std::ratio` type. `Mesi::Meters::Pow<std::ratio<2,1>>` are
square meters, and `Mesi::Seconds::Pow<std::ratio<-1,2>>` are `s^(-1/2) =
sqrt(Hz)`.

`Type` and `RationalType` have three extra template arguments, `t_ratio`,
`t_exponent_denominator`, and `t_power_of_ten`.
`t_ratio` expects a `std::ratio` type, and is a multiplier to the
value. It can be used to make non-decimal scaled units, like minutes
(`std::ratio<60,1>`) or fortnights, or imperial units.  `t_power_of_ten` also
expects a `std::ratio` and is used to create SI prefixes, but will also take
any other rational value.  `t_exponent_denominator` accepts any strictly
positive integer value and can in special cases be used to create roots of
scaling factors. Combining `t_ratio=std::ratio<2,1>`,
`t_power_of_ten=std::ratio<3,1>`, `t_exponent_denominator=4` will result in an
overall scaling factor of `2^(1/4) * 10^3`.
You do not need to worry about simplifying the multiplier between `t_ratio` and
`t_power_of_10` or calculating roots of `t_ratio`, as this is done for you, so
long as you don't use `RationalTypeReduced` directly (which there is good no
reason to do).

Limitations
-----------
Currently only accepts relatively standard types for the T argument (float,
double, int), but it should work with anything comparable so long as it has
all the required operator overloads.

Planned Features
----------------

* Conversion between Mesi::Seconds, and std::chrono.
* Confirmation of the efficiency of the dissassembly.
* Maths functions
