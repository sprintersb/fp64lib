#define FUN mul

#include "func2.h"

const __flash L vals[] =
{
    L_nan,
    L_pinf,  L_minf,
    L_ptiny, L_mtiny,
    L_p0, L_m0,
    L_m1, L_p1,
    0x7feff00000000000ULL, 0x001ff00000000000ULL, 
    0xffeff00000000000ULL, 0x801ff00000000000ULL,
    0x3fe0fffffffffff0ULL,
    0x3fefffffffffffffULL,
    0x3feffffffffaaaaaULL,
    0x3feffffffff55555ULL,
    0x3feffffffffcccccULL,
    0x3feffffffff33333ULL,
    0x3feffffffffffff1ULL,
    0x3feffffffff00001ULL,
    0x3feffffff0000000ULL,
    0x3feffffff8000000ULL,
    0x3feffffffc000000ULL,
    0x3feffffff7000000ULL,
    0x3feffffffc000000ULL,
    0x0000001000000000ULL,
    0x0010000000000000ULL,
    0x0010000000000000ULL - 1,
    0x0010000000000000ULL + 1,
};

int main (void)
{
    uint32_t nn = 0;
    uint32_t good = 0;

    for (size_t ix = 0; ix < ARRAY_SIZE (vals); ++ ix)
    {
        L x = vals[ix];
        for (size_t iy = 0; iy < ARRAY_SIZE (vals); ++ iy)
        {
            L y = vals[iy];

            // FIXME: We are still having 1.25% of rounding quirks in
            // rpretA and/or muldf3_pse, so allow for ULP of 1.
            good += atest2_ulp (x, y, 1);
            nn += 1;
        }
    }

    float nf = avrtest_u32tof (nn);
    float fails = avrtest_subf (nf, avrtest_u32tof (good));
    float pfails = avrtest_divf (fails, nf);
    float pp = avrtest_mulf (pfails, 100.0f);

    printf ("good: %lu / %lu -> %.2f%% fails\n", good, nn, pp);

    return 0;
}
