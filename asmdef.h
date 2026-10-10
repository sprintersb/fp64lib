/* Copyright (c) 2006  Dmitry Xmelkov
   Copyright (c) 2026  Georg-Johann Lay
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

/* $Id$	*/

#ifndef FP64_ASMDEF_H
#define FP64_ASMDEF_H

#include_next "asmdef.h"

;;; An entry for a function that would usually live in GCC's libgcc.
.macro  GCC_ENTRY name
	.global	\name
	.type	\name, "function"
\name:
.endm

;;; An entry for a function from math.h, FD for double and FL for long double.
;;; Other than ENTRY_FLOAT, we don't .weak-en the symbol so that it is
;;; stronger than libgcc/libf7's, which are all weak.
.macro  ENTRY_DOUBLE fd, fl
#if __SIZEOF_DOUBLE__ == 8
	.global	\fd
	.type	\fd, "function"
#ifdef HAVE_LD_GNU_ATTRIBUTE_8
	.gnu_attribute Tag_GNU_AVR_BITS_DOUBLE,64
#endif
\fd:
#endif /* __SIZEOF_DOUBLE__ == 8 */

#if __SIZEOF_LONG_DOUBLE__ == 8
	.global	\fl
	.type	\fl, "function"
#ifdef HAVE_LD_GNU_ATTRIBUTE_12
	.gnu_attribute Tag_GNU_AVR_BITS_LONG_DOUBLE,64
#endif
\fl:
#endif /* __SIZEOF_LONG_DOUBLE__ == 8 */
.endm


/* swap two registers without need of temporary register */
.macro _SWAP	dst, src
	eor dst, src
	eor src, dst
	eor dst, src
.endm

/* swap two words with the help of r0/r1 */
.macro _SWAPW	dst, src
	X_movw r0, \src
	X_movw \src, \dst
	X_movw \dst, r0
.endm

/* alias to be compatible with AVR programming manual */
#define byte3(x)	hh8(x)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Chicken out when the code demands more than the hardware can do.

#if __AVR_TINY__
    .end
#endif

.macro .require_mul
#ifndef __AVR_HAVE_MUL__
    .end
#endif
.endm

.macro .require_lpmx
#if defined (__AVR_HAVE_ELPM__) && !defined (__AVR_HAVE_ELPMX__)
    .end
#elif !defined (__AVR_HAVE_ELPM__) && !defined (__AVR_HAVE_LPMX__)
    .end
#endif
.endm

#endif	/* !FP64_ASMDEF_H */
