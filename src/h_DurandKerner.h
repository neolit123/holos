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
  durand-kerner (weierstrass) algorithm for finding complex roots
  of polynomials.
  accuracy depends a lot on data type precision.
*/

#ifndef _H_DURAND_KERNER_
#define _H_DURAND_KERNER_

#include "src/h_Types.h"
#include "src/h_Horner.h"
#include "src/h_ComplexNumber.h"

/* settings */
#ifdef _DURAND_KERNER_USE_INLINE_
  #define _DURAND_KERNER_INLINE inline
#else
  #define _DURAND_KERNER_INLINE
#endif

#define dk_t double

#define DK_EPSILON  1E-16
#define DK_MAX_ITR  1E+3
#define DK_MAX_N    256

const cnum_s dk_demoivre_c = {0.4, 0.9};

/* accepts an array of complex numbers */
_DURAND_KERNER_INLINE
void durand_kerner_c
(const cnum_s *coeff, cnum_s *roots, const h_uint16 order)
{
  register h_uint16 i, j;
  register h_uint32 itr;
  cnum_s coeff_sc[DK_MAX_N];
  cnum_s x;
  cnum_s hor;

  i = 0;
  while(i < order)
  {
    cnum_from(&roots[i], cnum_pow(dk_demoivre_c, i));
    i++;
  }

  cnum_from(&coeff_sc[0], cnum_r1);
  i = 1;
  while(i < order+1)
  {
    cnum_from(&coeff_sc[i], cnum_div(coeff[i], coeff[0]));
    i++;
  }

  itr = 0;
  while(itr < DK_MAX_ITR)
  {
    i = 0;
    while(i < order)
    {
      j = 0;
      x = cnum_r1;
      while (j < order)
      {
        if (i != j)
          x = cnum_mul(cnum_sub(roots[i], roots[j]), x);
        j++;
      }
      hor = horner_eval_c(coeff_sc, roots[i], order);
      x = cnum_div(hor, x);
      x = cnum_sub(roots[i], x);
      if (h_Abs(h_Abs(x.r) - h_Abs(roots[i].r)) < DK_EPSILON &&
          h_Abs(h_Abs(x.i) - h_Abs(roots[i].i)) < DK_EPSILON)
        return;
      cnum_from(&roots[i], x);
      i++;
    }
    itr++;
  }
}

/* accepts an array of real numbers */
_DURAND_KERNER_INLINE
void durand_kerner
(const dk_t *coeff, cnum_s *roots, const h_uint16 order)
{
  register h_uint16 i;
  cnum_s coeff_c[DK_MAX_N];
  i = 0;
  while(i < (order+1))
  {
    cnum_set(&coeff_c[i], coeff[i], 0);
    i++;
  }
  durand_kerner_c(coeff_c, roots, order);
}

#endif /* _H_DURAND_KERNER_ */
