#! /bin/bash

# Copyright (c) 2007, Dmitry Xmelkov
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of the copyright holders nor the names of
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Use
#
#     AVRTEST_HOME="..." ./run-avrtest.sh ...
#
# to specify where the AVRtest executables and headers can be found.
# This is only needed when the AVRtest home folder is not already in
# the PATH.
#
# Use
#
#     MCUS="..." ./run-avrtest.sh ...
#
# in order to override the predefined list of MCUs.
# The cycles/ cases will onyl be performed for the 1st MCU.
#
# Use
#
#     CFLAGS="-save-temps" ./run-avrtest.sh ...
#
# in order to add additional CFLAGS.
# To use a compiler other than avr-gcc, use
#
#     CC=my-compiler ./run-avrtest.sh ...
#
# In order to pass additional arguments to the avrtest executable, use
#
#     AARGS='...' ./run-avrtest.sh ...


set -e

myname=$(basename $0)

: ${CC:=avr-gcc}
FP64LIB_HOME=${FP64LIB_HOME-../../..}

Errx ()
{
    echo "$myname: $*"
    exit 1
}

if [ -z $AVRTEST_HOME ]; then
    if [ -z $(which avrtest) ]; then
	Errx "error: no avrtest executable found in the path, set AVRTEST_HOME"
    else
	AVRTEST_HOME=$(dirname $(which avrtest))
    fi
    echo "using AVRTEST_HOME=$AVRTEST_HOME"
fi

: ${MCU_LIST="atmega328 atxmega128a1 avr128da32" }

FLAG_STOP=			# Stop at any error
FLAG_PRINT=			# Show what AVRtest prints
AVRTEST_EXIT_NO_MPFR=97


Usage ()
{
    cat <<EOF
Usage: $1 [-shpv] [FILE]...
Options:
  -s          Stop at any error, temporary files will save
  -h          Show this help
  -p          Print the AVRtest output (if any)
  -v          Verbose mode; echo shell commands being executed
If FILE is not specified, the full test list is used.
EOF
}

OPTS="shpv"

# First option pass for -h only so that we get the defaults right when
# options like -a are specified.
while getopts $OPTS opt ; do
    case $opt in
	h)	Usage `basename $myname` ; exit 0 ;;
    esac
done

# Second option pass: When -h was not specified, do the work.
OPTIND=1
while getopts $OPTS opt ; do
    case $opt in
	p)	FLAG_PRINT=1 ;;
	s)	FLAG_STOP=1 ;;
	h)	Usage `basename $myname` ; exit 0 ;;
	v)	set -x ;;
	*)	Errx "Invalid option(s). Try '-h' for more info."
    esac
done
shift $((OPTIND - 1))

test_list=${*:-"arith/*.c trig/*.c exp-log/*.c discrete/*.c cycles/*.c"}

# -Wno-array-bounds: Ditch wrong warnings due to avr-gcc PR105523.
# This works with more GCC versions than --param=min-pagesize=0.
cflags="-std=gnu99 -W -Wall -Os -mcall-prologues -mrelax"
cflags="${cflags} -Iinclude"
cflags="${cflags} -I$FP64LIB_HOME -L$FP64LIB_HOME"
cflags="${cflags} -I$AVRTEST_HOME $AVRTEST_HOME/dejagnuboards/exit.c"

Err_echo ()
{
    echo "*** $myname: $*"
    if [ $FLAG_STOP ] ; then
	Errx "Stop"
    fi
}

# $1 = ELF file
# $2 = mcu as understood by avr-gcc
# Extra options for AVRtest are passed in o_sim.
Simulate_avrtest ()
{
    # The following exit stati will be returned with -q:
    # -  0  Everything went fine.
    # -  1  Target program called abort()
    # -  x  Target program called exit (x)
    # - 10  Program timeout as set by -m MAXCOUNT.
    # - 11  Something is wrong with the program file:  Does not fit into
    #       memory, not an AVR executable, ELF headers broken, ...
    # - 12  The program goes haywire:  Stack overflow, illegal instruction
    #       or PC.
    # - 13  Problem with symbol information like an odd function address.
    # - 14  Problem with using file I/O with the host's file system: Bad file
    #       handle or file name, illegal argument.  This does *not* encompass
    #       when fopen cannot open the file; this will just return a 0-handle.
    # - 20  Out of memory.
    # - 21  Wrong avrtest usage: Unknown options, etc.
    # - 22  Program file could not be found / read.
    # - 23  IEEE single emulation failed.
    # - 24  IEEE double emulation failed.
    # - 25  64-bit fixed-point emulation failed.
    # - 26  A syscall has been invoked with bad arguments.
    # - 42  Fatal error in avrtest.

    # -no-stdin keeps AVRtest from hanging in rare situations of bogus
    # code that tries to read from stdin, but there is no input.

    # Default is to use avrtest for simulation which is faster than
    # avrtest_log.  When you want instruction logging, run like
    #
    # $ LOG=_log ... ./run-avrtest.sh -p ...
    #
    # Notice that this will product LOTS of output which you will
    # only see with -p (print avrtest output).  For better localization,
    # run with logging initialy off  AARGS='-no-log' and then turn it
    # on / off in the app with LOG_ON, LOG_OFF, LOG_PUSH_ON, LOG_POP etc.

    local cmd=$(${AVRTEST_HOME}/avrtest-elf $1 $LOG)
    msg=$($cmd -q -no-stdin $1 -m 600000000 $AARGS 2>&1)
    RETVAL=$?

    if [[ $FLAG_PRINT || $do_cycles ]]; then
	if [[ -n "$msg" ]]; then
	    echo "$msg"
	fi
    fi

    [ $RETVAL -eq 0 ] || [ $RETVAL -eq $AVRTEST_EXIT_NO_MPFR ]
}


# Usage: Compile SRCFILE MCU ELFILE
Compile ()
{
    if [ ! -f $FP64LIB_HOME/libfp64-$2.a ]; then
	(cd $FP64LIB_HOME; make MCU=$2 XCC="$CC")
    fi
    $CC $cflags $at_cflags $CFLAGS -mmcu=$2 -DS_MCU='"'$2'"' $1 -o $3 -lfp64-$2
    if [[ -f "$3" && "$CFLAGS" =~ -save-temps ]]; then
	avr-objdump -d $3 > $lst_file
    fi
}

Extract_Directives ()
{
    at_cflags=
    if at_cflags=$(grep '.* at:cflags { .* }.*' $1); then
	at_cflags=$(sed 's/.* at:cflags { \(.*\) }.*/\1/' <<< "$at_flags")
    fi
}

n_files=0	# number of operated files
n_emake=0	# number of compile/link errors
n_esimul=0	# number of simulation errors
n_skipped=0	# number of skipped tests because AVRtest misses features

for test_file in $test_list ; do
    case "$test_file" in
	logging/*)
	    continue;;
	-* )
	    Err_echo "error: $test_file option specified after the files"
	    exit 1 ;;
    esac

    Extract_Directives "$test_file"

    case `basename $test_file` in

	*.c | *.cpp | *.cc | *.S | *.sx)
	    n_files=$(($n_files + 1))

	    rootname=$(basename $test_file | sed -e 's:\..*::g')

	    do_cycles=; [[ $test_file =~ cycles ]] && do_cycles=1

	    elf_file=$rootname.elf
	    lst_file=$rootname.lst

	    for mcu in ${MCUS-${MCU_LIST}} ; do
		if [[ ! $do_cycles ]]; then
		    echo -n "Simulate: $test_file "
		    echo -n "$mcu ... "
		fi
		if ! Compile $test_file $mcu $elf_file;
		then
		    Err_echo "compile failed"
		    n_emake=$(($n_emake + 1))
		    break
		elif ! Simulate_avrtest $elf_file $mcu
		then
		    Err_echo "simulate failed: $RETVAL"
		    n_esimul=$(($n_esimul + 1))
		else
		    if [[ $RETVAL == $AVRTEST_EXIT_NO_MPFR ]]; then
			n_skipped=$(($n_skipped + 1))
			echo "skipped: avrtest without MPFR"
		    elif [[ ! $do_cycles ]]; then
			echo "OK"
		    else
			# Only do cycles for the 1st MCU.
			break
		    fi
		fi
	    done
	    if ! [[ "$CFLAGS" =~ -save-temps ]]; then
		rm -f $elf_file
	    fi
	    ;;

	*)
	    Errx "Unknown file type: $test_file"
    esac
done

echo "-------"
echo "Done.  Number of operated files: $n_files"
if [ $n_skipped -gt 0 ] ; then
    echo "Number of skipped tests: $n_skipped"
fi

if [ $(($n_emake + $n_ehost + $n_esimul)) -gt 0 ] ; then
    [ $n_emake -gt 0 ]   && echo "*** Compile/link errors: $n_emake"
    [ $n_esimul -gt 0 ]  && echo "*** Simulate errors:     $n_esimul"
    exit 1
else
    echo "Success."
fi

# eof
