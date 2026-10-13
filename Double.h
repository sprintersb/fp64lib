/* Copyright (c) 2019-2025  Uwe Bissinger
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

/*
 * Simple C++ wrapper class to ease use of fp64lib.
 *
 * Usage:
 *    #include <avr/fp64lib/Double.h>
 * instead of
 *    #include <avr/fp64lib/fp64lib.h>
 * Now variables can be declared by "Double x,y,z;" (Notice the uppercase "D"!)
 * and usual operations can be done similar to "normal" double or int variables,
 * like:
 *    Double a = Double("1.234567890123456");
 *    Double b = a * Double(3.14);
 *    b *= Double(2);
 *    b = Double::sin(b);
 * Beware of code like
 *    uint64_t i = 123456789;
 *	  Double b(i);
 * This will *NOT* set b to 123456789 but to a double value that has
 * this value as internal representation (b = 6.099576e-316).
 *
 * Notice that this is only needed when AVG GCC doesn't support 64-bit
 * (long) double, which is the case for GCC 9 and below.  With GCC 10+
 * you can just use <math.h> and double as usual, i.e.
 *    #include <math.h>
 *    double a = 1.23;
 *    double b = a * 3.14;
 *    b = sin (b);
 *    long double la = 1.23L;
 *    long double lb = la * 3.14L;
 *    lb = sinl (lb);
 * In order to get fp64lib as double implementation, link with -lfp64.
 * In AVR GCC 17+ you can instead link with -mfp64lib which inserts -lfp64
 * in the right place for you.
 * In order to set the size of the double type, use -mdouble=32 or -mdouble=64.
 * Similarly, set the size of long double with -mlong-double=64 etc.
 * See https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html#index-mdouble
 *
 * For more see online documentation at https://fp64lib.org or in the provided
 * demo programs for fp64lib.
 */

#ifndef DOUBLE_H
#define DOUBLE_H
#include <avr/fp64lib/fp64lib.h>

class Double {
	public:
		Double()				{ x = 0ULL; }
		Double( float f )		{ x = fp64_sd( f ); }
		Double( double f ) {
#if __SIZEOF_DOUBLE__ == 8
			__builtin_memcpy (&x, &f, 8);
#else
			x = fp64_sd( f );
#endif
		}
		Double( long double f ) {
#if __SIZEOF_LONG_DOUBLE__ == 8
			__builtin_memcpy (&x, &f, 8);
#else
			x = fp64_sd( f );
#endif
		}
		Double( float64_t f ) 	{ x = f; }
		Double( int16_t n )		{ x = fp64_int16_to_float64( n ); }
		Double( uint16_t n )	{ x = fp64_uint16_to_float64( n ); }
		Double( int32_t n )		{ x = fp64_int32_to_float64( n ); }
		Double( uint32_t n )	{ x = fp64_uint32_to_float64( n ); }
		Double( char *s )		{ x = fp64_atof( s ); }
		Double( const Double& d ) { x = d.x; }

		const char* toString() const {
			return fp64_to_string( x, 17, 15);
		}
		const char* toString( int prec ) const {
			return fp64_to_string( x, prec+2, prec);
		}
		float64_t data() const {
			return this->x;
		}

		explicit operator float64_t() const {
			return data();
		}

		explicit operator float() const {
			return fp64_ds (this->x);
		}

		explicit operator double() const {
			double d;
#if __SIZEOF_DOUBLE__ == 8
			__builtin_memcpy (&d, &x, 8);
#else
			d = fp64_ds (this->x);
#endif
			return d;
		}

		explicit operator long double() const {
			long double d;
#if __SIZEOF_LONG_DOUBLE__ == 8
			__builtin_memcpy (&d, &x, 8);
#else
			d = fp64_ds (this->x);
#endif
			return d;
		}

		Double operator+=( const Double& y ) {
			this->x = fp64_add( this->x, y.x );
			return *this ;
		}
		Double operator+( const Double& y ) const {
			Double res;
			res.x = fp64_add( this->x, y.x );
			return res;
		}
		Double operator-=( const Double& y ) {
			this->x = fp64_sub( this->x, y.x );
			return *this;
		}
		Double operator-( const Double& y ) const {
			Double res;
			res.x = fp64_sub( this->x, y.x );
			return res;
		}
		Double operator+() const {
			Double res;
			res.x = this->x;
			return res;
		}
		Double operator-() const {
			Double res;
			res.x = fp64_neg( this->x );
			return res;
		}
		Double operator*=( const Double& y ) {
			this->x = fp64_mul( this->x, y.x );
			return *this;
		}
		Double operator*( const Double& y ) const {
			Double res;
			res.x = fp64_mul( this->x, y.x );
			return res;
		}
		Double operator/=( const Double& y ) {
			this->x = fp64_div( this->x, y.x );
			return *this;
		}
		Double operator/( const Double& y ) const {
			Double res;
			res.x = fp64_div( this->x, y.x );
			return res;
		}
		Double operator%=( const Double& y ) {
			this->x = fp64_fmod( this->x, y.x );
			return *this;
			}
		Double operator%( const Double &y ) const {
			Double res;
			res.x = fp64_fmod( this->x, y.x );
			return res;
		}
		bool operator<( const Double &y ) const {
			return fp64_compare( this->x, y.x ) < 0;
		}
		bool operator>( const Double &y ) const {
			return fp64_compare( this->x, y.x ) > 0;
		}
		bool operator<=( const Double &y ) const {
			return fp64_compare( this->x, y.x ) <= 0;
		}
		bool operator>=( const Double &y ) const {
			return fp64_compare( this->x, y.x ) >= 0;
		}
		bool operator==( const Double &y ) const {
			return fp64_compare( this->x, y.x ) == 0;
		}
		bool operator!=( const Double &y ) const {
			return fp64_compare( this->x, y.x ) != 0;
		}
#define FP64LIB_IMPL(F)							\
		static Double F( const Double &x ) {	\
			Double res;							\
			res.x = fp64_##F( x.x );			\
			return res;							\
		}
		FP64LIB_IMPL (abs)
		FP64LIB_IMPL (acos)
		FP64LIB_IMPL (asin)
		FP64LIB_IMPL (atan)
		FP64LIB_IMPL (atanh)
		FP64LIB_IMPL (cbrt)
		FP64LIB_IMPL (ceil)
		FP64LIB_IMPL (cos)
		FP64LIB_IMPL (cosh)
		FP64LIB_IMPL (cotan)
		FP64LIB_IMPL (exp)
		FP64LIB_IMPL (exp2)
		FP64LIB_IMPL (exp10)
		FP64LIB_IMPL (floor)
		FP64LIB_IMPL (log)
		FP64LIB_IMPL (log2)
		FP64LIB_IMPL (log10)
		FP64LIB_IMPL (round)
		FP64LIB_IMPL (sin)
		FP64LIB_IMPL (sinh)
		FP64LIB_IMPL (tan)
		FP64LIB_IMPL (tanh)
		FP64LIB_IMPL (trunc)
#undef FP64LIB_IMPL

		static Double atan2( const Double &x, const Double &y ) {
			Double res;
			res.x = fp64_atan2( x.x, y.x );
			return res;
		}
		static Double hypot( const Double &x, const Double &y ) {
			Double res;
			res.x = fp64_hypot( x.x, y.x );
			return res;
		}
		static Double pow( const Double &x, const Double &y ) {
			Double res;
			res.x = fp64_pow( x.x, y.x );
			return res;
		}
		static Double e() {
			return Double(float64_EULER_E);
		}
		static Double minus_one() {
			return Double(float64_NUMBER_MINUS_ONE);
		}
		static Double one() {
			return Double(float64_NUMBER_ONE);
		}
		static Double pi() {
			return Double(float64_NUMBER_PI);
		}
		static Double pio2() {
			return Double(float64_NUMBER_PIO2);
		}
		static Double ldexp( const Double &x, int p2 ) {
			Double res;
			res.x = fp64_ldexp( x.x, p2 );
			return res;
		}
		static Double frexp( const Double &x, int *p2 ) {
			Double res;
			res.x = fp64_frexp( x.x, p2 );
			return res;
		}
		static long lround( const Double &x ) {
			return fp64_lround( x.x );
		}
		static long lrint( const Double &x ) {
			return fp64_lrint( x.x );
		}

   private:
      float64_t x;
};
#endif
