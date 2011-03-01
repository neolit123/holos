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
#ifndef h_DspOnePole_included
#define h_DspOnePole_included
//----------------------------------------------------------------------

/**
 * \file h_DspOnePole.h
 * \brief one pole filters
 */

#include "src/h_Utils.h"

/**
 * \brief one pole filters
 *
 * leeky integrator with various routings to get shelving and high pass <br>
 * usage:
 * \code
 *  // include dspOnePole.h
 *  // create instance
 *  dspOnePole f0;
 *  // setup
 *  f0.setup(1, 44100.f, 11000.f, 1.2f, true);
 *  // process
 *  f0.process(input);
 * \endcode
 */

class h_DspOnePole
{
  private:
    float a0, b1, y, _gain, _gain2;
    unsigned int ftype;
    bool hasintrp;
    float i_a0, i_b1;
    float d_a0, d_b1;
    float s_a0, s_b1;

    /// reset parameters (private)

    virtual void reset_p(void)
      {
        i_a0 = i_b1 = d_a0 = d_b1 = s_a0 = s_b1 = 0.f;
      }

  public:

    /**
     * constructor
     */

    h_DspOnePole()
      {
        reset_p();
        hasintrp = false;
        ftype = 0;
        setup();
      }

    /**
     * setup filter
     *
     * \code
     *  // filter types:
     *  // 0 - no filter
     *  // 1 - low pass
     *  // 2 - high pass
     *  // 3 - low shelf
     *  // 4 - high shelf
     *  \endcode
     * @param[in] type unsigned int - filter type (0 - 4)
     * @param[in] srate float - sample rate (Hz)
     * @param[in] freq float - frequency (Hz)
     * @param[in] gain float - gain (dB)
     * @param[in] intrp bool - parameter interpolation (on / off)
     */

    void setup(const unsigned int type=0, const float srate=44100,
               const float freq=11000.f, const float gain=1.f, const bool intrp=false)
      {
        //gain
        _gain = h_DB2Lin(gain);
        _gain2 = _gain - 1;
        // reset params if filter has changed
        if (type != ftype) reset_p();
        // if type out of range set to 0
        ftype = type;
        if (ftype < 0 || ftype > 4)
        {
          ftype = 0;
        }
        // coeff
        b1 = h_ExpF(-PI2*freq / srate);
        a0 = 1.f - b1;
        // has interpolation
        hasintrp = true;
        if (!intrp)
        {
          i_a0 = a0;
          i_b1 = b1;
          hasintrp = false;
        }
      }

     /*
     * interpolate filter coeff <br>
     * <br>
     * \code
     * // call this from h_Format::doProcessBlock(..)
     * // or each N samples: myfilter.interpolate(N)
     * \endcode
     * \param sampleFrames long
     * \return void
     */

    virtual void interpolate(long sampleFrames)
      {
        if (hasintrp && ftype != 0)
        {
          float inv_sb = 1/sampleFrames;
          // ---
          d_a0 = (a0 - s_a0)*inv_sb;
          i_a0 = s_a0;
          s_a0 = a0;
          // ---
          d_b1 = (b1 - s_b1)*inv_sb;
          i_b1 = s_b1;
          s_b1 = b1;
        }
      }

    /**
     * process input sample with filter
     * @param[in] in float
     * @return float
     */

    virtual float process(float in)
      {
        if (hasintrp)
        {
          i_a0 += d_a0;
          i_b1 += d_b1;
        }
        switch (ftype)
        {
          // no filter
          case 0:
            return in*_gain;
            break;
          // lp
          case 1:
            y = i_a0*in + i_b1*y + DENORM;
            return (y - DENORM)*_gain;
            break;
          // hp
          case 2:
            y = i_a0*in + i_b1*y + DENORM;
            return (in - y - DENORM)*_gain;
            break;
          // ls
          case 3:
            y = i_a0*in + i_b1*y + DENORM;
            return in + (y - DENORM)*_gain2;
            break;
          // hs
          case 4:
            y = i_a0*in + i_b1*y + DENORM;
            return in + (in - y - DENORM)*_gain2;
            break;
        }
      }

};

//----------------------------------------------------------------------
#endif
