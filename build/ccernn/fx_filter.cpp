#define H_DEBUG_LOG "holos_win.log"
#define H_DEBUG_LOG_UNIQUE
#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

#define H_NOGUI

#include "holos.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

h_ParamDescr my_Params[] =
{
  {"filter","",pt_None,PF_DEFAULT, 0.5  }
};

//----------------------------------------------------------------------

h_Descriptor my_Descriptor =
{
  "fx_filter",
  "ccernn",
  "holos test plugin",
  0,
  H_MAGIC,
  df_None,
  2,    // inputs
  2,    // outputs
  1,    // parameters
  0,    // programs
  my_Params,
  H_NULL,
  H_NULL_RECT
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
        lowpass = true;
        cur0 = 0;
        cur1 = 0;
        weight = 0;
        transferParameters();
      }

    virtual ~my_Instance()
      {
      }

    //----------

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

    //----------

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



