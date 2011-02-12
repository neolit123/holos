/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef h_ParDb_included
#define h_ParDb_included
//----------------------------------------------------------------------

// dB / decibel paramter

#include "h/h_ParFloat.h"

//----------------------------------------------------------------------

//[untested]

//class h_ParDb : public h_ParFloat
//{
//  public:
//    h_ParDb(h_ParameterListener* aListener, axString aName, axString aLabel="", float aValue=0, float aMin=0, float aMax=1, float aStep=0)
//    : h_ParFloat(aListener,aName,aLabel,aValue,aMin,aMax,aStep)
//      {
//      }
//    virtual float getValue(void)
//      {
//        float v = parFloat::getValue();
//        // don't remember wqhere i found this. is it correct?
//        return 10^(v/20);
//        //return axPowf(10,(v/20));
//      }
//    //TODO: setValue(float db)
//};

//----------------------------------------------------------------------
#endif
