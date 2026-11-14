#define FUN add

#include "func2.h"

int main (void)
{
    uint16_t n = 5000;
    uint16_t good = 0;

    for (uint16_t i = 0; i < n; ++i)
    {
        L x, y;
        do
            x = rand_u64 ();
        while (fp64_isnan (x));
        do
            y = rand_u64 ();
        while (fp64_isnan (y));

        good += atest2_ulp (x, y, 1);
    }

    float nf = avrtest_u32tof (n);
    float fails = avrtest_subf (nf, avrtest_u32tof (good));
    float pfails = avrtest_divf (fails, nf);
    float pp = avrtest_mulf (pfails, 100.0f);

    printf ("good: %u / %u -> %.2f%% fails\n", good, n, pp);

    return 0;
}
