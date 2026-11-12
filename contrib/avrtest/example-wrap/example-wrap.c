#include <stdint.h>
#include <stdio.h>        // required by avrtest_printf
#include <avr/pgmspace.h> // PSTR
#include "avrtest.h"
#include "fp64lib.h"

// For convenience, introduce PSTR and avrtest_* for printing and diagnosing.
// Notice that avrtest_*printf is more capable than AVR-LibC's printf since
// the former can print IEEE double.
#define printf(msg, ...)      avrtest_printf_P  (PSTR (msg), ##__VA_ARGS__)
#define fprintf(f, msg, ...)  avrtest_fprintf_P (f, PSTR (msg), ##__VA_ARGS__)

int main (int argc, char *argv[])
{
    float xf = 1.0f;

    if (argc > 1)
    {
        char *end;
        xf = avrtest_strtof (argv[1], &end);
        if (*end)
        {
            fprintf (stderr, "bad float argv[1]: %s\n", argv[1]);
            avrtest_exit (1);
        }
    }

    float64_t x = avrtest_ftod64 (xf);

    // Let fp64lib calculate exp, recording consumed cycles.
    // This includes the cycles consumed by the peek functions!
    // In order to see the cycles for the bare fp64lib function,
    // build with  $ make ... WRAPS=
    avrtest_cycles_call ();
    float64_t y = fp64_exp (x);
    uint32_t cycles = avrtest_cycles ();

    // Get exp as calculated by the host's (the machine running AVRtest)
    // implementation of exp.
    float64_t z = avrtest_exp_d64 (x);

    // Print results and delta.  Print flag 'D' is for float64_t, i.e.
    // 64-bit IEEE double irrespective of the size of AVR GCC's long double.
    printf ("\n== Results ==\n");
    printf ("exp(%Df) = %.15Df = %.14Da // fp64lib\n", x, y, y);
    printf ("exp(%Df) = %.15Df = %.14Da // host exp\n", x, z, z);
    printf ("delta ULP = %Df\n", avrtest_ulp_d64 (y, z));

    printf ("cycles: %lu\n", cycles);

    // All fine.  The following exit() is provided by
    // $(AVRTEST_HOME)/dejagnuboards/exit.c.
    // It shows some information about the terminated program (cycles,
    // reason of termination, ...).  If you prefer quiet, simulate with
    // $ avrtest -q ...
    // If you prefer the stock version of exit(), use
    // $ make ... exit_c=
    return 0;
}
