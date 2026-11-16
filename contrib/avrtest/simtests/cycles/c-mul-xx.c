#ifndef NN
#define NN 6000
#endif

#define FUN mulxx

#include <stdio.h>
#include "avrtest.h"
#include "fp64lib.h"

float64_t avrtest_mulxx_d64 (float64_t x)
{
    return avrtest_mul_d64 (x, x);
}

float64_t fp64_mulxx (float64_t x)
{
    return fp64_mul (x, x);
}


#include "func1.h"

int main (void)
{
    L x0 = avrtest_ftod64 (-1.0e10f);
    L x1 = avrtest_ftod64 (+1.0e10f);

    s_fun = "x*x";
    atest1_show_cycles (x0, x1, NN);

    return 0;
}
