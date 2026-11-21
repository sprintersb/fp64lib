#define FUN atan

#include "func1.h"

int main (void)
{
    // atest1: compare fp64_FUN(x) against avrtest_FUN_d64(x)
    atest1 (L_nan);
    atest1 (L_pinf);
    atest1 (L_minf);
    atest1 (L_p0);
    atest1 (L_m0);

    atest1_ulp (L_m1, 1);
    atest1_ulp (L_p1, 1);

    return 0;
}
