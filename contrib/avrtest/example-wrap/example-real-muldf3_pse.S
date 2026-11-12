/* This is only needed when linked WITHOUT

       --wrap=__fp64_muldf3_pse

   Without a definition of __real___fp64_muldf3_pse the linker would fail
   with an undefined symbol error because __wrap___fp64_muldf3_pse is calling
   __real___fp64_muldf3_pse.  The definition below must not be located
   in  example-wrap-muldf3_pse.S  so that the assembler generates a
   relocation for that call.  */

.global __real___fp64_muldf3_pse
__real___fp64_muldf3_pse = 0
