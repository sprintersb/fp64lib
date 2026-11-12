The two sub-folders show how the
[AVRtest](https://github.com/sprintersb/atest) AVR core simulator
can be used for testing and debugging fp64lib.

Advantages:

* Syscalls provide a wide variety of features with almost no
cost for the target:
  * Logging values to the host's stdout or stderr.
  * Emulating printf, **IEEE double arithmetic**, pseudo-random numbers,
  cycle counter.
  * Passing command line parameters to the program (argc / argv).
  * Rudimentary fail detection like: Stack underflow, re-running of `main`,
  locating or running code outside of the `text` section.

* AVRtest is fast.  Simulation speed is comparable to an AVR running at 90 MHz.

* No uploading to a device is needed.  No Flash wear-out due to bulk testing.

* Instruction logging.

* A divice can be quipped with more RAM than the physical device actually has.
This extends the range of conceivable tests.

* AVRtest is used by the GCC testsuite, so pretty mature and stable.

Disadvantages:

* Programs must be re-compiled to use the AVRtest features.

* Ordinary AVR executables that make use of I/O (ports, EEPROM, timers)
won't work with AVRtest.

* No GDB server support, so no "real" debugging is possible.

## example-wrap

Provide functions that can called from anywhere in the assembly code.
The workers are imlemented in C and display formats like IEEE double
or the internal extended float format.

For a start, `cd example-wrap` and run
```
$ make
$ make help
```

## simtests

A small testsuite for fp64lib functions. It just indicates a possible
route to automatically testing fp64lib.  For a start, `cd simtests` and run
```
$ ./run-avrtest.sh
$ ./run-avrtest.sh -h
$ MCUS='atmega328' ./run-avrtest.sh trig/sin-*.c
```
