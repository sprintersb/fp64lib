#define FUN atan2

#include "func2.h"

int main (void)
{
    L x0 = avrtest_ftod64 (-10.0f);
    L x1 = avrtest_ftod64 (+10.0f);
    L y0 = avrtest_ftod64 (-10.0f);
    L y1 = avrtest_ftod64 (+10.0f);
    float max_rel_err = 4.5e-16;

    for (uint16_t i = 0; i < 3000; ++i)
    {
        L x, y;
        do
            x = avrtest_prand_d64 (x0, x1);
        while (fp64_isnan (x));
        do
            y = avrtest_prand_d64 (y0, y1);
        while (fp64_isnan (y));

        atest2_rel (x, y, max_rel_err);
    }

    return 0;
}
