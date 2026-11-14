// Bulk test against AVRtest's values for x uniformly distributed in [x0, x1].
// Fails if the relative error exceeds max_rel_err.

#define FUN cospi

#include "func1.h"

int main (void)
{
    // sinpi etc require MPFR >= 4.2
    if (avrtest_can_mpfr () < 4020)
        return AVRTEST_EXIT_NO_MPFR;

    L x0 = avrtest_ftod64 (-10.0f);
    L x1 = avrtest_ftod64 (+10.0f);
    float max_rel_err = 2.2e-16;

    for (int i = 0; i < 1000; ++i)
    {
        L x = avrtest_prand_d64 (x0, x1);
        atest1_rel (x, max_rel_err);
    }

    return 0;
}
