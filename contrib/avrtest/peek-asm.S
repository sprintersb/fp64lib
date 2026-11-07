#include "avrtest.h" /* LOG_PUSH_OFF, LOG_POP */
#include "fp64asm.h"

#ifdef __AVR_TINY__
#define N_REGS 16
#else
#define N_REGS 32
#endif

.macro .pushall
    ;; Turn off instruction logging and push the current
    ;; log state (on/off) to be resurrected at the end of .popall.
    LOG_PUSH_OFF

    ;; Store GPRs
    .reg = 32
    .rept N_REGS
        .reg = .reg - 1
        push .reg
    .endr

    clr     __zero_reg__                    ; GCC ABI

    ;; Store SREG, RAMPZ.
    in      __tmp_reg__, SREG_IO_ADDR   $  push    __tmp_reg__
#ifdef RAMPZ
    in      __tmp_reg__, RAMPZ_IO_ADDR  $  push    __tmp_reg__
    out     RAMPZ_IO_ADDR, __zero_reg__
#else
    push    __zero_reg__
#endif

    ;; Pass stack location as  peek_t *R24
    in      r24, SPL_IO_ADDR
    in      r25, SPH_IO_ADDR
#ifdef __AVR_TINY__
    subi    r24, lo8(-1)
    sbci    r25, hi8(-1)
#else
    adiw    r24, 1
#endif
.endm


.macro .popall
    ;; Restore RAMPZ, SREG.
    pop     __tmp_reg__
#ifdef RAMPZ
    out     RAMPZ_IO_ADDR, __tmp_reg__
#endif
    pop     __tmp_reg__
    out     SREG_IO_ADDR,  __tmp_reg__

    ;; Restore GPRs
    .reg = 32 - N_REGS
    .rept N_REGS
        pop .reg
        .reg = .reg + 1
    .endr

    ;; Resume instruction logging as pushed in .pushall.
    LOG_POP
.endm


/* Make function FUN that
   1) Turns off instruction logging,
   2) Saves the CPU state (SREG, RAMPZ, GPRs)
   3) Runs c_FUN
   4) Restores the CPU state
   5) Continues with the previous instruction logging state of on/off.  */

.macro MAKE_PEEK fun
    .section .text.\fun,"ax","progbits"
\fun:
    .pushall
    XCALL   c_\fun
    XJMP    pop_all
    .global \fun
    .type   \fun, "function"
    .size   \fun, . - \fun
.endm


;;; Common tail for all the peek* functions below.
    .type pop_all, "function"
    .section .text.pop_all,"ax","progbits"
pop_all:
    .popall
    ret
    .size pop_all, . - pop_all


/* Make them functions.  They save the CPU state (GPR, SREG, RAMPZ),
   run c_peek*, and then restore the CPU state.  The function calls
   can be dropped anywhere in the assembly source (though not after
   a skip insn), like in fp64_powserx.S:

   XCALL peekax $ mov 1,1
   XCALL peekbx
      XCALL _U(__fp64_add_pse)  ; create x*Cn + Cn-1...
      bld rA7, 7                ; save sign bit
   XCALL peekax

   The first peekax gets a "cookie" of 1, whereas the other calls don't
   get a cookie (cookie is -1).  */

MAKE_PEEK peeka
MAKE_PEEK peekb
MAKE_PEEK peekc

MAKE_PEEK peekax
MAKE_PEEK peekbx

MAKE_PEEK peekx
