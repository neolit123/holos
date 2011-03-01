
#define H_DEBUG_LOG "holos_win.log"

//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

#define H_DEBUG_MEM
#define H_DEBUG_MEM_PRINT
#define H_DEBUG_NEW

#include "holos.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:

    my_Descriptor()
    : h_Descriptor()
      {
        m_Name        = "build.cpp";
        m_Author      = "ccernn";
        m_Product     = "holos test plugin";
        m_Version     = 0;
        m_UniqueId    = H_MAGIC;
        m_Flags       = df_None;
        m_NumInputs   = 2; // inputs
        m_NumOutputs  = 2; // outputs
        m_EditorRect  = H_NULL_RECT;
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

#include "src/h_Memory.h"

class my_Instance : public h_Instance
{
  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {

        void* test1 = h_Malloc(1000);
        char* test2 = new char[2000];

        //h_Free(test1);
        //h_Free(test1);
        //h_Free(test2);

        //delete test1;
        //delete[] test2;

      }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------

//#include "holos_impl.h"
#include "holos_impl.h"


