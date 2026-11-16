#define FUN sqrt

#include "func1.h"

int main (void)
{
    atest1 (L_nan);
    atest1 (L_pinf);
    atest1 (L_minf);
    atest1 (L_p0);
    atest1 (L_m0);
    atest1 (L_p1);
    atest1 (L_m1);
    atest1 (avrtest_ftod64 (0.25f));
    atest1 (avrtest_ftod64 (2.25f));
    atest1 (avrtest_ftod64 (65536.0f));
    atest1 (avrtest_ftod64 (255.125f * 255.125f));

    return 0;
}
