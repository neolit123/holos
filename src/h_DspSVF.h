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
#ifndef h_DspSVF_included
#define h_DspSVF_included
//----------------------------------------------------------------------

/**
 * @file
 * \brief state variable filter
 */

#define svf_mode_none 0
#define svf_mode_lp   1
#define svf_mode_hp   2
#define svf_mode_bp   3
#define svf_mode_n    4

/**
 * \brief state variable filter
 *
 * based on hal chamberlin's book
 * 'Musical Applications of Microprocessors: '
 * http://www.earlevel.com/Digital%20Audio/StateVar.html
 */

class h_DspSVF
{
  private:

    /*@{*/
    float z1_L,z2_L; /**< temp variables */
    /*@{*/

  public:

    /*@{*/
    int   mMode; /**< mode [lp, hp, bp, br] */
    float mFreq; /**< frequency */
    float mBW;   /**< bandwidth */
    /*@{*/

  public:

    /**
     * constructor
     */

    h_DspSVF()
      {
        z1_L =z2_L = 0;
        setup(0,1,1);
      }

    /**
     * set filter parameters:
     * @param[in] m int mode
     * @param[in] f float frequency
     * @param[in] b float bandwidth
     */

    void setup(int m, float f, float b)
      {
        mMode  = m;
        mFreq  = f;
        mBW    = b;
      }

    //void resume(void)
    //  {
    //    z1 = z2 = 0;
    //  }

    /**
     * process inputs with filter
     * @param[in] ins float**
     * @param[in] outs float**
     * @return out_L float
     */

    virtual float process(float** ins, float** outs)
    //float process(float in)
      {
        float in_L = *ins[0];
        float out_L = 0;
        if( mMode==0 ) out_L = in_L;
        else
        {
          float L = z2_L + mFreq * z1_L;
          float H = in_L - L - mBW   * z1_L;
          float B = mFreq * H + z1_L;
          float N = H + L;
          z1_L = B;
          z2_L = L;
          switch(mMode)
          {
            case 1: out_L = L; break;
            case 2: out_L = H; break;
            case 3: out_L = B; break;
            case 4: out_L = N; break;
          } //switch
        } //mMode
        return out_L;
      } //process

};

//----------------------------------------------------------------------
#endif
