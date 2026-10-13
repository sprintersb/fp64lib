This folder contains a descendant of Uwe Bissinger's fp64lib
from https://github.com/fp64lib/fp64lib

fp64lib is a mostly IEEE double compliant implementation for AVR GCC.

fp64lib is structured like AVR-LibC's libm/fplib, and the files
use the same license header.  Files that are under a different licence,
like code from Florian Königstein's avr_f64.c implementation, are not
included.  These are files that address double &harr; string converisions.

### Goals and Incentives

* Provide fp64lib to a wider audience than just confined to a couple
of Arduino boards.
* Integrate fp64lib into the GNU tools so that it can be used from
vanilla C/C++ code after including `<math.h>`.
Remove the need for extra build rules or config additions or interface files.
* Fix subtle problems like ABI violations, anti patterns, build warnings, etc,
* Make fp64lib available as an alternative to LibF7 — currently the
only 64-bit IEEE double implementation that is integrated into the tools —
without the requirement to re-configure the tools, or to use special
headers or config files.
Though when AVR GCC doesn't support 64-bit double, interfaces
like `<avr/fp64lib/fp64lib.h>` and `<avr/fp64lib/Double.h>` are available.

### Differences to the upstream fp64lib

* Integrates fp64lib into the AVR-LibC build system &check;
* Fix all build errors and warnings &check;
* Adjust the ABI such that
  * Functions match the GCC ABI and `<math.h>` interface &check;
  * Set `.gnu_attribute` as appropriate &check;
* Fix EIND usage and related ABI breakage &check;
* Fix RAMPZ usage and related ABI breakage &check;
* Don't mix lookup tables and executable code &check;
* Don't put @function symbols in data sections &check;
* Put global states into the `.bss` section and not into `.data` &check;
* Resolve Arduino related stuff like `#ifdef ARDUINO_AVR_MEGA2560` &check;

### fp64lib Limitations

* fp64lib is not available for all AVR devices / core archiectures.
The code bluntly inserts `.end` directives when ISA features are required
that fp64lib doesn't support:
   * MUL: `.require_mul`
   * LPMx, ELPMx: `.require_lpmx`

  Consequence are *"undefined reference to"* errors from the linker
when an attempt is being made to link against `libfp64.a` for the
following GCC cores: avr2, avr25, avr3, avr31, avr35 and avrtiny.
For a list of respective devices, see the [AVR GCC Options][1].

[1]: https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html

* For some functions and input values, the relative error may exceed
thousands of ULPs (Unites in the L(e)ast Place).  This occurs close to
* the non-zero zeros and poles (if any) of the trigonometric
functions `sin`, `cos`, `tan`.
* the zero of `sinh` and `tanh`.

* fp64lib doesn't (yet) implement some features:
  * `pow (x,y)` for negative `x` and `y`.
  * Special functions like gamma and bessel.

* fp64lib is not re-entrant, i.e. at most one thread of execution may
invoke libfp64 functions at a time.

On top of that, AVR-LibC doesn't fully support IEEE double:
* The printf and scanf families of function don't support IEEE double.
Though they correctly process / consume the passed arguments, but they
print double values as `?`, see the [AVR-LibC documentation][2].

[2]: https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__avr__stdio.html#gaa3b98c0d17b35642c0f3e4649092b9f1

### Changes to the AVR-LibC Build Bystem

* Added configure option `--enable-fp64lib` (default) to build `libfp64.a`.

* Add macros `DOUBLE_COMPARE_BOOL` and `DOUBLE_COMPARE_TRISTATE` to `config.h`
and display them in the FAQ → Toolchain section of the documentation.
They are needed for the correct implementation of double comparisons.

* Add a `__AVR_LIBC_WITH_FP64LIB__` macro to `<avr/version.h>`.

* Populate the `fp64lib.h`, `fp64def.h`, `Double.h` headers to
`avr/include/avr`.
