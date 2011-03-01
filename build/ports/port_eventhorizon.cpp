/*
  Copyright 2006, Thomas Scott Stillwell
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  The name of Thomas Scott Stillwell may not be used to endorse or
  promote products derived from this software without specific prior written
  permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define H_NOGUI
#include "holos.h"

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:
    my_Descriptor()
    : h_Descriptor()
      {
        m_Name        = "event horizon";
        m_Author      = "sstillwell";
        m_Product     = "holos port";
        m_Version     = 2;
        m_UniqueId    = H_MAGIC + 0xf001;
        m_Flags       = df_None;
        m_NumInputs   = 2;
        m_NumOutputs  = 2;
        appendParameter( new h_ParFloat("threshold", "",   PF_DEFAULT,  0.0, -30.0,  0.0, 0.1  ) );
        appendParameter( new h_ParFloat("ceiling",   "",   PF_DEFAULT, -0.1, -20.0,  0.0, 0.1  ) );
        appendParameter( new h_ParFloat("soft clip", "db", PF_DEFAULT,  2.0,   0.0,  6.0, 0.01 ) );
        //appendParameter( new parFloat(this,"soft clip ratio","",   10.0,   3.0, 20.0, 0.1  ) );
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance
{
  private:

    float _log2db,_db2log;
    float slider1,slider2,slider3,slider4;
    float thresh,threshdb;
    float ceiling,ceildb,makeup,makeupdb;
    float sc,scv,sccomp,peakdb,peaklvl;
    float scratio,scmult;

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        _log2db =  8.6858896380650365530225783783321;  // 20 / ln(10)
        _db2log =  0.11512925464970228420089957273422; // ln(10) / 20
        slider1 =  0.0;
        slider2 = -0.1;
        slider3 =  2.0;
        slider4 =  10;
        transferParameters();
      }

    virtual ~my_Instance()
      {
      }

    //[internal]
    void recalcAll(void)
      {
        thresh    = h_Exp(slider1 * _db2log);
        threshdb  = slider1;
        ceiling   = h_Exp(slider2 * _db2log);
        ceildb    = slider2;
        makeup    = h_Exp((ceildb-threshdb) * _db2log);
        makeupdb  = ceildb - threshdb;
        sc        = -slider3;
        scv       = h_Exp( sc * _db2log);
        sccomp    = h_Exp(-sc * _db2log);
        peakdb    = ceildb + 25;
        peaklvl   = h_Exp(peakdb * _db2log);
        scratio   = slider4;
        scmult    = h_Abs((ceildb - sc) / (peakdb - sc));
      }

    virtual void do_HandleParameter(h_Parameter* a_Parameter)
      {
        int   index = a_Parameter->getIndex();
        float value = a_Parameter->getValue();
        switch (index)
        {
          case 0: slider1 = value; break;
          case 1: slider2 = value; break;
          case 2: slider3 = value; break;
          case 3: slider4 = value; break;
        }
        recalcAll();
      }

    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs)
      {
        float spl0 = *a_Inputs[0];
        float spl1 = *a_Inputs[1];
        //float peak = axMax(axAbs(spl0),axAbs(spl1));
        spl0 = spl0 * makeup;
        spl1 = spl1 * makeup;
        float sign0 = h_Sign(spl0);
        float sign1 = h_Sign(spl1);
        float abs0 = h_Abs(spl0);
        float abs1 = h_Abs(spl1);
        float overdb0 = 2.08136898 * h_Log(abs0) * _log2db - ceildb;
        float overdb1 = 2.08136898 * h_Log(abs1) * _log2db - ceildb;
        if (abs0>scv) { spl0 = sign0 * (scv + h_Exp(overdb0*scmult)*_db2log); }
        if (abs1>scv) { spl1 = sign1 * (scv + h_Exp(overdb1*scmult)*_db2log); }
        spl0 = h_Min(ceiling,h_Abs(spl0)) * h_Sign(spl0);
        spl1 = h_Min(ceiling,h_Abs(spl1)) * h_Sign(spl1);
        *a_Outputs[0] = spl0;
        *a_Outputs[1] = spl1;
      }
};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"


/* original js:

// Copyright 2006, Thomas Scott Stillwell
// All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are permitted
//provided that the following conditions are met:
//
//Redistributions of source code must retain the above copyright notice, this list of conditions
//and the following disclaimer.
//
//Redistributions in binary form must reproduce the above copyright notice, this list of conditions
//and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//The name of Thomas Scott Stillwell may not be used to endorse or
//promote products derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
//IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
//BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
//THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
desc:peak-eating limiter

slider1:0.0<-30.0,0.0,0.1>Threshold
slider2:-0.1<-20.0,0.0,0.1>Ceiling
slider3:2.0<0,6.0,0.01>Soft clip (dB)
//slider4:10<3,20,0.1>Soft clip ratio

@init
  pi = 3.1415926535;
  log2db = 8.6858896380650365530225783783321; // 20 / ln(10)
  db2log = 0.11512925464970228420089957273422; // ln(10) / 20

@slider
  thresh=exp(slider1 * db2log);
  threshdb=slider1;
  ceiling=exp(slider2 * db2log);
  ceildb=slider2;
  makeup=exp((ceildb-threshdb) * db2log);
  makeupdb=ceildb-threshdb;
  sc = -slider3;
  scv = exp(sc * db2log);
  sccomp = exp(-sc * db2log);
  peakdb = ceildb+25;
  peaklvl = exp(peakdb * db2log);
  scratio = slider4;
  scmult = abs((ceildb - sc) / (peakdb - sc));

@sample
  peak=max(abs(spl0),abs(spl1));
  spl0=spl0*makeup;
  spl1=spl1*makeup;
  sign0 = sign(spl0);
  sign1 = sign(spl1);
  abs0=abs(spl0);
  abs1=abs(spl1);
  overdb0 = 2.08136898 * log(abs0) * log2db - ceildb;
  overdb1 = 2.08136898 * log(abs1) * log2db - ceildb;
  abs0 > scv ? (
    spl0=sign0*(scv + exp(overdb0*scmult)*db2log);
  );
  abs1 > scv ? (
    spl1=sign1*(scv + exp(overdb1*scmult)*db2log);
  );
  spl0=min(ceiling,abs(spl0))*sign(spl0);
  spl1=min(ceiling,abs(spl1))*sign(spl1);

*/
