#pragma once

#include <cmath>
#include <type_traits>
#include <cstdint>

// TODO:
// Exercise 2:
// Implement greated common divisor using constexpr, such that it can be evaluated at compile time.
// Your implementation should use variadic templates, to enable calling it
// with any number of arguments, ensure that at least two arguments are provided.

// // Base case for two arguments
// template <typename T>
// constexpr T gcd(T a, T b) {
//     return b == 0 ? a : gcd(b, a % b);
// }

// // Recursive case for more than two arguments
// template <typename T, typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 2)>>
// constexpr T gcd(T a, T b, Args... args) {
//     return gcd(gcd(a, b), args...);
// }


// Base case: GCD of two numbers
constexpr std::int64_t gcd(std::int64_t a, std::int64_t b) {
    return (b == 0) ? a : gcd(b, a % b);
}

// Base case: GCD of two numbers
constexpr int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

// Recursive case: GCD of multiple numbers using variadic templates
template <typename T, typename... Args>
constexpr T gcd(T a, T b, Args... args) {
    return gcd(a, gcd(b, args...));
}



// Exercise 3:
// Implement minimum common multiple using constexpr, such that it can be evaluated at compile time.
// As with GCD, your implementation should use variadic templates, to enable calling it
// with arbitrary number of arguments, but at least two

// Function to calculate minimum common multiple (mcm) using gcd
template <typename T>
constexpr T mcm(T a, T b) {
    return (a * b) / gcd(a, b);
}

// Recursive case for more than two arguments
template <typename T, typename... Args>
constexpr T mcm(T a, T b, Args... args) {
    return mcm(mcm(a, b), args...);
}


// Exercise 4:
// Implement power using template metaprogramming. The template struct must be
// called `Power`, and be callable with `Power<base, exponent, modulus>::value`.
// I.e. it needs a compile time evaluation `value`, which holds the result of
// the computation.
// Think about your base cases

consteval int my_pow(int base, int exponent) {
    int result{1};

    for (int i{0}; i < exponent; ++i)
        result *= base;

    return result;
}

// Recursive case for exponentiation (exponent > 0)
template <int base, int exponent, int modulus>
struct Power {
    static constexpr int value =
        (exponent % 2 == 0) ?
        Power<(base * base) % modulus, exponent / 2, modulus>::value :
        (base * Power<base, exponent - 1, modulus>::value) % modulus;
};

// Specialization for the case where exponent is 0
template <int base, int modulus>
struct Power<base, 0, modulus> {
    static constexpr int value = (modulus == 1) ? 0 : 1;
};



