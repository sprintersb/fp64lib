This folder contains a descendant of Uwe Bissinger's fp64lib
from https://github.com/fp64lib/fp64lib

fp64lib is structured like AVR-LibC's libm/fplib, and the files
use the same license header.  Files that are under a different licence, like
code from Florian Königstein's avr_f64.c implementation, are not included.
These are files that address double &harr; string converisions.

### Incentives

* Provide fp64lib to a wider audience than just Arduino.
* Integrate fp64lib into the GNU tools so that it can be used
without requiring extra build or configurations steps.
* fp64lib can be used without any extra header files or configuration
files and whatnot.
* Fix subtle problems like ABI violations, anto patterns, build warnings, etc,
* Make fp64lib available as an alternative to LibF7 — currently the
only 64-bit double implementation that is integrated into the tools —
without the requirement to re-configure the tools, or to use special
headers or config files.
Though when AVR GCC doesn't support 64-bit double, an interface
header could be supplied as a convenience.

### Changes to the build system
* Added configure option `--enable-fp64lib` (default) to build `libfp64.a`.
* Add macros `DOUBLE_COMPARE_BOOL` and `DOUBLE_COMPARE_TRISTATE` to `config.h`.
* Display these in the FAQ→Toolchain section.

### ToDo

* Integrate fp64lib into the AVR-LibC build system &check;
* Fix all build errors and warnings &check;
* The previous point has beed achieved by inserting `.end`
directives when ISA features are required that fp64lib doesn't
support yet:
   * MUL: `.require_mul`
   * LPMx, ELPMx: `.require_lpmx`

  The goal is to generalize the implementation such that these
markers can be removed.
* Adjust the ABI such that
  * Functions match the GCC ABI and `<math.h>` interface &check;
  * Set `.gnu_attribute` as appropriate &check;
  * Avoid namespace pollution by using a `__` namespace for globals.
* Don't mix lookup tables and executable code &check;
* Don't put @function symbols in data sections &check;
* Fix EIND usage and related ABI breakage &check;
* Fix RAMPZ usage and related ABI breakage &check;
* Fix Arduino related stuff like `#ifdef ARDUINO_AVR_MEGA2560` &check;
* Fix non-reentrant anti patterns.
