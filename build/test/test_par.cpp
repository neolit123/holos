#include "holos.h"

const char* strings[] =
{
  (char*)"string0",
  (char*)"string1",
  (char*)"string2",
  (char*)"string3"
};

class desc : public h_Descriptor
{
  public:
    desc() : h_Descriptor()
    {
      m_Name        = "test_par";
      m_Author      = "na";
      m_Product     = "holos test plugin";
      m_Version     = 0;
      m_UniqueId    = H_MAGIC;
      m_Flags       = df_None;
      m_NumInputs   = 2;
      m_NumOutputs  = 2;
      m_EditorRect  = H_NULL_RECT;
      m_Parameters.append
        (new h_ParFloat("p0", "p0_label", NULL, 4, 0, 10, 0.1));
      m_Parameters.append
        (new h_ParInt("p1", "p1_label", NULL, 0, 0, 3, strings));
    }
};

class inst : public h_Instance
{
  public:
    inst(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host, a_Descriptor)
    {
      prepareParameters();
    }
    
    virtual void do_HandleParameter(h_Parameter* a_Parameter)
    {
      trace(a_Parameter->getIndex() << ":" << a_Parameter->getValue());
    }
};

#define H_DESCRIPTOR  desc
#define H_INSTANCE    inst

#include "holos_impl.h"
