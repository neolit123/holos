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
#ifndef h_Types_included
#define h_Types_included
//----------------------------------------------------------------------

#include "src/h_Defines.h"
#include <stdint.h>

/*
// ### should be better plat. dependent (or deleted)

// limits
//----------------------------------------------------------------------

#define H_CHAR_BIT       8
#define H_CHAR_MAX       127
#define H_SHRT_MAX       32767
#define H_WCHAR_MAX      65535U
#define H_INT_MAX        2147483647
#define H_UINT_MAX       8589934591U
#define H_LONG_LONG_MAX  9223372036854775807LL
#define H_ULONG_LONG_MAX 18446744073709551615ULL

#define H_FLT_MAX        3.40282347e+38F
#define H_FLT_MIN        1.17549435E-38F
#define H_FLT_EPSILON    1.19209290E-7F
#define H_FLT_DENORM     1.40129846E-45F
#define H_DBL_MAX        1.7976931348623157e+308
#define H_DBL_MIN        2.2250738585072014E-308
#define H_DBL_EPSILON    2.2204460492503131E-16
#define H_DBL_DENORM     4.9406564584124654E-324
#define H_LDBL_MAX       1.18973149535723176502e+4932L
#define H_LDBL_MIN       3.36210314311209350626E-4932L
#define H_LDBL_EPSILON   1.08420217248550443401E-19L
#define H_LDBL_DENORM    3.64519953188247460253E-4951L
*/

// plat. dependent
//----------------------------------------------------------------------

#define h_int8_t          int8_t
#define h_uint8_t         uint8_t
#define h_int16_t         int16_t
#define h_uint16_t        uint16_t
#define h_int32_t         int32_t
#define h_unt32_t         uint32_t
#define h_int64_t         int64_t
#define h_uint64_t        uint64_t

#define h_uintptr_t       uintptr_t
#define h_intptr          intptr_t
#define h_size_t          size_t
#define h_ssize_t         ssize_t

// aligned types
//----------------------------------------------------------------------

#define __aligned_t(x)    __attribute__ ((aligned (x)))

typedef double      __aligned_t(8) double_8;
typedef long long   __aligned_t(8) longlong_8;

// aliasing types
//----------------------------------------------------------------------

#define __may_alias_t     __attribute__ ((may_alias))

typedef void*                 __may_alias_t void_ptr_a;
typedef long double           __may_alias_t ldouble_a;
typedef double                __may_alias_t double_a;
typedef float                 __may_alias_t float_a;
typedef long long             __may_alias_t longlong_a;
typedef unsigned long long    __may_alias_t ulonglong_a;
typedef long                  __may_alias_t long_a;
typedef unsigned long         __may_alias_t ulong_a;
typedef int                   __may_alias_t int_a;
typedef unsigned int          __may_alias_t uint_a;
typedef short                 __may_alias_t short_a;
typedef unsigned short        __may_alias_t ushort_a;
typedef char                  __may_alias_t char_a;
typedef unsigned char         __may_alias_t uchar_a;

//----------------------------------------------------------------------
#endif
