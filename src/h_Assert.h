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

/*
  ISO C/C++ compatible runtime and static assertion for GCC (4.4.x)
  no external dependencies.
*/

#ifndef h_Assert_included
#define h_Assert_included
//----------------------------------------------------------------------

/* for c89 compat mainly if the header is used elsewhere */
#ifdef __GNUC_STDC_INLINE__
  #define _H_SA_INLINE inline
#else
  #define _H_SA_INLINE
#endif

#include <stdlib.h>

#ifdef H_DEBUG /* H_DEBUG */

  /*
    h_Assert()
    runtime assertion
  */
  unsigned int _h_Assert () __attribute__ (( noinline ));
  unsigned int _h_Assert
  (
    const unsigned int e, const char* file,
    const char* func, const unsigned int line,
    const char* e_str
  )
  {
    if (!e)
    {
      dtrace("### H_ASSERT: " << file << ", " << func << ", " << line
        << ", (" << e_str << ")");
      exit(1);
    }
    return 0;
  }
  #define h_Assert(e) _h_Assert((e), __FILE__, __FUNCTION__, __LINE__, #e)

  /*
    h_StaticAssert()
    compile time assertion ( requires GCC 4.4.x )
  */
  #if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
    _H_SA_INLINE unsigned int __H_SA_TRUE (void) { return 1; }
    unsigned int _H_SA (void)
      __attribute__ (( error("### STATIC ASSERTION at this point!") ))
      __attribute__ (( optimize(0) ));
    #define h_StaticAssert(e) ( (!(e)) ? _H_SA(): __H_SA_TRUE() )
  #else
    /*
      switch can also be used. it does not generate extra code and is much
      more portable, but not very useful at the same time.
    */
    #define h_StaticAssert(e) switch(0) { case 0: case (e): ; }
  #endif

#else /* H_DEBUG */

  _H_SA_INLINE unsigned int __H_SA (void) { return 0; }
  #define h_Assert(e)       __H_SA()
  #define h_StaticAssert(e) __H_SA()

#endif /* H_DEBUG */

//----------------------------------------------------------------------
#endif /* h_Assert_included */
