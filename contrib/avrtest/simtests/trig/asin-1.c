#define FUN asin

#include "func1.h"

int main (void)
{
    // atest1: compare fp64_FUN(x) against avrtest_FUN_d64(x)
    atest1 (L_nan);
    atest1 (L_pinf);
    atest1 (L_minf);
    atest1 (L_p0);
    atest1 (L_m0);
    // FIXME: We are still having rounding quirks in
    // rpretA and muldf3_pse, so don't scrutinize L_p0 | 0x1 for now.
    atest1 (L_p0 | 0x2);
    atest1 (L_m0 | 0x2);

    return 0;
}
