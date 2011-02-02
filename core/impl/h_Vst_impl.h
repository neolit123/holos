#ifndef h_Vst_impl_included
#define h_Vst_impl_included
#ifdef h_Vst_included
//----------------------------------------------------------------------

#include "extern/vst/aeffect.h"
#include "extern/vst/aeffectx.h"
//#include "extern/vst/vstfxstore.h"

#include "core/impl/h_Vst_Host_impl.h"
#include "core/impl/h_Vst_Instance_impl.h"
#include "core/impl/h_Vst_Format_impl.h"

//
//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------
//

#ifdef H_LINUX
  #define _H_ASM_MAIN_SYMBOL asm ("main");
#endif
#ifdef H_WIN32
  #define _H_ASM_MAIN_SYMBOL asm ("_main");
#endif

//----------

// a.k.a. createEffectInstance
AEffect*  main_plugin(audioMasterCallback audioMaster) _H_ASM_MAIN_SYMBOL
#define   main main_plugin

//----------

AEffect* main(audioMasterCallback audioMaster)
{
  static_Debug.initialize();
  static_Core.initialize();
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;
  return static_Core.m_Format->entrypoint(audioMaster);
}

//TODO:
// be sure we handle multiple instances properly..
// main() is called for each instance
// (and for the initial enumeration)

//----------------------------------------------------------------------
#endif
#endif

