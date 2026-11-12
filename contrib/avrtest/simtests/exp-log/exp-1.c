// Test specific values against AVRtest's and abort on mismatch.

#define FUN exp

#include "func1.h"

int main (void)
{
    // atest1: compare fp64_FUN(x) against avrtest_FUN_d64(x)
    atest1 (L_nan);
    atest1 (L_pinf);
    atest1 (L_minf);
    atest1 (L_p0);
    atest1 (L_m0);
    atest1 (L_ptiny);
    atest1 (L_mtiny);

    return 0;
}
