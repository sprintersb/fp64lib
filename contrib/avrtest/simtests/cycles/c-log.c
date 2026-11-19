#ifndef NN
#define NN 1000
#endif

#define FUN log

#include "func1.h"

int main (void)
{
    L x0 = avrtest_ftod64 (+0.001f);
    L x1 = avrtest_ftod64 (+100.0f);

    atest1_show_cycles (x0, x1, NN);

    return 0;
}
