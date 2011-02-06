#define H_DEBUG_LOG "holos_diagnosis.log"
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

#include "holos.h"

#include "gui/h_Window.h"
#include "gui/wdg/wdg_Background.h"
#include "gui/skin/skin_Default.h"

#include "lib/h_Cpu.h"
#include "lib/h_DetectWine.h"

//----------------------------------------------------------------------

h_Descriptor my_Descriptor =
{
  "holos_diagnosis",
  "ccernn",
  "holos diagnosis plugin",
  0,
  H_MAGIC,
  //df_HasEditor | df_ReceiveMidi,
  df_None,
  2,    // inputs
  2,    // outputs
  0,    // parameters
  0,    // programs
  H_NULL,
  H_NULL,
  h_Rect(256,256)
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance,
                    public h_WidgetListener
{
  private:

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        trace("platform: " << static_Core.m_Platform->getName().ptr() );
          if (h_DetectWine()) { trace("- wine: yes"); }
          else  { trace("- wine: no"); }

        trace("format: " << static_Core.m_Format->getName().ptr());

        trace("cpu:");
          h_Cpu cpu;
          #ifdef _H_64BIT_
          trace("- bits: 64");
          #endif
          #ifdef _H_32BIT_
          trace("- bits: 32");
          #endif
          if (h_IsLittleEndian()) { trace("- endian: little"); }
          else { trace("- endian: big"); }
          trace("- caps: " << cpu.CpuCapsString() );
      }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"


