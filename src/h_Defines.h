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

#ifndef h_Defines_included
#define h_Defines_included
//----------------------------------------------------------------------

// version
// -----------------------------------------------------------------------------
#define H_MAJOR_VERSION   0
#define H_MINOR_VERSION   0
#define H_SUB_VERSION     1

// version to string
// -----------------------------------------------------------------------------
#define h_StrExpand(x) #x
#define h_Str(x) h_StrExpand(x)

#define H_VERSION_STRING  \
  h_Str(H_MAJOR_VERSION)"." \
  h_Str(H_MINOR_VERSION)"." \
  h_Str(H_SUB_VERSION)

#define H_HOLOS_TEXT \
  "powered by Holos v" H_VERSION_STRING " (http://holos.googlecode.com/)"

// general
// -----------------------------------------------------------------------------
#define H_MAX_PATHSIZE    512
#define H_MAX_STRINGSIZE  256

//'H_' + 0x0000
#define H_MAGIC 0x485f0000

//#define SPL float
//#define STR h_String
//#define REC h_Rect
//#define COL h_Color;


// platform
// -----------------------------------------------------------------------------

#ifdef linux
  #define H_LINUX
  #ifdef H_DEBUG_CON
    #undef H_DEBUG_CON
  #endif
#endif

#ifdef WIN32
  #define H_WIN32
  #define WINVER 0x0501
  #undef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
#endif

// format
// -----------------------------------------------------------------------------

#ifdef H_EXE
#endif

#ifdef H_LADSPA
  #define H_LIB
  #ifndef H_NOGUI
    #define H_NOGUI
  #endif
#endif

#ifdef H_VST
  #define H_LIB
#endif

// architechture
// -----------------------------------------------------------------------------

#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define _H_32BIT_
#endif

#if defined (__x86_64) || (__LP64__)
  #define _H_64BIT_
#endif

// standard
// -----------------------------------------------------------------------------

// deprecate..
//#ifndef   NULL
//  #define NULL 0
//#endif

#define H_NULL 0

#ifndef   CHAR_BIT
  #define CHAR_BIT 8
#endif
#ifndef   TRUE
  #define TRUE 1
#endif
#ifndef   true
  #define true 1
#endif
#ifndef   FALSE
  #define FALSE 0
#endif
#ifndef   false
  #define false 0
#endif

// dllexport, dllimport, cdecl, stdcall
// -----------------------------------------------------------------------------

#undef  __dllexport
#undef  __dllimport
#undef  __stdcall
#undef  __cdecl

#ifdef H_WIN32
  #define __dllexport __attribute__ ((dllexport))
  #define __dllimport __attribute__ ((dllimport))
  #define __cdecl     __attribute__ ((cdecl))
  #define __stdcall   __attribute__ ((stdcall))
#endif

#ifdef H_LINUX
  #define __dllexport
  #define __dllimport
  #define __cdecl
  #define __stdcall
#endif

// attributes
// -----------------------------------------------------------------------------

#define __aligned(x)              __attribute__ ((aligned (x)))
#define __alignedmax              __attribute__ ((aligned))
#define __packed                  __attribute__ ((packed))
#define ___shared                 __attribute__ ((shared))
#define __section(name)           __attribute__ ((section (name)))
#define __used                    __attribute__ ((used))
#define __unused                  __attribute__ ((unused))
#define __vector(x)               __attribute__ ((vector_size (x)))
#define __selectany               __attribute__ ((selectany))
#define __transparent_union       __attribute__ ((transparent_union))
#define __may_alias               __attribute__ ((may_alias))
#define __deprecated              __attribute__ ((deprecated))

#define __constructor             __attribute__ ((constructor))
#define __destructor              __attribute__ ((destructor))
#define __alloc_size1(x)          __attribute__ ((alloc_size(x)))
#define __alloc_size2(x, y)       __attribute__ ((alloc_size(x, y)))
#define __alias(name)             __attribute__ ((alias (name)))
#define ___always_inline   inline __attribute__ ((always_inline))
#define __malloc                  __attribute__ ((malloc))
#define __noinline                __attribute__ ((noinline))
#define __noreturn                __attribute__ ((noreturn))
#define __pure                    __attribute__ ((pure))
#define __target(value)           __attribute__ ((target (value)))
#define __weak                    __attribute__ ((weak))
#define __weakref(name)           __attribute__ ((weakref (name)))

#define __externc     extern "C"
#define __may_alias   __attribute__ ((may_alias))
#define __hotinline   ___always_inline __hot __optimize(3)
#define __asmv        __asm__ __volatile__
#define __vlt         __volatile__

// newer version of GCC required
// -----------------------------------------------------------------------------

#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  #define __optimize(level)         __attribute__ ((optimize (level)))
  #define __hot                     __attribute__ ((hot))
  #define __cold                    __attribute__ ((cold))
  #define __error(message)          __attribute__ ((error(message)))
  #define __warning(message)        __attribute__ ((warning(message)))
#else
  #define __optimize(level)
  #define __hot
  #define __cold
  #define __error
  #define __warning
  // disable TLS & show warning
  #undef __thread
  #define __thread
  #warning "### Thread-local storage requires GCC 4.4.x"
#endif

//----------------------------------------------------------------------
#endif
