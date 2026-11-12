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

#define LFU_(fun) fp64_##fun
#define LFUN(fun) LFU_ (fun)

#define AFU_(fun) avrtest_##fun##_d64
#define AFUN(fun) AFU_ (fun)

#define STRING_(x) #x
#define STRINGY(x) STRING_(x)
#define S_FUN STRINGY(FUN)

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
NI static bool same (L x, L y)
{
    bool nan1 = fp64_isNaN (x);
    bool nan2 = fp64_isNaN (y);
    if (nan1 + nan2 != 0)
        return nan1 + nan2 == 2;
    return fp64_compare (x, y) == 0;
}


// Fail if fp64_FUN (x) != avrtest_FUN_d64 (x)
UU NI static void atest1 (L x)
{
    L z1 = LFUN(FUN) (x);
    L z2 = AFUN(FUN) (x);
    if (! same (z1, z2))
    {
        printf ("\n%s (%+.16De)\n", S_FUN, x);
        printf ("result: %+.16De = %+.14Da\n", z1, z1);
        printf ("expect: %+.16De = %+.14Da\n", z2, z2);
        exit (1);
    }
}


// Fail if error in ULPs at X exceeds ULP_MAX.
UU NI static void atest1_ulp (L x, uint64_t ulp_max)
{
    L z1 = LFUN(FUN) (x);
    L z2 = AFUN(FUN) (x);
    L ulp = avrtest_ulp_d64 (z1, z2);
    ulp = avrtest_fabs_d64 (ulp);

    if (avrtest_cmp_d64 (ulp, avrtest_u64tod64 (ulp_max)) > 0)
    {
        printf ("\n%s (%+.16De) ULPs %.0Df exceeds %llu ULPs\n",
                S_FUN, x, ulp, ulp_max);
        printf ("result: %+.16De = %+.14Da\n", z1, z1);
        printf ("expect: %+.16De = %+.14Da\n", z2, z2);
        exit (2);
    }
}


// Fail if relative error at X exceeds REL_MAX.
UU NI static void atest1_rel (L x, float rel_max)
{
    L z1 = LFUN(FUN) (x);
    L z2 = AFUN(FUN) (x);

    if (same (z1, z2))
        return;

    L delta = avrtest_sub_d64 (z1, z2);

    if (avrtest_cmp_d64 (delta, 0) == 0)
        return;

    L rel = avrtest_div_d64 (delta, z2);
    rel = avrtest_fabs_d64 (rel);

    if (avrtest_cmp_d64 (rel, avrtest_ftod64 (rel_max)) > 0)
    {
        printf ("\n%s (%+.16De) REL error %.3De exceeds %.3e\n",
                S_FUN, x, rel, rel_max);
        printf ("result: %+.16De = %+.14Da\n", z1, z1);
        printf ("expect: %+.16De = %+.14Da\n", z2, z2);
        exit (2);
    }
}


// Capture cycles when fp64_FUN is evaluated at X.
UU NI static uint32_t atest1_cycles (L x)
{
    avrtest_cycles_call ();
    UU volatile L y = LFUN(FUN) (x);
    return avrtest_cycles ();
}


// Show average and max cycles for NN quidistant x in [X0, X1].
// Show x value where the max cycles occured.
UU NI static void atest1_show_cycles (L x0, L x1, uint32_t nn)
{
    const L dx = avrtest_sub_d64 (x1, x0);
    const L step = avrtest_div_d64 (dx, avrtest_u32tod64 (nn));
    L x_max = 0;
    uint64_t total = 0;
    uint32_t ma = 0;
    for (uint32_t n = 0; n <= nn; ++n)
    {
        L nsteps = avrtest_mul_d64 (step, avrtest_u32tod64 (n));
        L x = n == nn ? x1 : avrtest_add_d64 (x0, nsteps);

        uint32_t cyc = atest1_cycles (x);
        if (cyc > ma)
            ma = cyc, x_max = x;
        total += cyc;
    }

    float avr = avrtest_divf (avrtest_u64tof (total), avrtest_u32tof (1 + nn));

#ifdef S_MCU
    printf ("%s ", S_MCU);
#endif

    printf ("cycles %-8s avr:%6.0f  max:%6lu  xmax:0x%016llx=%+.16Df\n",
            s_fun ? s_fun : S_FUN, avr, ma, x_max,x_max);
}
