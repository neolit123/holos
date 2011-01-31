#ifndef holos_impl_included
#define holos_impl_included
#ifdef holos_h_included
//----------------------------------------------------------------------

//#ifdef H_INCLUDE_SRC

  #include "debug/impl/h_Debug_impl.h"
  #include "core/impl/h_Core_impl.h"

  //----------

  #include "core/impl/h_Linux_impl.h"
  #include "core/impl/h_Win32_impl.h"

  //----------

  #ifndef H_DESCRIPTOR
    #define H_DESCRIPTOR static_Descriptor
  #endif

  #ifndef H_INSTANCE
    #define H_INSTANCE h_Instance
  #endif

  //#ifndef H_EDITOR
  //  #define H_EDITOR h_Editor
  //#endif

  //----------

  #include "core/impl/h_Exe_impl.h"
  #include "core/impl/h_Ladspa_impl.h"
  #include "core/impl/h_Vst_impl.h"

//#endif

//----------------------------------------------------------------------
#endif
#endif
