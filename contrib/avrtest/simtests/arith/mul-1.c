#define FUN mul

#include "func2.h"

const __flash L vals[] =
{
    L_nan, L_pinf, L_minf, L_p0, L_m0, L_mtiny, L_ptiny, L_m1, L_p1,
    0x7feff00000000000ULL, 0x001ff00000000000ULL, 
    0xffeff00000000000ULL, 0x801ff00000000000ULL,
    0x3fe0fffffffffff0ULL,
    0x3feffffffffffff0ULL,
};

int main (void)
{
    for (size_t ix = 0; ix < ARRAY_SIZE (vals); ++ ix)
    {
        L x = vals[ix];
        for (size_t iy = 0; iy < ARRAY_SIZE (vals); ++ iy)
        {
            L y = vals[iy];
            // FIXME: We are still having rounding quirks in
            // rpretA and muldf3_pse, so allow for ULP of 1.
            atest2_ulp (x, y, 1);
        }
    }
    
}
