#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> // needed for avrtest_printf
#include <avr/pgmspace.h>
#include "avrtest.h"

typedef uint64_t L;

#define NI __attribute__((__noinline__,__noclone__))
#define UU __attribute__((__unused__))

#define printf(msg, ...)   avrtest_printf_P  (PSTR (msg), ##__VA_ARGS__)

#define ARRAY_SIZE(x) (sizeof (x) / sizeof (*x))

#if	defined(__AVR_TINY__)
#define N_REGS 16
#else
#define N_REGS 32
#endif

#if defined(__AVR_HAVE_ELPM__)
#define PRIret "05lx"
#else
#define PRIret "04lx"
#endif

#define FLAG_I_BIT  7
#define FLAG_T_BIT  6
#define FLAG_H_BIT  5
#define FLAG_S_BIT  4
#define FLAG_V_BIT  3
#define FLAG_N_BIT  2
#define FLAG_Z_BIT  1
#define FLAG_C_BIT  0

// As pushed on the stack by peek-asm.S::.pushall
typedef struct
{
    uint8_t rampz;
    uint8_t sreg;
    uint8_t r[N_REGS];
    uint8_t ret[3];
}
peek_t;

UU static bool flagT (const peek_t *p) { return p->sreg & (1 << FLAG_T_BIT); }

typedef uint8_t __attribute__((__vector_size__(4))) vec4_t;

// Get the return location of peek*.  This is also the location of a
// potential cookie (mov <cookie>, <cookie>).
UU static uint32_t ret_addr (const peek_t *p)
{
#if 0
#if defined(__AVR_3_BYTE_PC__)
    uint8_t b0 = p->ret[2];
    uint8_t b1 = p->ret[1];
    uint8_t b2 = p->ret[0];
#else
    uint8_t b0 = p->ret[1];
    uint8_t b1 = p->ret[0];
    uint8_t b2 = 0;
#endif
    uint32_t waddr = (0x100UL * b2 + b1) * 0x100 + b0;
    return 2 * waddr;
#else
    vec4_t waddr = { 0, 0, 0, 0 };
#if defined(__AVR_3_BYTE_PC__)
    waddr[0] = p->ret[2];
    waddr[1] = p->ret[1];
    waddr[2] = p->ret[0];
#else
    waddr[0] = p->ret[1];
    waddr[1] = p->ret[0];
#endif
    return 2 * (uint32_t) waddr;
#endif
}

// fp64lib's extended double representation stuffed into an ordinary double.
UU static L dbl_10 (const peek_t *p, uint8_t r_mant, uint8_t r_expo)
{
    // mant AX has 7 bytes r18..r24 with explicit leading 1.
    uint64_t mant = * (const uint64_t*) &p->r[r_mant - 1];
    mant &= ~0xffL;
    int16_t bias_expo = * (const int16_t*) &p->r[r_expo];
    int16_t expo = bias_expo - 1023;
    L dbl = avrtest_ldexp_d64 (avrtest_u64tod64 (mant), expo - 63);
    return dbl;
}

UU static L dbl_A10 (const peek_t *p)
{
    return dbl_10 (p, 18, 30);
}

UU static L dbl_B10 (const peek_t *p)
{
    return dbl_10 (p, 10, 26);
}

static inline bool rbit (const peek_t *p, int r, int b)
{
    return p->r[r] & (1 << b);
}

// Cookie: Is there a  mov <cookie>, <cookie>  at the return location?
UU static int8_t get_cookie (const peek_t *p)
{
    uint32_t ret = ret_addr (p);
#if defined __AVR_HAVE_ELPM__
    uint16_t code = pgm_read_word_far (ret);
#else
    uint16_t code = pgm_read_word ((uint16_t) ret);
#endif
    bool is_mov = 0x2c00 == (code & 0xfc00);
    if (is_mov)
    {
        uint8_t op2 = (code & 0x0F) | ((code >> 5) & 0x0010);
        uint8_t op1 = (code >> 4) & 0x1F;
        if (op1 == op2)
            return op1;
    }
    return -1;
}

UU static int8_t show_cookie (const peek_t *p)
{
    static const char s_cookie[][4] PROGMEM =
    {
        "00",
        "add", // 1
        "mul", // 2
        "div", // 3
        "res", // 4
        "05", "06", "07", "08", "09",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
        "30", "31"
    };

    int8_t c = get_cookie (p);
    if (c == 0)
        printf ("\n");
    printf ("%" PRIret " ", ret_addr (p));
    if (c >= 0)
        printf ("%02d.%-3S ", c, s_cookie[c]);
    else
        printf ("       ");

    return c;
}

UU static void show_flags (const peek_t *p)
{
    char s_sreg[9] = "--------";
    uint8_t sreg = p->sreg;
    if (sreg & FLAG_I_BIT)  s_sreg[0] = 'I';
    if (sreg & FLAG_T_BIT)  s_sreg[0] = 'T';
    if (sreg & FLAG_H_BIT)  s_sreg[0] = 'H';
    if (sreg & FLAG_S_BIT)  s_sreg[0] = 'S';
    if (sreg & FLAG_V_BIT)  s_sreg[0] = 'V';
    if (sreg & FLAG_N_BIT)  s_sreg[0] = 'N';
    if (sreg & FLAG_Z_BIT)  s_sreg[0] = 'Z';
    if (sreg & FLAG_C_BIT)  s_sreg[0] = 'C';

    printf ("sreg=0x%02x=%s", sreg, s_sreg);
}

UU static void show_d8 (const peek_t *p, uint8_t reg, char id)
{
    const L *f = (const L*) &p->r[reg];
    const uint64_t *i = (const uint64_t*) &p->r[reg];
    printf ("%c = %.13Da = 0x%016llx = %.16Dg\n", id, *f, *i, *f);
}

UU static void show_d10 (const peek_t *p, uint8_t reg, uint8_t r_expo, char id)
{
    L d10 = dbl_10 (p, reg, r_expo);
    printf ("%cX =% 23.16Dg  S=%d  T=%d: (%02x) ",
            id, d10, rbit (p, reg+7, 7), flagT(p), p->r[reg+7]);
    for (int8_t i = 6; i >= 0; --i)
        printf ("%02x,", p->r[reg + i]);
    int16_t expo = *(const int16_t*) & p->r[r_expo];
    printf (" %04x=1023%+d\n", expo, expo - 1023);
}

void c_peeka (const peek_t *p)
{
    show_cookie (p);
    show_d8 (p, 18, 'A');
}

void c_peekb (const peek_t *p)
{
    show_cookie (p);
    show_d8 (p, 10, 'B');
}

void c_peekc (const peek_t *p)
{
    show_cookie (p);
    show_d8 (p, 2, 'C');
}

void c_peekax (const peek_t *p)
{
    show_cookie (p);
    show_d10 (p, 18, 30, 'A');
}

void c_peekbx (const peek_t *p)
{
    show_cookie (p);
    show_d10 (p, 10, 26, 'B');
}

void c_peekx (const peek_t *p)
{
    printf ("rA7 = %d, X = %d\n",
            (int8_t) p->r[25], *(const int*) & p->r[26]);
}

#define	rB7	17
#define	rB6	16
#define	rB5	15
#define	rB4	14
#define	rB3	13
#define	rB2	12
#define	rB1	11
#define	rB0	10

#define rR8 6
#define rR7 7
#define rR6 8
#define rR5 9

#define MM0 rR6 /* r8 */
#define MM1 rR5 /* r9 = 1 + MM0 */
#define MM2 rR8 /* r6 */
#define MM3 rR7 /* r7 = 1 + MM2 */
#define MM4 rB0
#define MM5 rB1
#define MM6 rB2
#define MM7 rB3
#define MM8 rB4

void c_peekmm (const peek_t *p)
{
    printf ("    r%2d %2d %2d %2d %2d %2d %2d %2d %2d\n",
            MM8, MM7, MM6, MM5, MM4, MM3, MM2, MM1, MM0);
    printf ("MM = %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
            p->r[MM8], p->r[MM7], p->r[MM6], p->r[MM5], p->r[MM4],
            p->r[MM3], p->r[MM2], p->r[MM1], p->r[MM0]);
}
