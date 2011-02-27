
#define H_NOGUI

#include <math.h>     // sinf
#include <memory.h>   // memset
#include "holos.h"
#include "extern/mverb/MVerb.h"

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:
    my_Descriptor()
    : h_Descriptor()
      {
        m_Name        = "mverb";
        m_Author      = "martin eastwood";
        m_Product     = "holos port";
        m_Version     = 2;
        m_UniqueId    = H_MAGIC + 0xf000;
        m_Flags       = df_None;
        m_NumInputs   = 2;
        m_NumOutputs  = 2;

        appendParameter( new h_Parameter("Damping",       "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Density",       "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Bandwidth",     "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Decay",         "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Predelay",      "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Size",          "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Gain",          "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Mix",           "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Early/Late Mix","", PF_DEFAULT, 0.5 ) );
      }
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
            transferParameters(); // should we do this every time we suspend/resums?
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
#include "holos.cpp"
