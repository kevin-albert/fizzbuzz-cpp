/**
 * Compile-time implementation of fizzbuzz
 * @author Kevin Albert <54.kevinalbert@gmail.com>
 *
 * Unlicensed. Feel free to use and take credit for this code.
 */

#include <iostream>

//
// Data types - ints and strings
//

// Ints are easy
template <int N> struct Int { enum { value = N }; };

// 
// Strings are a little trickier
// We define them as static character arrays that have max size of 8
// 
template <char ...C>
struct CharArray {
    static constexpr char const value[sizeof...(C)+1] = { C...,'\0' };
};

template <char ...C>
constexpr char const CharArray<C...>::value[sizeof...(C)+1];

template <int N, int M>
constexpr char CharAt(char const(&c)[M]) noexcept { return c[N < M ? N : M-1]; }

// String "constructor"
#define String(s) CharArray<\
    CharAt<0>(s), CharAt<1>(s), CharAt<2>(s), CharAt<3>(s), \
    CharAt<4>(s), CharAt<5>(s), CharAt<6>(s), CharAt<7>(s)>

//
// Arithmetic and logic
//
template <int N, int M>
struct Divisible { enum { value = (N % M) == 0 }; };

template <bool Condition, typename IfTrue, typename IfFalse>
class If;

template <typename IfTrue, typename IfFalse>
struct If<true, IfTrue, IfFalse> { using then = IfTrue; };

template <typename IfTrue, typename IfFalse>
struct If<false, IfTrue, IfFalse> { using then = IfFalse; };

//
// Evaluate Callback<i> for i in 1..N
// We use a weird right-fold to accumulate a parameter pack <N, N, N-1, N-2, ... 1>
// and then expand it 
template <template <int> class Callback, int N, int J, int ...I>
struct PrintLoopRec {
    static void go() { PrintLoopRec<Callback, N, J+1, J, I...>::go(); }
};
void expand(int...) {}
template <class C> int print(C c) { std::cout << c << '\n'; return 0; }
template <template <int> class Callback, int N, int ...I>
struct PrintLoopRec<Callback, N, N, I...> {
    static void go() { expand(print(Callback<N-I>::value)...); }
};

template <template <int> class Callback, int N>
using PrintLoop = PrintLoopRec<Callback, N+1, 2, 1>;

//
// The actual program
//

// Compute(i) - returns either i, "FizzBuzz", "Buzz", or "Fizz" depending on the
//              value of i
//
template <int I>
using Compute = typename
    If<Divisible<I,15>::value, String("FizzBuzz"), typename
      If<Divisible<I, 5>::value, String("Buzz"), typename
        If<Divisible<I, 3>::value, String("Fizz"), Int<I>>::then
      >::then
    >::then;

int main(void) {
    // Evaluate the loop
    PrintLoop<Compute, 100>::go();
}
