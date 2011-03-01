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

/*
  algorithm to evaluate integer order polynomials using horner's scheme.
*/

#ifndef _H_HORNER_included
#define _H_HORNER_included

#include "src/h_Types.h"
#include "src/h_ComplexNumber.h"

/* settings */
#ifndef _HORNER_USE_INLINE
  #define _HORNER_INLINE inline
#else
  #define _HORNER_INLINE
#endif

#define horner_t double

/* real */
_HORNER_INLINE
horner_t horner_eval
(const horner_t *coeff, const horner_t x, h_uint16 order)
{
  register horner_t y = coeff[0];
  register h_uint16 n = 1;
  order += 1;
  while(n < order)
  {
    y = y*x + coeff[n];
    n++;
  }
  return y;
}

/* complex */
_HORNER_INLINE
cnum_s horner_eval_c
(const cnum_s *coeff, const cnum_s x, h_uint16 order)
{
  register h_uint16 n = 1;
  cnum_s y = coeff[0];
  order += 1;
  while(n < order)
  {
    y = cnum_add(cnum_mul(y, x), coeff[n]);
    n++;
  }
  return y;
}

#endif /* _H_HORNER_included */
