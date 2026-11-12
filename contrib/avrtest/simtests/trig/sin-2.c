// Bulk test against AVRtest's values for x uniformly distributed in [x0, x1].
// Fails if the relative error exceeds max_rel_err.

#define FUN sin

#include "func1.h"

int main (void)
{
    L x0 = avrtest_ftod64 (-3.0f);
    L x1 = avrtest_ftod64 (+3.0f);
    float max_rel_err = 4.0e-16;

    for (int i = 0; i < 3000; ++i)
    {
        L x = avrtest_prand_d64 (x0, x1);
        atest1_rel (x, max_rel_err);
    }

    return 0;
}
