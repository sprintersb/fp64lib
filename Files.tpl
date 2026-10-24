# Files.am is also included by include/avr/fp64lib/Makefile.am

fp64lib_a_c_sources =

# List all files since SUBST_BASENAME_FILES is not allowed in _SOURCES.
# sed 's:^\(.*\):\t\1 \\:;${s:[ \t]*\\$::}' <<< $(ls fp64*.S)
fp64lib_a_asm_sources = \
@@@

# Populated by include/avr/fp64lib/Makefile.am
fp64lib_h = fp64def.h fp64lib.h Double.h

fp64lib_a_extra_dist = \
	fp64asm.h \
	$(fp64lib_h) \
	README.md
