#ifndef NN
#define NN 6000
#endif

#define FUN 1divx

#include <stdio.h>
#include "avrtest.h"
#include "fp64lib.h"

float64_t avrtest_1divx_d64 (float64_t x)
{
    return avrtest_div_d64 (avrtest_s32tod64 (1), x);
}

float64_t fp64_1divx (float64_t x)
{
    return fp64_div (avrtest_s32tod64 (1), x);
}


#include "func1.h"

int main (void)
{
    L x0 = avrtest_ftod64 (-1.0e10f);
    L x1 = avrtest_ftod64 (+1.0e10f);

    s_fun = "1/x";
    atest1_show_cycles (x0, x1, NN);

    return 0;
}
