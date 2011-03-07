#define H_DEBUG_CON
#include "holos.h"

const char* strings[] =
{
  (char*)"string0",
  (char*)"string1",
  (char*)"string2",
  (char*)"string3"
};

float pow2(const float x)
{
  return h_Pow(x, 2.f);
}

float inv_pow2(const float x)
{
  return h_Log(x) / h_Log(2.f);
}

h_Parameters test_Parameters;

class desc : public h_Descriptor
{
  public:
    desc() : h_Descriptor()
    {
      test_Parameters = m_Parameters;

      m_Name        = "test_par";
      m_Author      = "na";
      m_Product     = "holos test plugin";
      m_Version     = 0;
      m_UniqueId    = H_MAGIC;
      m_Flags       = df_None;
      m_NumInputs   = 2;
      m_NumOutputs  = 2;
      m_EditorRect  = H_NULL_RECT;
      appendParameter
        (new h_ParFloat("parFloat", "", H_NULL, 2.5, 0, 5, 0.01, &pow2, &inv_pow2 ));
      appendParameter
        (new h_ParInt("parInt", "", H_NULL, 4, 0, 40, &pow2, &inv_pow2 ));
      appendParameter
        (new h_ParInt("parBool", "", H_NULL, 0, 0, 1));
      appendParameter
        (new h_ParText("parText", "", H_NULL, 2, 4, strings));
      
    }
};

class inst : public h_Instance
{
  public:
    inst(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host, a_Descriptor)
    {
      transferParameters();
    }

    virtual void do_HandleParameter(h_Parameter* a_Parameter)
    {
      trace(a_Parameter->getIndex() << ":" << a_Parameter->getValue());
    }
};

#define H_DESCRIPTOR  desc
#define H_INSTANCE    inst

#include "holos_impl.h"
