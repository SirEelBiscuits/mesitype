MesiType
========

[![Build Status](https://api.travis-ci.org/SirEelBiscuits/mesitype.svg?branch=master)](https://travis-ci.org/SirEelBiscuits/mesitype)

MEticulous Systeme International TYPEs. For automatic, compile-time checked handling of units with orthogonal meanings. Ideally, the system will add zero overhead given a sufficiently smart compiler, but this has not been checked yet.

Features
--------
MesiType allows for the combination of any of [metres](http://en.wikipedia.org/wiki/Metre),  [seconds](http://en.wikipedia.org/wiki/Second), and [kilograms](http://en.wikipedia.org/wiki/Kilogram), and any integral exponent thereof. Operators return appropriate types for the operands, allowing you to, for example, divide a Meters type, by a Seconds type, and end up with Meters per Second.

The types do not support casting to or from any type. This is by design. The goal of the system is to allow the compiler to detect and prevent type clashes. Implicit casts into or out of this type would allow this design goal to be circumvented, and so all casts are explicit.

Usage
-----
To create a custom type, the syntax is:

    Mesi::Type<T, t_m, t_s, t_kg>
    
where T is the 'storage type', and t_m, t_s, and t_kg are the (integral) exponents of metres, seconds, and kilos respectively. E.g., to create a meters type you might use:

    Mesi::Type<float, 1, 0, 0>
    
and to create a meters squared per kilo type, you might use:

    Mesi::Type<float, 2, 0, -1>

For simple units, where supported, literal operator overloads exist. These use float for the T argument by default, but this is configurable - see the source for details. These can be invoked as any of:

* _m
* _s
* _kg
* _N

or any of the above with a 2 suffix for that unit squared. These overloads are constexpr, and so should incur no runtime cost. Additionally to create hybrid units, using the above with * and / should also have no cost as these are constexpr functions as well. Example:

    auto speed = 4_m / 1_s;

As some compilers don't support literal overloads, there are also macros that produce the same, these are:

* METERS(x)
* SECONDS(x)
* KILOS(x)
* NEWTONS(x)

all of which can optionally take a 2 suffix. Additionally there is a macro to produce a unit with no metre, seconds, or kilos exponent, which is

* SCALAR(x)

if you have a suggestion for a literal overload for this, it'd be welcome!

These types also have shorthand names for ease of variable creation, in the Mesi namespace as:

* Meters
* Seconds
* Kilos
* Newtons
* Scalar

except for Scalar, these may also take a Sq suffix for the squared variant. Where possible, I strongly recommend using the precise unit names only when they need to be enforced. For intermediate storage, auto is shorter, safer, and far more sensible. That said, how you use it is your business.

Limitations
-----------
Currently only accepts relatively standard types for the T argument (float, double, int), but it should work with anything comparable so long as it has all the required operator overloads. There are improvements that could be made for this thanks to the addition of decltype to the spec, but it increases code complexity massively, and so far I've no indication that it's needed.

SI prefixes are not supported - you cannot mark a type as kiloseconds or picograms. This would require a system like std::chrono's ratios, which would be both complex, and remove some of the optimisation oportunities for the compiler (potentially). I am not planning to change this.

Planned Features
----------------

* Conversion between Mesi::Seconds, and std::chrono.
* Adding other common and useful units to the suffixes and shorthand variable names.
* Confirmation of the efficiency of the dissassembly.
