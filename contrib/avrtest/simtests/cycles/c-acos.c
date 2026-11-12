#ifndef NN
#define NN 1000
#endif

#define FUN acos

#include "func1.h"

int main (void)
{
    L x0 = avrtest_ftod64 (-1.0f);
    L x1 = avrtest_ftod64 (+1.0f);

    atest1_show_cycles (x0, x1, NN);

    return 0;
}
