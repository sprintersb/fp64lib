#include "base.h"

void one_test (L x, int nn)
{
    L z1 = fp64_ldexp (x, nn);
    L z2 = avrtest_ldexp_d64 (x, nn);

    if (! same (z1, z2))
    {
        printf ("\n%s (%+.16De, %d=0x%x)\n", "ldexp", x, nn, nn);
        printf ("result: %+.16De = %+.13Da\n", z1, z1);
        printf ("expect: %+.16De = %+.13Da\n", z2, z2);
        exit (1);
    }
}

void many_tests (L x)
{
    for (int nn = 0; nn <= 1060; ++nn)
        one_test (x, nn);

    for (int nn = 1; nn <= 1080; ++nn)
        one_test (x, -nn);
}

int main (void)
{
    many_tests (avrtest_ftod64 (+1.75f));
    many_tests (avrtest_ftod64 (-1.00f) - 1);

    return 0;
}
