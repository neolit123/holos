#define H_NOGUI
#include "holos.h"
#include "src/h_Parameter.h"

//----------------------------------------------------------------------

const char* str_onoff[] = { (char*)"off", (char*)"on" };

class my_Descriptor : public h_Descriptor
{
  public:

    my_Descriptor()
    : h_Descriptor()
      {
        m_Name   = "fx_blur";
        m_Author = "ccernn";
        appendParameter( new h_ParFloat("size",  "", PF_DEFAULT,0.5 ) );
        appendParameter( new h_ParFloat("decay", "", PF_DEFAULT,0.2 ) );
        appendParameter( new h_ParFloat("xfade", "", PF_DEFAULT,0.5 ) );
        appendParameter( new h_ParFloat("volume","", PF_DEFAULT,0.7 ) );
        appendParameter( new h_ParText ("freeze","", PF_DEFAULT,0, 2, str_onoff)  );
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance
{
  private:
    float*  BUF;
    float*  BUF_t;
    int     pos;
    int     bufsize;
    int     bufsize_t;
  public:
    float   decay, decay_t;
    float   decay2;
    float   vol;
    float   bs,d,d2,v,fr;

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        BUF = new float[0x40000];
        h_Memset(BUF,0,0x30000*4);
        BUF_t = BUF + 0x10000;
        pos = 0;
        bufsize = 0;
        decay = 0;

        // ouch, getSampleRate() crashes!

        //float srate = getSampleRate();
        //trace("srate: " << srate);
        //bufsize_t = srate * (bs*bs);
        //decay_t   = d*d;
        //decay2    = d2*d2;
        //vol       = v*v;

        //transferParameters();
      }

    virtual ~my_Instance()
      {
      }

    virtual void do_HandleState(int a_State)
      {
        switch(a_State)
        {
          case is_Open:
            //trace( "srate: " << getSampleRate() );
            transferParameters();
            break;
        }
      }


    virtual void do_HandleParameter(h_Parameter* a_Parameter)
      {
        // crash !!!
        float srate = getSampleRate(); // not available until is_Resume
        //float srate = 44100; //temp "fix"...
        //trace("srate: " << srate);

        int index = a_Parameter->getIndex();
        float value = a_Parameter->getValue();
        switch (index)
        {
          case 0:
            bs = value / 2;
            bufsize_t = srate * (bs*bs);
            break;
          case 1:
            d = value;
            decay_t = d*d;
            break;
          case 2:
            d2 = value / 10;
            decay2 = d2*d2;
            break;
          case 3:
            v = value * 2;
            vol = v*v;
            break;
          case 4:
            fr = value;
            break;
        }
      }

    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs)
      {
        float spl0 = *a_Inputs[0];
        float spl1 = *a_Inputs[1];

        bufsize += ( bufsize_t - bufsize ) * decay2;
        decay   += ( decay_t   - decay   ) * decay2;

        // left

        if (fr<0.5) BUF_t[pos] = spl0;
        BUF[pos] += (BUF_t[pos]-BUF[pos]) * decay;
        spl0 = BUF[pos] * vol;

        // right

        int po2 = pos + 0x20000;

        if( fr<0.5 ) BUF_t[po2] = spl1;
        BUF[po2] += ( BUF_t[po2] - BUF[po2] ) * decay;
        spl1 = BUF[po2] * vol;

        pos += 1;
        if( pos >= (int)bufsize ) pos=0;

        *a_Outputs[0] = spl0;
        *a_Outputs[1] = spl1;
      }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"



