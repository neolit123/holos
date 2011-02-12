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
#ifndef h_Integer_included
#define h_Integer_included
//----------------------------------------------------------------------

// http://eli.thegreenplace.net/2004/07/30/a-cool-algorithm-for-counting-ones-in-a-bitstring/

unsigned int h_NumBits(unsigned int a_Num)
  {
    unsigned count = 0;
    while (a_Num)
    {
      a_Num &= (a_Num-1);
      ++count;
    }
    return count;
  }

//----------

// http://bits.stephan-brumme.com/isPowerOfTwo.html

bool h_IsPowerOfTwo(unsigned int x)
  {
    return ((x&(x-1))==0);
  }

//----------

// http://bits.stephan-brumme.com/roundUpToNextPowerOfTwo.html

unsigned int h_NextPowerOfTwo(unsigned int x)
  {
    x--;
    x |= x >> 1;  // handle  2 bit numbers
    x |= x >> 2;  // handle  4 bit numbers
    x |= x >> 4;  // handle  8 bit numbers
    x |= x >> 8;  // handle 16 bit numbers
    x |= x >> 16; // handle 32 bit numbers
    x++;
    return x;
  }

//----------------------------------------------------------------------

/**
 * bit reverse algorithm <br>
 * origin: http://graphics.stanford.edu/~seander/bithacks.html
 * \code
 * unsigned int b = 0x0000000a;
 * unsigned int result = h_BitReverse(b); // result = 0xa0000000;
 * \endcode
 * @param[in] x unsigned int
 * @return x unsigned int
 */

//__h_utils_inline
unsigned int h_BitReverse(unsigned int v)
  {
    unsigned int r = v;
    int s = sizeof(v) * CHAR_BIT - 1;
    for (v >>= 1; v; v >>= 1)
    {
      r <<= 1;
      r |= v & 1;
      s--;
    }
    r <<= s;
    return r;
  }

//----------

/**
 * returns a specific bit of an integer
 * \code
 * int x = 0x0f;
 * for(unsigned int i=0; i<32; i++)
 * { cout << h_GetBit(x, i); }      // 11110000000000000000000000000000
 * cout << h_GetBit(x, 0);          // 1
 * cout << h_GetBit(x, 4);          // 0
 * \endcode
 * @param[in] x long int - input value
 * @param[in] bit unsigned int - which bit
 * @return unsigned int - 0x0 / 0x1
 */

#define h_GetBit(x, bit) ( 1 & ((x) >> (bit)) )

//#define LoByte(x) ((unsigned char)((x) & 0xff))
//#define HiByte(x) ((unsigned char)((unsigned short)(x) >> 8))


//----------------------------------------------------------------------
#endif
