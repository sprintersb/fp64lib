#define FUN sqrt

#include "func1.h"

NI L qrand (void)
{
    L mm = avrtest_u64tod64 (rand_u64 ());
    int expo = (int) ((2047 & avrtest_prand ()) - 1024);
    return avrtest_ldexp_d64 (mm, expo);
}

int main (void)
{
    for (uint16_t i = 0; i < 1000; ++i)
    {
        L x = qrand ();
        atest1_ulp (x, 1);
    }

    return 0;
}
