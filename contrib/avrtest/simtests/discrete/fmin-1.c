#define FUN fmin

#include "func2.h"

const __flash char* const __flash s_vals[] =
{
    _(inf),
    _(0), _(1), _(2), _(0x1.0p-1022), _(0x1.0p-1023), _(0x1.0p-1074)
};

#define QUIET_NAN (1ull << 52)

void test_2 (L x, L y)
{
    // Don't scrutinize NaNs.  Behavior depends on whether the
    // NaN is quiet or signaling.
    if (fp64_isnan (x))
        return;
    if (fp64_isnan (y))
        return;

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

void test_101 (L x, L y)
{
    test_8 (x, y);

    test_8 (x + 1, y + 0);
    test_8 (x + 0, y + 1);
    test_8 (x + 1, y + 1);

    test_8 (x - 1, y - 0);
    test_8 (x - 0, y - 1);
    test_8 (x - 1, y - 1);

    test_8 (x + 1, y - 1);
    test_8 (x - 1, y + 1);
}


int main (void)
{
    for (size_t ix = 0; ix < ARRAY_SIZE (s_vals); ++ix)
    {
        L x = avrtest_fstrtod64 (s_vals[ix], NULL);

        for (size_t iy = 0; iy < ARRAY_SIZE (s_vals); ++iy)
        {
            L y = avrtest_fstrtod64 (s_vals[iy], NULL);

            test_101 (x, y);
        }
    }

    return 0;
}
