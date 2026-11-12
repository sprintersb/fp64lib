SHELL = /bin/bash

# To use a different MCU, build with
# $ make ... MCU=atxmega128a1
MCU = atmega328

# To use a different AVR GCC, build with
# $ make ... CC=avr-gcc-16
CC = avr-gcc

FP64LIB_HOME = ../../..

ifeq ($(AVRTEST_HOME),)
# AVRTEST_HOME is not set:  Try to work out the AVRtest home folder.
# If not found, you can add the AVRtest home folder to PATH, or you
# can build with
# $ make ... AVRTEST_HOME=<avrtest-home-folder>
AVRTEST_HOME := $(dir $(shell which avrtest))
ifeq ($(AVRTEST_HOME),)
$(warning warning: avrtest not found in the PATH, use: make ... AVRTEST_HOME=...)
endif
endif
ifneq ($(AVRTEST_HOME),)
$(info using AVRTEST_HOME = $(AVRTEST_HOME))
endif

# Wrap __fp64_muldf3_pse so we can hook in functions like peeka / c_peeka.
# In order to NOT wrap the multiplication routine, build with
# $ make ... WRAPS=
# The wrapper is implemented in example-wrap-muldf3_pse.S so that we can
# change fp64lib functionality without changing fp64lib sources or the
# need to re-build libfp64.a.  re.linking is all what's needed.
WRAPS = __fp64_muldf3_pse

wlwrap = -Wl,--wrap=
WL_WRAPS := $(patsubst %,$(wlwrap)%,$(WRAPS))

# Compile-in AVRtest's exit module.  It provides convenient exit() and
# abort() functions, implements stderr and stdout as writing to the
# host's stderr / stdout, and sets up argc and argv according to
# $ avrtest ... -args <args>
# If you don't want that, build with
# $ make ... exit_c=
exit_c = $(AVRTEST_HOME)/dejagnuboards/exit.c

# For additional compile and link flags, build with
# make ... CFLAGS='<flags>'
OPTS = -mmcu=$(MCU) -std=gnu99 -fno-strict-aliasing -Wall
OPTS += -Os -mrelax -ffunction-sections -Wl,--gc-sections
OPTS += -I$(AVRTEST_HOME) $(exit_c)
OPTS += -I$(FP64LIB_HOME) -L$(FP64LIB_HOME) -lfp64-$(MCU)
OPTS += -Wl,-Map=$(@:.elf=.map)
OPTS += $(CFLAGS)

.PHONY: all elf lst run help force clean
.PHONY: example-wrap fp64lib clean-fp64lib

all: lst run

help:
	@cat help.txt

elf: example-wrap-$(MCU).elf

lst: example-wrap-$(MCU).lst

fp64lib: $(FP64LIB_HOME)/libfp64-$(MCU).a

run: run-example-wrap

LOG = _log

run-example-wrap: example-wrap-$(MCU).elf force
	$(shell $(AVRTEST_HOME)/avrtest-elf $< $(LOG)) $< -v -no-log -args 1.0

$(FP64LIB_HOME)/libfp64-$(MCU).a: force
	cd $(FP64LIB_HOME); $(MAKE) MCU=$(MCU) XCC=$(CC)

EXAMPLE_WRAP_SRCS = ../peek.c ../peek-asm.S
EXAMPLE_WRAP_SRCS += example-wrap.c
EXAMPLE_WRAP_SRCS += example-wrap-muldf3_pse.S
EXAMPLE_WRAP_SRCS += example-real-muldf3_pse.S

# Force remake to simplify building for changing MCUs
example-wrap-$(MCU).elf: $(FP64LIB_HOME)/libfp64-$(MCU).a force
	$(CC) -o $@ $(EXAMPLE_WRAP_SRCS) $(OPTS) $(WL_WRAPS)

%.lst: %.elf
	avr-objdump -d $< > $@

clean:
	rm -f -- $(wildcard *.elf *.lst *.map *.[iso])

clean-fp64lib:
	cd $(FP64LIB_HOME); $(MAKE) clean
