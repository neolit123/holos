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
#ifndef h_DspRBJ_included
#define h_DspRBJ_included
//----------------------------------------------------------------------

// biquad filters based on rbj's filter cookbook
//
// original filters by:
// Robert Bristow-Johnson, rbj at audioimagination.com

class h_DspRBJ
{
  private:
    float x1, x2, y1, y2;
    float a0, a1, a2, b0, b1, b2;
    float i_a0, i_a1, i_a2, i_b0, i_b1, i_b2;
    float d_a0, d_a1, d_a2, d_b0, d_b1, d_b2;
    float s_a0, s_a1, s_a2, s_b0, s_b1, s_b2;
    bool hasintrp;
    unsigned int seltype;

    enum
    {
      NOF,    // 0 - no filter
      LPF,    // 1 - low pass filter
      HPF,    // 2 - gigh pass filter
      BPF,    // 3 - band pass filter
      NOTCH,  // 4 - notch filter
      PEQ,    // 5 - peaking band
      LSH,    // 6 - low shelf filter
      HSH     // 7 - high shelf filter
    };

    /// reset parameters (private)

    virtual void reset_p(void)
      {
        x1 = x2 = y1 = y2 = i_a0 = i_a1 = i_a2 = i_b0 = i_b1 = i_b2 =
        d_a0 = d_a1 = d_a2 = d_b0 = d_b1 = d_b2 =
        s_a0 = s_a1 = s_a2 = s_b0 = s_b1 = s_b2 = 0.f;
      }

  public:

    /**
     * constructor
     * <br>
     * usage:
     * \code
     * // include dspRBJ.h
     * dspRBJ myfilter;
     * myfilter.setup(1, updateSampleRate(), 1000, 2.5);
     * // ...
     * myfilter.process(inpusample);
     * \endcode
     */

    h_DspRBJ()
      {
        reset_p();
        setup();
      }

    /**
     * setups filter parameters
     * <br><br>
     * filter types:
     * \code
     * // 0 - no filter
     * // 1 - low pass filter
     * // 2 - high pass filter
     * // 3 - band pass filter
     * // 4 - notch filter
     * // 5 - peaking band
     * // 6 - low shelf filter
     * // 7 - high shelf filter
     * \endcode
     * @param[in] type unsigned int - filter type. see list above.
     * @param[in] srate float - sample rate
     * @param[in] freq float - frequency (Hz)
     * @param[in] Q float - q factor (default = 0.5f)
     * @param[in] gain float - gain (dB)
     * @param[in] intrp bool - enable parameter interpolation (true / false)
     * @return void
     */

    void setup(unsigned int type=0, const float srate=44100, const float freq=639.f,
               float Q=0.5f, const float gain=0.f, const bool intrp=false)
      {
        // tmp vars
        float A, omega, sn, cs, alpha, beta, temp1, temp2, temp3, temp4;
        // prewarp
        Q = h_Max(Q, 0.01f);
        omega = PI2 * freq / srate;
        sn = h_SinF(omega);
        cs = h_CosF(omega);
        alpha = sn / ( 2.0f * Q );
        if (type > 7) type = 0;
        // reset params
        if (seltype != type) reset_p();
        seltype = type;
        // check if filter type out of range
        if (/*seltype < 0 ||*/ seltype > 7) // seltype = unsigned
        {
          seltype = 0;
        }
        // calculate coeff
        switch (type)
        {
          case LPF:
            A = h_PowF( 10.f, gain * 0.05f ); // 20
            b0 = 1.0f  / ( 1.0f  + alpha );
            b1 = ( -2.0f  * cs ) * b0;
            b2 = ( 1.0f  - alpha ) * b0;
            a1 = ( 1.0f  - cs ) * b0 * A;   // * A -> adds gain
            a0 = a1 * 0.5;
            a2 = a0;
            break;
          case HPF:
            A = h_PowF( 10.f, gain * 0.05f );
            b0 = 1.0f  / ( 1.0f  + alpha );
            b1 = ( -2.0f  * cs ) * b0;
            b2 = ( 1.0f  - alpha ) * b0;
            a1 = -( 1.0f  + cs ) * b0 * A;
            a0 = -a1 * 0.5;
            a2 = a0;
            break;
          case BPF:
            A = h_PowF( 10.f, gain * 0.05f );
            alpha = sn / ( 2.0f  * Q );
            b0 = 1.0f  / ( 1.0f  + alpha );
            b1 = ( -2.0f  * cs ) * b0;
            b2 = ( 1.0f  - alpha ) * b0;
            a0 = alpha * b0 * A;
            a1 = 0;
            a2 = -a0;
            break;
          case NOTCH:
            A = h_PowF( 10.f, gain * 0.05f );
            b0 = 1.0f  / ( 1.0f  + alpha );
            b1 = ( -2.0f  * cs ) * b0;
            b2 = ( 1.0f  - alpha ) * b0;
            a0 = b0 * A;
            a1 = b1 * A;
            a2 = a0;
            break;
          case PEQ:
            A = h_PowF( 10.f, gain * 0.025f );  // 40
            temp1 = alpha * A;
            temp2 = alpha / A;
            b0 = 1.0f  / ( 1.0f  + temp2 );
            b1 = ( -2.0f  * cs ) * b0;
            b2 = ( 1.0f  - temp2 ) * b0;
            a0 = ( 1.0f  + temp1 ) * b0;
            a1 = b1;
            a2 = ( 1.0f  - temp1 ) * b0;
            break;
          case LSH:
            A = h_PowF( 10.f, gain * 0.025f );
            temp1 = A + 1.0f; temp2 = A - 1.0f;
            temp3 = temp1 * cs; temp4 = temp2 * cs;
            Q = h_Min(Q / A * A, 1.87f); // limit to 0.75 octaves
            beta = sn * h_SqrtF( ( A * A + 1.0f  ) / Q - temp2 * temp2 );
            b0 = 1.0f  / ( temp1 + temp4 + beta );
            b1 = ( -2.0f  * ( temp2 + temp3 ) ) * b0;
            b2 = ( temp1 + temp4 - beta ) * b0;
            a0 = ( A * ( temp1 - temp4 + beta ) ) * b0;
            a1 = ( 2.0f  * A * ( temp2 - temp3 ) ) * b0;
            a2 = ( A * ( temp1 - temp4 - beta ) ) * b0;
            break;
          case HSH:
            A = h_PowF(10.f, gain * 0.025f);
            temp1 = A + 1.0f; temp2 = A - 1.0f;
            temp3 = temp1 * cs; temp4 = temp2 * cs;
            Q = h_Min(Q / A * A, 1.87f);
            beta = sn * h_SqrtF( ( A * A + 1.0f  ) / Q - temp2 * temp2 );
            b0 = 1.0f  / ( temp1 - temp4 + beta );
            b1 = ( 2.0f  * ( temp2 - temp3 ) ) * b0;
            b2 = ( temp1 - temp4 - beta ) * b0;
            a0 = ( A * ( temp1 + temp4 + beta ) ) * b0;
            a1 = ( -2.0f  * A * ( temp2 + temp3 ) ) * b0;
            a2 = ( A * ( temp1 + temp4 - beta ) ) * b0;
            break;
          case NOF:
            break;
        }
        // has interpolation
        hasintrp = true;
        if (!intrp)
        {
          i_a0 = a0;
          i_a1 = a1;
          i_a2 = a2;
          i_b1 = b1;
          i_b2 = b2;
          hasintrp = false;
        }
      }

    /*
     * interpolate filter coeff <br>
     * <br>
     * \code
     * // call this from axFormat::doProcessBlock(..)
     * // or each N samples: myfilter.interpolate(N)
     * \endcode
     * \param sampleFrames long
     * \return void
     */

    virtual void interpolate(long sampleFrames)
      {
        if (hasintrp && seltype != 0)
        {
          float inv_sb = 1/sampleFrames;
          // ---
          d_a0 = (a0 - s_a0)*inv_sb;
          i_a0 = s_a0;
          s_a0 = a0;
          // ---
          d_a1 = (a1 - s_a1)*inv_sb;
          i_a1 = s_a1;
          s_a1 = a1;
          // ---
          d_a2 = (a2 - s_a2)*inv_sb;
          i_a2 = s_a2;
          s_a2 = a2;
          // ---
          d_b1 = (b1 - s_b1)*inv_sb;
          i_b1 = s_b1;
          s_b1 = b1;
          // ---
          d_b2 = (b2 - s_b2)*inv_sb;
          i_b2 = s_b2;
          s_b2 = b2;
        }
      }

    /**
     * process input sample with filter
     * @param[in] in float
     * @return result float
     */

    virtual float process(const float in)
      {
        if (seltype != 0)
        {
          float result;
          // --
          // interpolate
          if (hasintrp)
          {
            i_a0 += d_a0;
            i_a1 += d_a1;
            i_a2 += d_a2;
            i_b1 += d_b1;
            i_b2 += d_b2;
            result = i_a0*in + i_a1*x1 + i_a2*x2 - i_b1*y1 - i_b2*y2 + DENORM;
          }
          else
          {
            result = a0*in + a1*x1 + a2*x2 - b1*y1 - b2*y2 + DENORM;
          }
          //--------
          x2 = x1;
          x1 = in;
          y2 = y1;
          y1 = result;
          return result - DENORM;
        }
        else
        {
          return in;
        }
      }

};

//----------------------------------------------------------------------
#endif
