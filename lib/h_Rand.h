/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_Rand_included
#define h_Rand_included
//------------------------------------------------------------------------------

/*
  axonlib's builtin random number generators
  64 bit integers are used so you might need to disable the 'long long'
  warning in g++: -Wno-long-long

  TODO: improve version of h_Rand() algorithm when H_USE_BETTER_RAND is used

*/

#include "h_Defines.h"

#ifdef H_HOT_INLINE_RAND
  #define __h_rand_inline __hotinline
#else
  #define __h_rand_inline inline
#endif

#ifndef H_NO_RAND      // if not defined: use axonlib's h_Rand() methods

#ifndef H_USE_BETTER_RAND // if not defined: use simpler h_Rand()

static __thread int _h_rnd = 1;

/// set seed

__h_rand_inline
void h_RandSeed (const unsigned int seed)
  {
    _h_rnd = seed;
  }

/// get random float between 0, 1

__h_rand_inline
float h_Rand(void)
  {
    return (float)((_h_rnd *= 16807) & 0x7FFFFFFF) * 4.6566129e-010f;
  }

/// get random float between 0, x

__h_rand_inline
float h_Rand(const float range)
  {
    return h_Rand() * range;
  }

/// get random float between -1, 1

__h_rand_inline
float h_RandSigned(void)
  {
    return (float)(_h_rnd *= 16807) * 4.6566129e-010f;
  }

/// get random float between -x, x

__h_rand_inline
float h_RandSigned(const float range)
  {
    return h_RandSigned() * range;
  }

/// get random integer number between 0, x

__h_rand_inline
unsigned int h_RandInt(const unsigned int range=32767)
  {
    return (unsigned int)(h_Rand() * range);
  }

/// get random integer from bit range (0, 31)
__h_rand_inline
unsigned int h_RandBit(const unsigned int range=16)
  {
    return ((_h_rnd *= 16807) & 0x7FFFFFFF) >> (31 - range);
  }

#else // H_USE_BETTER_RAND

/*
  ------------------------------------------------------------------------------
  h_Rand()
  - "rdtsc" 64 bit register dependant
  - seeded from "rdtsc"
  - uniform distribution
  - returns integer or floats in specified ranges
  author: lubomir i. ivanov (lii) for axonlib
  ------------------------------------------------------------------------------
*/

#define H_RAND_MAX        2147483647L
//#define INV_H_RAND_MAX    4.656612875245796924105750827168e-10f
#define H_INV_RAND_MAX    4.656612875245796924105750827168e-10f
#define H_RAND_MAXLL      9223372036854775806LL
#define _H_RAND_FLOAT     ( ( (float)( ( a * b ) >> 2) ) * 0.00000000093f )
#define _H_RAND_FLOAT_C   s * ( (float)( ( a * b ) >> 1) ) * H_INV_RAND_MAX

#ifdef _H_64BIT_
  // 64bit untested
  __thread unsigned int _h_rnd;
  #define _H_RAND_ARCH                                                    \
    register unsigned long low, high;                                     \
    register unsigned int  a, b;                                          \
    if (!_h_rnd)                                                          \
    {                                                                     \
      __asm__ ( "rdtsc;" : "=a" (low), "=d" (high) )                      \
      _h_rnd = (unsigned int)( (low) | ( (unsigned long)(high) << 32 ) )  \
    }                                                                     \
    else if (_h_rnd > H_RAND_MAXLL) _h_rnd = 0;                           \
    a = _h_rnd++
#endif

#ifdef _H_32BIT_
  __thread unsigned long long _h_rnd;
  #define _H_RAND_ARCH                          \
    register unsigned long a, b;                \
    if (!_h_rnd)                                \
      __asm__ ( "rdtsc;" : "=A" (_h_rnd) );     \
    else if (_h_rnd > H_RAND_MAXLL) _h_rnd = 0; \
    a = (unsigned long) _h_rnd++
#endif

#define _H_RAND                                   \
  _H_RAND_ARCH;                                   \
  b = a;                                          \
  a = ( (a * -29999531) & -30000469 ) * 20000469; \
  b = ( (b * -20000085) & -19999915 ) * 19999915;

__h_rand_inline
float h_Rand(void)
  {
    _H_RAND;
    return _H_RAND_FLOAT;
  }

__h_rand_inline
float h_Rand(const float s)
  {
    _H_RAND;
    return _H_RAND_FLOAT_C;
  }

__h_rand_inline
float h_RandSigned(void)
  {
    _H_RAND;
    return ( 2.f * _H_RAND_FLOAT - 1.f );
  }

__h_rand_inline
unsigned long h_RandInt(void)
  {
    _H_RAND;
    return ( a * b );
  }

__h_rand_inline
unsigned long h_RandInt(const unsigned long s)
  {
    _H_RAND;
    return (unsigned long)( _H_RAND_FLOAT_C );
  }

#endif  // H_USE_BETTER_RAND

#ifdef H_USE_RANDSINF

/*
  ------------------------------------------------------------------------------
  h_RandSinf()
  - non-uniform, trigonometric, "negative" distribution
    (more values towards min, max)
  - uses class constructor
  - recursive
  - seeded with "rdtsc" on initialization
  - thread unsafe
  - returns integer or floats in specified ranges
  author: lubomir i. ivanov (lii) for axonlib
  ------------------------------------------------------------------------------
*/

#define H_RANDSINF_MAX    3.40282347e+38F
#define H_RANDSF_MAX      2147483647L

#define _H_RANDSINF                         \
  if (x >= H_RANDSINF_MAX ) x = 0.f;        \
  __asm__                                   \
  (                                         \
    "fld1;"                                 \
    "faddp;"                                \
    "fxch;"                                 \
    "fmul %%st(1);"                         \
    "fsin;"                                 \
    : "=t" (y), "=u" (x) : "0" (x), "1" (y) \
  )

#define _H_RANDSINF_SHIFT \
  (y + 1.f) * 0.5f

class h_RandSinf
{
  private:
    float x, y;

  public:

    h_RandSinf(void)
      {
        #ifdef _H_64BIT_
        // 64bit untested
          register unsigned long low, high;
          register unsigned int _x;
          __asmv ( "rdtsc;" : "=a" (low), "=d" (high) );
          _x = (unsigned int) ( (low) | ( (unsigned long)(high) << 32 ) );
        #endif
        #ifdef _H_32BIT_
          register unsigned long long _x;
          __asmv ( "rdtsc;" : "=A" (_x) );
        #endif
        x = (float)(_x >> 16);
        y = 1.f;
      }

    h_RandSinf(const unsigned long _x)
      {
        x = (float)_x;
      }

    h_RandSinf(const float _x)
      {
        x = _x;
      }

    ~h_RandSinf()
      {
      }

    __h_rand_inline
    float rand(void)
      {
        _H_RANDSINF;
        return _H_RANDSINF_SHIFT;
      }

    __h_rand_inline
    float rand(const float _top)
      {
        _H_RANDSINF;
        return _top * _H_RANDSINF_SHIFT;
      }

    __h_rand_inline
    float randSigned(void)
      {
        _H_RANDSINF;
        return y;
      }

    __h_rand_inline
    unsigned long randInt(void)
      {
        _H_RANDSINF;
        return (unsigned long) (H_RANDSF_MAX * _H_RANDSINF_SHIFT);
      }

    __h_rand_inline
    unsigned long randInt(const unsigned long _top)
      {
        _H_RANDSINF;
        return (unsigned long) ( _top * _H_RANDSINF_SHIFT );
      }

};

#endif // H_USE_RANDSINF

#else // H_NO_RAND

/*
  ------------------------------------------------------------------------------
  stdlib based methods rand()
  ------------------------------------------------------------------------------
*/

#include "math.h"
#include "time.h"
#include "stdarg.h"
#include "stdlib.h"

__deprecated
___h_rand_inline
void h_Randomize(const int aSeed = (unsigned int)time(0))
  {
    srand(aSeed);
  }

__deprecated
__h_rand_inline
float h_Rand(const float f = 1)
  {
    return (f * (float)rand() / (float)RAND_MAX);
  }

__deprecated
__h_rand_inline
float h_Rand(const float aLow, const float aHigh)
  {
    return aLow + ( (float)rand() / (float)RAND_MAX ) *(aHigh - aLow);
  }

__deprecated
__h_math_inline
int h_RandInt(const int i=RAND_MAX)
  {
    return h_MinInt(i, (int)h_Floor( (i + 1) * (float)rand() / (float)RAND_MAX ) );
  }

__deprecated
__h_rand_inline
float h_RandSigned(void)
  {
    return (2 * (float)rand() / (float)RAND_MAX) - 1;
  }

#endif // H_NO_RAND

#endif // h_Rand_included

