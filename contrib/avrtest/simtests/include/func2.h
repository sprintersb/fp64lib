#include "base.h"

// Fail if fp64_FUN (x, y) != avrtest_FUN_d64 (x, y)
UU NI static void atest2 (L x, L y)
{
    L z1 = LFUN(FUN) (x, y);
    L z2 = AFUN(FUN) (x, y);
    if (! same (z1, z2))
    {
        printf ("\n%s (%+.16De, %+.16De)\n", S_FUN, x, y);
        printf ("result: %+.16De = %+.13Da\n", z1, z1);
        printf ("expect: %+.16De = %+.13Da\n", z2, z2);
        exit (1);
    }
}


// Fail if error in ULPs at X exceeds ULP_MAX.
UU NI static bool atest2_ulp (L x, L y, uint64_t ulp_max)
{
    L z1 = LFUN(FUN) (x, y);
    L z2 = AFUN(FUN) (x, y);
    L ulp = avrtest_ulp_d64 (z1, z2);
    ulp = avrtest_fabs_d64 (ulp);

    if (avrtest_cmp_d64 (ulp, avrtest_u64tod64 (ulp_max)) > 0)
    {
        printf ("\n%s (%+.16De, %+.16De) ULPs %.0Df exceeds %llu ULPs\n",
                S_FUN, x, y, ulp, ulp_max);
        printf ("result: %+.16De = %+.13Da\n", z1, z1);
        printf ("expect: %+.16De = %+.13Da\n", z2, z2);
        exit (2);
    }

    return ulp == 0;
}


// Fail if relative error at X exceeds REL_MAX.
UU NI static void atest2_rel (L x, L y, float rel_max)
{
    L z1 = LFUN(FUN) (x, y);
    L z2 = AFUN(FUN) (x, y);

    if (same (z1, z2))
        return;

    L delta = avrtest_sub_d64 (z1, z2);

    if (avrtest_cmp_d64 (delta, 0) == 0)
        return;

    L rel = avrtest_div_d64 (delta, z2);
    rel = avrtest_fabs_d64 (rel);

    if (avrtest_cmp_d64 (rel, avrtest_ftod64 (rel_max)) > 0)
    {
        printf ("\n%s (%+.16De, %+.16De) REL error %.3De exceeds %.3e\n",
                S_FUN, x, y, rel, rel_max);
        printf ("result: %+.16De = %+.13Da\n", z1, z1);
        printf ("expect: %+.16De = %+.13Da\n", z2, z2);
        exit (2);
    }
}


// Capture cycles when fp64_FUN is evaluated at X.
UU NI static uint32_t atest2_cycles (L x, L y)
{
    avrtest_cycles_call ();
    UU volatile L z = LFUN(FUN) (x, y);
    return avrtest_cycles ();
}


// Show average and max cycles for NN quidistant (x, y) in [X0, X1] × [Y0, Y1].
// Show (x, y) value where the max cycles occured.
UU NI static void atest2_show_cycles (L x0, L x1, L y0, L y1, uint32_t nn)
{
    const L dx = avrtest_sub_d64 (x1, x0);
    const L dy = avrtest_sub_d64 (y1, y0);
    const L stepx = avrtest_div_d64 (dx, avrtest_u32tod64 (nn));
    const L stepy = avrtest_div_d64 (dy, avrtest_u32tod64 (nn));
    L x_max = 0;
    L y_max = 0;
    uint64_t total = 0;
    uint32_t ma = 0;
    for (uint32_t n = 0; n <= nn; ++n)
    {
        L nstepsx = avrtest_mul_d64 (stepx, avrtest_u32tod64 (n));
        L x = n == nn ? x1 : avrtest_add_d64 (x0, nstepsx);

        for (uint32_t m = 0; m <= nn; ++m)
        {
            L mstepsy = avrtest_mul_d64 (stepy, avrtest_u32tod64 (m));
            L y = m == nn ? y1 : avrtest_add_d64 (y0, mstepsy);

            uint32_t cyc = atest2_cycles (x, y);
            if (cyc > ma)
                ma = cyc, x_max = x, y_max = y;
            total += cyc;
        }
    }

    float avr = avrtest_divf (avrtest_u64tof (total), avrtest_u32tof (1 + nn));

#ifdef S_MCU
    printf ("%s ", S_MCU);
#endif

    printf ("cycles %-8s avr:%6.0f  max:%6lu  "
            "xmax:0x%016llx=%+.16Df  ymax:0x%016llx=%+.16Df\n",
            s_fun ? s_fun : S_FUN, avr, ma, x_max,x_max, y_max,y_max);
}
