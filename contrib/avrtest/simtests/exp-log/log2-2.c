// Bulk test against AVRtest's values for x uniformly distributed in [x0, x1].
// Fails if the relative error exceeds max_rel_err.

#define FUN log2

#include "func1.h"

int main (void)
{
    L x0 = L_ptiny;
    L x1 = avrtest_ftod64 (+10.0f);
    float max_rel_err = 2.5e-16;

    for (int i = 0; i < 1000; ++i)
    {
        L x = avrtest_prand_d64 (x0, x1);
        atest1_rel (x, max_rel_err);
    }

    return 0;
}
