#define H_DEBUG_LOG "holos_win.log"
#define H_DEBUG_CON

//#define H_AUTOSYNC


#include "holos.h"
//----------------------------------------------------------------------

#include "lib/h_BasePath.h"
//#include "lib/h_Rand.h"
//#include "lib/h_Expression.h"

class my_Instance : public h_Instance
{
  public:
    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        char buffer[256];
        trace("base path: " << h_GetBasePath(buffer));

      }
    virtual ~my_Instance()
      {
      }
};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"


