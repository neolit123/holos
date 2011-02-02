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

/**
  \file parInteger.h
  \brief parameter with integer values
*/

#ifndef parInteger_included
#define parInteger_included
//----------------------------------------------------------------------

#include "par/axParameter.h"

class parInteger : public axParameter
{
  public:
    int     mMin, mMax, mStep;
    char**  mStrings;
    float   mRange;
    float   mInvRange;
    float   mHalfStep;

  public:

    parInteger( axParameterListener* aListener, const axString aName,
                const axString aLabel="", const int aValue=0,
                const int aMin=0, const int aMax=100, char** aStrings=NULL)
    : axParameter(aListener,aName,aLabel)
      {
        setup(aValue, aMin, aMax, aStrings);
      }

    //virtual ~parFloat() {}

    //--------------------------------------------------

    void setup(const int aVal, const int aMin, const int aMax, char** aStrings)
      {
        mMin       = aMin;
        mMax       = aMax;
        mRange     = mMax - mMin + 1;      // 4
        mInvRange  = 1/mRange; 
        mHalfStep  = (mInvRange)*0.5;
        mStrings   = aStrings;
        setInt(aVal);
      }

    //----------

    virtual void setInt(int aValue)
      {
        mValue = (float)(aValue-mMin) * mInvRange;
        mValue += mHalfStep;
      }

    //----------

    virtual int getInt(void)
      {
        //return mRange * (float)(mValue + mMin);
        const float n = axFloor (mValue * mRange );
        return (mMin + axMinInt(n, (mRange-1)));
      }

    //--------------------------------------------------

    virtual void  setValue(const float aValue)  { setInt ((int)aValue); }
    virtual float getValue(void)                { return (float)getInt(); }

    //--------------------------------------------------

    //virtual void  doReset(void)             { mValue=mDefault; if(mListener) mListener->onChange(this); }
    //virtual void  doSetValue(float aValue)  { mValue=axMax(0,axMin(1,aValue)); if(mListener) mListener->onChange(this); }
    //virtual float doGetValue(void)          { return mValue; }
    //virtual void  doGetName(char* buf)      { strcpy(buf,mName.ptr()); }
    //virtual void  doGetLabel(char* buf)     { strcpy(buf,mLabel.ptr()); }

    virtual void  doGetDisplay(char* buf)
      {
        const int i = getInt();
        if( mStrings ) axStrcpy( buf, mStrings[i] );
        //else __builtin_sprintf( buf, "%i", (int)i );
        else axItoa(buf,i);
      }

    //--------------------------------------------------

};

//----------------------------------------------------------------------
#endif

