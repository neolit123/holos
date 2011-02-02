#ifndef h_Exe_impl_included
#define h_Exe_impl_included
#ifdef h_Exe_included
//----------------------------------------------------------------------

#ifndef H_NOGUI
  #include "gui/h_Window.h"
#endif

#include "core/impl/h_Exe_Host_impl.h"
#include "core/impl/h_Exe_Instance_impl.h"
#include "core/impl/h_Exe_Format_impl.h"

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

//int main(void)
int main(int argc, char** argv)
{
  static_Debug.initialize();
  static_Core.initialize();
  //parse cmdline args -> entrypoint
  return static_Core.m_Format->entrypoint(H_NULL);
}

//----------------------------------------------------------------------
#endif
#endif
