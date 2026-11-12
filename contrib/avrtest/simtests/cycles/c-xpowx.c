#ifndef NN
#define NN 6000
#endif

#define FUN xpowx

#include <stdio.h>
#include "avrtest.h"
#include "fp64lib.h"

float64_t avrtest_xpowx_d64 (float64_t x)
{
    return avrtest_pow_d64 (x, x);
}

float64_t fp64_xpowx (float64_t x)
{
    return fp64_pow (x, x);
}


#include "func1.h"

int main (void)
{
    L x0 = avrtest_ftod64 (0.0f);
    L x1 = avrtest_ftod64 (10.0f);

    s_fun = "x^x";
    atest1_show_cycles (x0, x1, NN);

    return 0;
}
