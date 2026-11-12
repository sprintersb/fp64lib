#ifndef NN
#define NN 1000
#endif

#define FUN atan

#include "func1.h"

int main (void)
{
    L x0 = avrtest_ftod64 (-10.0f);
    L x1 = avrtest_ftod64 (+10.0f);

    s_fun = "atan10";
    atest1_show_cycles (x0, x1, NN);

    return 0;
}
