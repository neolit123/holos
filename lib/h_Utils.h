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

#ifdef H_HOT_INLINE_UTILS
  #define __h_utils_inline __hotinline
#else
  #define __h_utils_inline inline
#endif

#include "lib/h_Stdlib.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define h_StrExpand(x) #x
//#define h_Str(x) h_StrExpand(x)

//----------

/**
 * swap the values of two variables <br>
 \code
 * origin: http://graphics.stanford.edu/~seander/bithacks.html
 * note: type of first variable is used
 * float x = 5.f;
 * float y = 3.f;
 * h_Swap(x, y);
 * \endcode
 * @param[in] x type-unsafe
 * @param[in] y type-unsafe
 * @return void
 */

#define h_Swap(x,y) { typeof(x) tmp = (x); (x) = (y); (y) = (tmp); }

//----------

/**
 * h_GetArrSize() helper function
 */

template<class T, size_t N> T decay_array_to_subtype(T (&a)[N]);

/**
 * returns the size of an array <br>
 * \code
 * char a[15];
 * int b[h_GetArrSize(a) + 1];
 * unsigned int j = h_GetArrSize(b);    // j = 16
 * // ----------------
 * // NOTE: passing a pointer will not work
 * int ar[21];
 * int* ptr = ar;
 * unsigned int j = h_GetArrSize(ptr); // <- will not work
 * \endcode
 * @param[in] x array type-unsafe
 * @return unsigned int
 */

#define h_GetArrSize(x) (sizeof(x)/sizeof(decay_array_to_subtype(x)))

//----------------------------------------------------------------------
//
//
////----------------------------------------------------------------------

// h_Strrchr

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

//----------

// ptr to (reversed) hex
void h_CreateUniqueName(char* a_Buffer, char* a_Prefix, void* a_Ptr)
  {
    static char h_int2hex[17] = "0123456789abcdef";       // +'\0' = 17
    while (*a_Prefix != '\0') *a_Buffer++ = *a_Prefix++;  // memset + update pointers
    unsigned long iptr = (unsigned long)a_Ptr;            // todo: 64-bit (long long?)
    while (iptr>0)
    {
      int j = iptr & 0x0f;          // 0..f
      iptr >>= 4;                   // /16 (0..f)
      *a_Buffer++ = h_int2hex[j];   // to char
    }
    *a_Buffer++ = '\0';
  }

//----------

/**
 * radix algorithm
 * @param[in] source long*
 * @param[in] dest long*
 * @param[in] N long
 * @param[in] byte int
 */

__h_utils_inline
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
