
#define H_NOGUI

#include <math.h>     // sinf

#include <memory.h>   // memset
#include "holos.h"
#include "extern/mverb/MVerb.h"

//----------------------------------------------------------------------

h_ParamDescr my_Params[] =
{
  {"Damping",       "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Density",       "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Bandwidth",     "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Decay",         "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Predelay",      "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Size",          "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Gain",          "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Mix",           "",pt_None,pf_Automate, 0.5, 0,1,0},
  {"Early/Late Mix","",pt_None,pf_Automate, 0.5, 0,1,0}
};

//----------

h_Descriptor my_Descriptor =
{
  "mverb",
  "martin eastwood",
  "holos port",
  0,
  H_MAGIC + 0xf000,
  df_None,
  2,    // inputs
  2,    // outputs
  9,    // parameters
  0,    // programs
  my_Params,
  H_NULL
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance
{
  private:
    MVerb<float> em_verb;

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
      }

    //----------

    virtual ~my_Instance()
      {
      }

    //----------

    virtual void do_HandleState(int a_State)
      {
        switch (a_State)
        {
          case is_Resume:
            transferParameters();
            em_verb.setSampleRate( getSampleRate());
            break;
        }
      }

    //----------

    virtual void do_HandleParameter(h_Parameter* a_Parameter)
      {
        int   index = a_Parameter->getIndex();
        float value = a_Parameter->getValue();
        em_verb.setParameter(index,value);
      }

    //----------

    virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Size)
      {
        em_verb.process(a_Inputs,a_Outputs,a_Size);
        return true;
      }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"
