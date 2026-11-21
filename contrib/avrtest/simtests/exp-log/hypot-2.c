#define FUN hypot

#include "func2.h"

void testing (L lo, L hi, uint16_t n, float max_rel_err)
{
    for (uint16_t i = 0; i < n; ++i)
    {
        L x = avrtest_prand_d64 (lo, hi);
        L y = avrtest_prand_d64 (lo, hi);

        atest2_rel (x, y, max_rel_err);
    }
}

int main (void)
{
    float max_rel_err = 2.4e-16;

    L lo = avrtest_ftod64 (-10e10f);
    L hi = avrtest_ftod64 (+10e10f);
    testing (lo, hi, 500, max_rel_err);

    lo = avrtest_ftod64 (-100.0f);
    hi = avrtest_ftod64 (+100.0f);
    testing (lo, hi, 1000, max_rel_err);

    return 0;
}
