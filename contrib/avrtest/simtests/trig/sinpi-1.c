// Test specific values against AVRtest's and abort on mismatch.

#define FUN sinpi

#include "func1.h"

int main (void)
{
    // sinpi etc require MPFR >= 4.2
    if (avrtest_can_mpfr () < 4020)
        return AVRTEST_EXIT_NO_MPFR;

    atest1 (L_nan);
    atest1 (L_pinf);
    atest1 (L_minf);
    atest1 (L_p0);
    atest1 (L_m0);

    for (int n = -20; n <= 20; ++n)
    {
        L x = avrtest_ldexp_d64 (avrtest_s32tod64 (n), -1);
        int iy = 0?0
            : (n & 3) == 1 ? +1
            : (n & 3) == 3 ? -1
            : 0;

        atest1xy (x, avrtest_s32tod64 (iy));
    }

    return 0;
}
