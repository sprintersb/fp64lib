#include "base.h"

// Test tristate comparators.

#define PROTO(f, op)                                                \
    extern int8_t tricmp_##f (L, L) __asm("__" #f "df2.tristate");  \
    static bool cmp_##f (L x, L y)                                  \
    {                                                               \
        return tricmp_##f (x, y) op 0;                              \
    }

PROTO (ne, !=)
PROTO (eq, ==)
PROTO (le, <=)
PROTO (lt, <)
PROTO (ge, >=)
PROTO (gt, >)

#define MK_CMP(f, op)                                                   \
    UU NI void test_##f (L x, L y)                                      \
    {                                                                   \
        bool z1 = cmp_##f (x, y);                                       \
        bool z2 = (avrtest_cmp_d64 (x, y) op 0);                        \
        if (z1 != z2)                                                   \
        {                                                               \
            printf ("\n%s (%+.16De, %+.16De)\n", #f, x, y);             \
            printf ("result: %d\n", z1);                                \
            printf ("expect: %d\n", z2);                                \
            if (!dont_exit)                                             \
                exit (10);                                              \
        }                                                               \
    }

MK_CMP (ne, !=)
MK_CMP (eq, ==)
MK_CMP (le, <=)
MK_CMP (lt, <)
MK_CMP (ge, >=)
MK_CMP (gt, >)

void test1 (L x, L y)
{
    test_ne (x, y);
    test_eq (x, y);
    test_le (x, y);
    test_lt (x, y);
    test_ge (x, y);
    test_gt (x, y);
}

NI void test2 (L x, L y)
{
    test1 (x, y);
    test1 (y, x);
}

NI void test8 (L x, L y)
{
    L mx = fp64_neg (x);
    L my = fp64_neg (y);
    test1 (x, y);
    test1 (x, my);
    test1 (mx, y);
    test1 (mx, my);
}

NI void test72 (L x, L y)
{
    test8 (x, y);
    test8 (x, y + 1);
    test8 (x, y - 1);
    test8 (x + 1, y);
    test8 (x + 1, y + 1);
    test8 (x + 1, y - 1);
    test8 (x - 1, y);
    test8 (x - 1, y + 1);
    test8 (x - 1, y - 1);
}

static bool good (L x)
{
    return fp64_isfinite (x) && x != L_p0 && x != L_m0;
}

int main (void)
{
    test8 (L_p0, L_p0);
    test8 (L_p0, L_p1);
    test8 (L_p0, L_pinf);
    test8 (L_p1, L_pinf);
    test8 (1, L_pinf);
    test8 (1, L_p0);

    for (int i = 0; i < 500; ++i)
    {
        L x, y;
        do
            x = rand_u64 ();
        while (! good (x));
        do
            y = rand_u64 ();
        while (! good (y));

        test72 (x, y);
    }

    return 0;
}
