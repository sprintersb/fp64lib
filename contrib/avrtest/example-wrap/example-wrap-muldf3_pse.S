#include "fp64asm.h" /* ENTRY, ENDFUNC */
#include "avrtest.h" /* LOG_REGS */

/*  To add a new peek function, append the line

        MAKE_PEEK <myfun>

    too peek-asm.S, and implement

        void c_<myfun> (const peek_t *p);

    in peek.c.  Then invoke it like

        XCALL <myfun>                          ; pass cookie -1
        XCALL <myfun> $ mov <cookie>, <cookie> ; pass cookie <cookie>

    from assembly code, making sure that the call
    is NOT FOLLOWED BY A RET when linker relaxation is on.  */


;;; App is linked with --wrap=__fp64_muldf3_pse, so the linker will
;;; resolve symbol S to __wrap_S.  We use this to inject
;;; some logging / printing around __fp64_muldf3_pse.

ENTRY __wrap___fp64_muldf3_pse
    ;; Show GPRS, SP, SREG
    LOG_REGS                    ; doesn't change CPU state

    ;; Print AX with cookie #2 and BX without a cookie.
    XCALL peekax $ mov 2,2      ; doesn't change CPU state
    XCALL peekbx                ; doesn't change CPU state

    ;; App is linked with --wrap=__fp64_muldf3_pse, so the following
    ;; call invokes the real thing.
    XCALL __real___fp64_muldf3_pse

    ;; Print AX without cookie indicates result.
    XCALL peekax                ; doesn't change CPU state
    NOP                         ; defeat relaxing!
    ret
ENDFUNC
