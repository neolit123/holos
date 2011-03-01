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
#ifndef h_Utils_included
#define h_Utils_included
//----------------------------------------------------------------------

#ifdef H_UTILS_USE_INLINE
  #define H_UTILS_INLINE inline
#else
  #define H_UTILS_INLINE
#endif

#include "src/h_Types.h"
#include "src/h_Stdlib.h"

// convert to boolean
#define h_Bool(x)         (!(!(x)))
// get bit
#define h_BitGet(x, bit)  ( 1  &  ((x)  >> (bit)))
// set bit
#define h_BitSet(x, bit)  ((x) |  (1    << (bit)))
// clear bit
#define h_BitClr(x, bit)  ((x) & ~(1    << (bit)))
// flip bit
#define h_BitFlp(x, bit)  ((x) ^  (1    << (bit)))

// swap two values
#define h_Swap(x,y) { typeof(x) tmp = (x); (x) = (y); (y) = (tmp); }

// size of array
template<class T, size_t N> T decay_array_to_subtype(T (&a)[N]);
#define h_ArraySize(x) (sizeof(x)/sizeof(decay_array_to_subtype(x)))

// strip path
H_UTILS_INLINE
const char* h_StripPath(const char* a_Path)
  {
    if (a_Path)
    {
      char *slash, *backslash;
      slash     = h_Strrchr(a_Path,'/');
      backslash = h_Strrchr(a_Path,'\\');//+1;
      if (slash) return slash + 1;
      else if (backslash) return backslash + 1; // see above
    }
    return (char*)"NULL";
  }

// bit reverse
H_UTILS_INLINE
unsigned long h_BitReverse(unsigned long v)
  {
    unsigned long r = v;
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

// ptr to (reversed) hex
H_UTILS_INLINE
void h_CreateUniqueName(char* a_Buffer, char* a_Prefix, void* a_Ptr)
  {
    static char h_int2hex[17] = "0123456789abcdef";       // +'\0' = 17 (64 bot = 8*2 hex)
    while (*a_Prefix != '\0') *a_Buffer++ = *a_Prefix++;  // memset + update pointers
    #ifdef _H_64BIT_
      h_int64 iptr = (h_int64)a_Ptr;
    #else
      unsigned long iptr = (unsigned long)a_Ptr;
    #endif
    //todo: h_BitReverse
    while (iptr>0)
    {
      int j = iptr & 0x0f;          // 0..f
      iptr >>= 4;                   // /16 (0..f)
      *a_Buffer++ = h_int2hex[j];   // to char
    }
    *a_Buffer++ = '\0';
  }

// radix
H_UTILS_INLINE
void h_Radix(long *source, long *dest, unsigned long N, int byte)
  {
    unsigned int i;
    long count[256];
    long index[256];
    h_Memset(count, 0, sizeof(count));
    for (i=0; i<N; i++) count[ ((source[i]) >> (byte*8)) & 0xff ]++;
    index[0] = 0;
    for (i=1; i<256; i++) index[i] = index[i-1]+count[i-1];
    for (i=0; i<N; i++) dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
  }

//----------------------------------------------------------------------
#endif
