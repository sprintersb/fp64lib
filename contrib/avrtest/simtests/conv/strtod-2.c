#include "base.h"

void add_digs (char **s, uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i)
    {
        char d;
        do
            d = '0' + (15 & avrtest_prand ());
        while (d > '9');

        *(*s)++ = d;
    }
    **s = '\0';
}

void maybe_add_sign (char **s, bool maybe_space)
{
    uint8_t r = avrtest_prand ();

    if ((r & 3) == 1 && maybe_space)
        *(*s)++ = ' ';
    else if ((r & 3) == 2)
        *(*s)++ = '+';
    else if ((r & 3) == 3)
        *(*s)++ = '-';
    **s = '\0';
}

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

void testit (void)
{
    char str[100], *s = &str[0];

    maybe_add_sign (&s, 1);
    add_digs (&s, 1 + (3 & avrtest_prand ()));

    stest (str);

    if (1 & avrtest_prand ())
        *s++ = '.';

    add_digs (&s, 1 + (3 & avrtest_prand ()));

    stest (str);

    if (1 & avrtest_prand ())
    {
        *s++ = 'e';
        maybe_add_sign (&s, 0);
        add_digs (&s, 1 + (3 & avrtest_prand ()));
    }

    stest (str);
}

int main (void)
{
    for (int i = 0; i < 1000; ++i)
        testit ();

    return 0;
}
