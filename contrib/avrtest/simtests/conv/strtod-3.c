#include "base.h"

void stest (const char *str)
{
    int ulp_max = 1;

    L z1 = fp64_strtod (str, NULL);
    L z2 = avrtest_strtod64 (str, NULL);

    L ulp = avrtest_ulp_d64 (z1, z2);
    ulp = avrtest_fabs_d64 (ulp);

    if (avrtest_cmp_d64 (ulp, avrtest_s32tod64 (ulp_max)) > 0)
    {
        printf ("\n%s (%s, NULL)\n", "strtod", str);
        printf ("result: %+.16De = %+.13Da\n", z1, z1);
        printf ("expect: %+.16De = %+.13Da\n", z2, z2);
        exit (3);
    }
}

void testit (int ex)
{
    char str[100], *s = &str[0];

    *s++ = '-';

    *s++ = '1';
    *s++ = '.';
    *s++ = '9';
    *s++ = '9';
    *s++ = '9';

    *s++ = 'e';

    itoa (ex, s, 10);

    stest (str);
    stest (str + 1);
    stest (str + 3);
}


int main (void)
{
    for (int i = -20; i <= 20; ++i)
        testit (i);

    for (int i = 300; i <= 340; ++i)
    {
        testit (i);
        testit (-i);
    }

    return 0;
}
