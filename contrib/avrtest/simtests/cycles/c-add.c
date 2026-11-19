#ifndef NN
#define NN 42
#endif

#define FUN add

#include "func2.h"

int main (void)
{
    L x0 = avrtest_ftod64 (-10.0f);
    L x1 = avrtest_ftod64 (+10.0f);
    L y0 = avrtest_ftod64 (-10.0f);
    L y1 = avrtest_ftod64 (+10.0f);

    s_fun = "x+y";
    atest2_show_cycles (x0, x1, y0, y1, NN);

    return 0;
}
