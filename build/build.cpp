
#include "holos.h"
//----------------------------------------------------------------------

#include "lib/h_BasePath.h"

class my_Instance : public h_Instance
{
  public:
    my_Instance(h_Descriptor* a_Descriptor)
    : h_Instance(a_Descriptor)
      {
        //trace("my_Instance");
        //trace("base path: ");
        //trace( h_GetBasePath() );
      }
    virtual ~my_Instance()
      {
      }
};

// plugin code

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"
