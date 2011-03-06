
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

//#define H_DEBUG_LOG "holos_debug.log"

//#define H_DEBUG_MEM
//#define H_DEBUG_MEM_PRINT
//#define H_DEBUG_MEM_NOREDEFINE
//#define H_DEBUG_NEW

#define H_NOGUI
#include "holos.h"
#include "src/h_Rand.h"

//----------------------------------------------------------------------

const char* strings[] = { (char*)"string0", (char*)"string1", (char*)"string2", (char*)"string3" };

float p2(const float x) { return x*x; }
float inv_p2(const float x) { return h_Sqrt(x); }

class my_Descriptor : public h_Descriptor
{
  public:

    my_Descriptor()
    : h_Descriptor()
      {
        m_Name        = "test_ladspa";
        m_Author      = "ccernn";
        m_Product     = "holos test plugin";
        m_Version     = 0;
        m_UniqueId    = H_MAGIC;
        m_Flags       = df_None;
        m_NumInputs   = 2;    // inputs
        m_NumOutputs  = 2; // outputs
        //m_EditorRect  = h_Rect(0,0,640,480);

        appendParameter( new h_Parameter("param",   "", PF_DEFAULT, 0 ) );
        appendParameter( new h_ParFloat( "float",   "", PF_DEFAULT, 1.5, 0,5, 0.25 ) );
        appendParameter( new h_ParInt(   "int",     "", PF_DEFAULT, 3,   1,   10 ) );

        appendParameter( new h_ParFloat("parFloat", "", PF_DEFAULT, 440, 1, 44100, 0, &p2, &inv_p2 )); // 16
        appendParameter( new h_ParInt(  "parInt",   "", PF_DEFAULT, 256, 1,1024, &p2, &inv_p2 )); // 16
        appendParameter( new h_ParText( "parText",  "", PF_DEFAULT, 2,   4, strings));
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance
{
  private:
    h_Rand rnd;

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        rnd.seed(19);
      }

    //----------

    virtual ~my_Instance()
      {
      }

    //----------

    virtual void  do_HandleParameter(h_Parameter* a_Parameter)
      {
        char disp[H_MAX_STRINGSIZE];
        a_Parameter->getDisplay(disp);
        trace("param: " << a_Parameter->getName().ptr() << " = " << disp );
//        float i = a_Parameter->getInternal();
//        float v = a_Parameter->getValue();
//        char buf[32];
//        h_Ftoa(buf,i);
//        trace( a_Parameter->getName().ptr() );
//        trace( "  i: " << i);
//        trace( "  v: " << v);
//        a_Parameter->getDisplay(buf);
//        trace( "  d: " << buf);
      }

    //----------

    virtual void do_HandleState(int a_State)
      {
        switch(a_State)
        {
          case is_Open:
            transferParameters();
            break;
        }
      }

    //----------

    //virtual void do_HandleMidi(int a_Offset, unsigned char a_Msg1, unsigned char a_Msg2, unsigned char a_Msg3)
    //  {
    //  }

    //----------

    virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Length)
      {
        return false;
      }

    //----------

    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs)
      {
        *a_Outputs[0] = rnd.randSigned();
        *a_Outputs[1] = rnd.randSigned();
      }

    //----------

    //virtual void do_PostProcess(float** a_Inputs, float** a_Outputs, int a_Length)
    //  {
    //  }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------

#include "holos_impl.h"



