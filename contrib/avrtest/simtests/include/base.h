#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fp64lib.h"
#include "avrtest.h"
#include <avr/pgmspace.h>

#define L float64_t

#define NI __attribute((noipa))
#define UU __attribute((unused))

#define AVRTEST_EXIT_NO_MPFR 97

#define LFU_(fun) fp64_##fun
#define LFUN(fun) LFU_ (fun)

#define AFU_(fun) avrtest_##fun##_d64
#define AFUN(fun) AFU_ (fun)

#define STRING_(x) #x
#define STRINGY(x) STRING_(x)
#define S_FUN STRINGY(FUN)

#define ARRAY_SIZE(X) (sizeof (X) / sizeof (*X))

#ifdef __AVR_HAVE_ELPM__
// On devices with ELPM / RAMPZ, push code beyond 0x10000 for testing.
__asm (".pushsection .lowtext\n"
       ".skip 0x10000\n"
       ".popsection");
#endif

#define printf(msg, ...)  avrtest_printf_P (PSTR (msg), ##__VA_ARGS__)

#define L_p0 		0x0000000000000000LLU	// +0.0
#define L_m0		0x8000000000000000LLU	// -0.0
#define L_nan		0x7fffffffffffffffLLU	// NaN
#define L_pinf		0x7ff0000000000000LLU	// +INF
#define L_minf      0xfff0000000000000LLU	// -INF
#define L_p1		0x3ff0000000000000LLU	// +1.0
#define L_m1        0xdff0000000000000LLU	// -1.0
#define L_pbig		(L_pinf - 1)
#define L_mbig		(L_minf - 1)
#define L_ptiny		(0x0000000000000001LLU)
#define L_mtiny		(0x8000000000000001LLU)


const char *s_fun = NULL;

// Return true iff X == Y on binary level, taking
// any NaN as equal, but +0.0 != -0.0.
UU NI static bool same (L x, L y)
{
    bool nan1 = fp64_isNaN (x);
    bool nan2 = fp64_isNaN (y);
    if (nan1 + nan2 != 0)
        return nan1 + nan2 == 2;
    return fp64_compare (x, y) == 0;
}

UU NI static uint64_t rand_u64 (void)
{
    uint32_t lo = avrtest_prand ();
    uint32_t hi = avrtest_prand ();
    return lo | ((uint64_t) hi << 32);
}

#endif // BASE_H
