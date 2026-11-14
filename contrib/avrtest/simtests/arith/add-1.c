#define FUN add

#include "func2.h"

const __flash L vals[] =
{
    L_nan,
    L_pinf,  L_minf,
    L_pbig,  L_mbig,
    L_ptiny, L_mtiny,
    L_p0, L_m0,
    L_m1, L_p1,
    0x7feff00000000000ULL, 0x001ff00000000000ULL,
};

int main (void)
{
    for (size_t ix = 0; ix < ARRAY_SIZE (vals); ++ix)
    {
        L x = vals[ix];
        for (size_t iy = 0; iy < ARRAY_SIZE (vals); ++iy)
        {
            L y = vals[iy];
            atest2 (x, y);
        }
    }

    return 0;
}
