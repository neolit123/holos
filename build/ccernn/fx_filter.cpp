#define H_NOGUI
#include "holos.h"

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:

    my_Descriptor()
    : h_Descriptor()
      {
        m_Name   = "fx_filter";
        m_Author = "ccernn";
        appendParameter(new h_Parameter("filter","",PF_DEFAULT,0.5));
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance
{
  private:
    bool  lowpass;
    float cur0,cur1;
    float weight;

  public:


    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        // do not call virtual methods in constructor !!!
        // trace( getSampleRate() );
        lowpass = true;
        cur0 = 0;
        cur1 = 0;
        weight = 0;
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
          //case is_Close:
          //  break;
          //case is_Suspend:
          //  trace( "srate: " << getSampleRate() );
          //  break;
          //case is_Resume:
          //  trace( "srate: " << getSampleRate() );
          //  break;
          //case is_Rate:
          //  trace( "srate: " << getSampleRate() );
          //  break;
        }
      }

    virtual void do_HandleParameter(h_Parameter* a_Parameter)
      {
        int index = a_Parameter->getIndex();
        if (index==0)
        {
          float value = a_Parameter->getValue();
          if (value<0.5) { lowpass=true;  value*=2; }
          else { lowpass=false; value=(value-0.5)*2; }
          weight = value*value*value*value;
        }
      }

    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs)
      {
        float in0 = *a_Inputs[0];
        float in1 = *a_Inputs[1];

        cur0 += (in0-cur0) * weight;
        cur1 += (in1-cur1) * weight;

        if (lowpass)
        {
          *a_Outputs[0] = cur0;
          *a_Outputs[1] = cur1;
        }
        else
        {
          *a_Outputs[0] = in0 - cur0;
          *a_Outputs[1] = in1 - cur1;
        }

      }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"



