#define FUN ceil

#include "func1.h"

void test_3x (L x)
{
    atest1 (x);
    atest1 (x + 1);
    if (x != 0)
        atest1 (x - 1);
    else
    {
        atest1 (L_m0);
        atest1 (L_m0 | 0x1);
    }
}

void test_9x (L x)
{
    test_3x (x);
    L half = avrtest_ftod64 (0.5f);
    test_3x (avrtest_add_d64 (x, half));
    test_3x (avrtest_sub_d64 (x, half));
}

int main (void)
{
    float f0 = -11.0f;
    float f1 = +11.0f;
    float df = 1.0f;

    for (float f = f0; avrtest_cmpf (f, f1) <= 0; f = avrtest_addf (f, df))
    {
        L x = avrtest_ftod64 (f);
        test_9x (x);
    }

    for (int ex = 40; ex <= 70; ++ex)
    {
        L x = avrtest_ldexp_d64 (L_p1, ex);
        test_9x (x);
        x = avrtest_ldexp_d64 (L_m1, ex);
        test_9x (x);
    }

    return 0;
}
