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
#ifndef h_DspDCF_included
#define h_DspDCF_included
//----------------------------------------------------------------------

/**
 * \file h_DspDCF.h
 * \brief DC offset filter.
 */

#include "src/h_Math.h" // DENORM

/**
 * \brief DC offset filter.
 *
 * one pole, one zero DC offset filter from julius orion smith (jos). <br>
 * http://www-ccrma.stanford.edu/~jos/filters/ <br>
 * usage:
 * \code
 * // include "src/h_DspDCF.h"
 * // create instance
 * h_DspDCF dcfilter;
 * // setup
 * float dcout = dcfilter.process(input); // process input sample
 * \endcode
 *
 */

class h_DspDCF
{
  private:
    float x1, y1;
    float R;

  public:

    /**
     * constructor
     */

    h_DspDCF()
      {
        x1 = y1 = 0;
        setup(0.999f);
      }

    /**
     * sets filter coeff. suggested range [0.99 - 1]
     * @param[in] inR float
     */

    void setup(const float inR)
      {
        R = inR;
      }

    /**
     * process input sample with filter
     * @param[in] in float
     * @return result float
     */

    virtual float process(const float in)
      {
        const float y = in - x1 + R*y1 + DENORM;
        x1 = in;
        y1 = y;
        return y - DENORM;
      }

};

//----------------------------------------------------------------------
#endif
