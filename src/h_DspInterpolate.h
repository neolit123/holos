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
#ifndef h_DspInterpolate_included
#define h_DspInterpolate_included
//----------------------------------------------------------------------

/**
 * @file h_DspInterpolate.h
 * \brief interpolation methods
 */

#include "src/h_DspRBJ.h"

// todo: more methods:: http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/interpolation/
// todo: other filter options once avaiable
//
// q: should this be useable more globally? it is math related and not strictly DSP.
// perhaps move all interpolation methods in axInterpolate class and have dspInterpolate
// to call axInterpolate::dosomething(..input, options..)

/**
 * \brief interpolation methods
 *
 * can be used to interpolate between sample values and apply antialising
 * filter <br>
 * <br>
 * usage:
 * \code
 * // include dspInterpolate.h
 * // # new instance
 * dspInterpolate i0;
 *
 * // # setup stage
 * bool aafilter = true;
 * int os_factor = 4;
 * float srate = updateSampleRate();
 * i0.setup(os_factor, srate, aafilter);
 *
 * // create a static function for callback
 * static float callback_f(void* _this, float sample) {...}
 *
 * // # process stage
 * // pass an instance of the current class, the 'input' value and 'callback_f'
 * float result = i0.process(this, callback_f, input);
 * \endcode
 * <br>
 * example in:
 * /plugins/liteon/_os.cpp
 */

class h_DspInterpolate
{
  private:
    float factor, invf;
    bool filter_enabled;
    h_DspRBJ f0;

  public:

    /**
     * setup parameters
     * @param[in] _factor unsigned int factor (1x....Nx)
     * @param[in] _srate unsigned int sample rate (hz)
     * @param[in] _filter_enabled bool enable filter (true / false)
     * @return void
     */

    void setup(const unsigned int _factor=4, const float _srate=44100,
               const bool _filter_enabled=true)
      {
        factor = _factor;
        invf = 1.f/factor;
        //float invf = 1.f/factor; // i guess this one is wrong?
        if (factor < 1) factor = 1;
        filter_enabled = _filter_enabled;
        if (_filter_enabled)
        {
          // setup filter here:
          // filter.setup ( lp, srate, freq, q, gain, param interpolation )
          f0.setup(1, _factor*_srate,_factor*_srate*0.499f, 0.5f, 1.f, false);
        }
      }

    /**
     * process input <br>
     * *cb_function must point to a static function.
     * @param[in] parent void* - instance of parent class
     * @param[in] *cb_function float - pointer to function (callback)
     * @param[in] in float - input value
     * @return return float - output value
     */

    inline
    float process(void* parent, float (*cb_function)(void*, float), const float in)
      {
        float y, t_y, x = 0.f;
        float t_in = in, out;
        // --
        for (unsigned int i=0; i<factor; i++)
        {
          // linear interp
          y = t_in + x*(in - t_in);
          // process (call back to function)
          y = (*cb_function)(parent, y);
          // filter
          t_y = y;
          if (filter_enabled) t_y = f0.process(y)*0.9f;
          // discard samples
          if (x == 0.f) out = t_y;
          x += invf;
        }
        return out;
      }
};

//----------------------------------------------------------------------
#endif
