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
  holos's builtin random number generators
  64 bit integers are used so you might need to disable the 'long long'
  warning in g++: -Wno-long-long
*/

//#include "lib/h_Defines.h"
//#include "lib/h_Cpu.h"
//#include "lib/h_Math.h"
#include "src/h_Defines.h"
#include "src/h_Cpu.h"
#include "src/h_Math.h"

#ifdef _H_RAND_USE_INLINE_
  #define _H_RAND_INLINE_ inline
#else
  #define _H_RAND_INLINE_
#endif

/*
  h_Rand
  default fast prng
*/
class h_Rand
{
  private:
    unsigned int _h_rnd;

  public:
    h_Rand(const unsigned int x)
    { _h_rnd = x; }

    h_Rand()
    {
      h_Cpu c;
      _h_rnd = (unsigned int)c.rdtsc();
    }

    ~h_Rand() {}

    /// set seed
    _H_RAND_INLINE_ void seed (const unsigned int seed)
    {
      _h_rnd = seed;
    }

    /// get random float between 0, 1
    _H_RAND_INLINE_ float rand(void)
    {
      return (float)((_h_rnd *= 16807) & 0x7FFFFFFF) * 4.6566129e-010f;
    }

    /// get random float between 0, x
    _H_RAND_INLINE_ float rand(const float range)
    {
      return rand() * range;
    }

    /// get random float between -1, 1
    _H_RAND_INLINE_ float randSigned(void)
    {
      return (float)(_h_rnd *= 16807) * 4.6566129e-010f;
    }

    /// get random float between -x, x
    _H_RAND_INLINE_ float randSigned(const float range)
    {
      return randSigned() * range;
    }

    /// get random integer number between 0, x
    _H_RAND_INLINE_ unsigned int randInt(const unsigned int range=32767)
    {
      return (unsigned int)(rand() * range);
    }

    /// get random integer from bit range (0, 31)
    _H_RAND_INLINE_ unsigned int randBit(const unsigned int range=16)
    {
      return ((_h_rnd *= 16807) & 0x7FFFFFFF) >> (31 - range);
    }
};


/*
  h_RandAlvo
  better prng based on:
  http://www.number.com.pt/Alvo.html
*/

#define H_RANDSF_MH_  2147483647L
#define H_ALVORND_T   double

class h_RandAlvo
{
  private:
    H_ALVORND_T x, a;
  public:
    h_RandAlvo()
    {
      h_Cpu c;
      x = ((H_ALVORND_T)((unsigned int)c.rdtsc()))*0.000000001f;
      a = x*0.99f;
    }

    ~h_RandAlvo() {}

    _H_RAND_INLINE_ H_ALVORND_T rand(const H_ALVORND_T range = 1.f)
    {
      a += x;
      x = a*x + 0.5f;
      x -= (unsigned int)x;
      a *= 0.5f;
      return x*range;
    }

    _H_RAND_INLINE_ void seed(unsigned int s)
    {
      if (s == 0)
      {
        x = 100.f;
        a = x*0.99f;
        return;
      }
      x = ((H_ALVORND_T)(s *= 16807))*0.000000001f;
      a = x*0.99f;
    }

    _H_RAND_INLINE_ H_ALVORND_T randSigned(const H_ALVORND_T range = 1.f)
    {
      return (2.f*rand() - 1.f)*range;
    }

    _H_RAND_INLINE_ unsigned int randInt(const unsigned int top = H_RANDSF_MH_)
    {
      return (unsigned long) (rand()*top);
    }

    _H_RAND_INLINE_ unsigned int randBit(const unsigned int bits = 16)
    {
      return (unsigned int) (rand()*H_RANDSF_MH_) >> (31 - bits);
    }

};

/*
  ------------------------------------------------------------------------------
  h_RandSinf
  - non-uniform, trigonometric, "negative" distribution
  "mostly for educational purposes"
  ------------------------------------------------------------------------------
*/

#define H_RANDSINF_MH_    3.40282347e+38F

#define _H_RANDSINF \
  if (x >= H_RANDSINF_MH_ ) x = 0.f; \
  y = h_Sin((x+=1)*y);

  /*
  __asm__ \
  ( \
    "fld1;" \
    "faddp;" \
    "fxch;" \
    "fmul %%st(1);" \
    "fsin;" \
    : "=t" (y), "=u" (x) : "0" (x), "1" (y) \
  )
  */

#define _H_RANDSINF_SHIFT \
  (y + 1.f) * 0.5f

class h_RandSinf
{
  private:
    float x, y;

  public:
    h_RandSinf(void)
    {
      h_Cpu c;
      unsigned int _x = (unsigned int)c.rdtsc();
      x = (float)(_x >> 16);
      y = 1.f;
    }

    h_RandSinf(const unsigned int _x)  { x = (float)_x; }
    h_RandSinf(const float _x)         { x = _x;        }

    ~h_RandSinf() {}

    _H_RAND_INLINE_ float rand(void)
    {
      _H_RANDSINF;
      return _H_RANDSINF_SHIFT;
    }

    _H_RAND_INLINE_ float rand(const float _top)
    {
      _H_RANDSINF;
      return _top * _H_RANDSINF_SHIFT;
    }

    _H_RAND_INLINE_ float randSigned(void)
    {
      _H_RANDSINF;
      return y;
    }

    _H_RAND_INLINE_ unsigned int randInt(void)
    {
      _H_RANDSINF;
      return (unsigned int) (H_RANDSF_MH_ * _H_RANDSINF_SHIFT);
    }

    _H_RAND_INLINE_ unsigned int randInt(const unsigned int _top)
    {
      _H_RANDSINF;
      return (unsigned int) ( _top * _H_RANDSINF_SHIFT );
    }
};

#endif // h_Rand_included

