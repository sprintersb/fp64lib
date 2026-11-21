#define FUN hypot

#include "func2.h"

void test_2 (L x, L y)
{
    atest2 (x, y);
    atest2 (y, x);
}

int main (void)
{
    test_2 (L_nan, L_p0);
    test_2 (L_pinf, L_p0);
    test_2 (L_pinf, L_minf);
    test_2 (L_pinf, L_pinf);
    test_2 (L_p0, L_p0);

    L x = avrtest_ftod64 (3.0f);
    L y = avrtest_ftod64 (4.0f);
    atest2 (x, y);

    return 0;
}
