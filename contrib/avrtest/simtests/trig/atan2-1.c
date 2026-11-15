#define FUN atan2

#include "func2.h"

const __flash char* const __flash s_vals[] =
{
    _(nan),
    _(+inf), _(-inf),
    _(+0), _(-0),
    _(+1), _(-1),
    _(+2), _(-2),
};

void test_2 (L x, L y)
{
    atest2 (x, y);
    atest2 (y, x);
}

void test_8 (L x, L y)
{
    test_2 (x, y);
    test_2 (x, y ^ L_m0);
    test_2 (x ^ L_m0, y);
    test_2 (x ^ L_m0, y ^ L_m0);
}


int main (void)
{
    test_8 (L_pinf, L_p0);
    test_8 (L_pinf, L_p1);
    test_8 (L_p0, L_p0);
    test_8 (L_p0, L_p1);

    for (size_t ix = 0; ix < ARRAY_SIZE (s_vals); ++ix)
    {
        L x = avrtest_fstrtod64 (s_vals[ix], NULL);

        for (size_t iy = 0; iy < ARRAY_SIZE (s_vals); ++iy)
        {
            L y = avrtest_fstrtod64 (s_vals[iy], NULL);

            atest2_ulp (x, y, 1);
        }
    }

    return 0;
}
