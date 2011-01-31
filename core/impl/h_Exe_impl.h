#ifndef h_Exe_impl_included
#define h_Exe_impl_included
#ifdef h_Exe_included
//----------------------------------------------------------------------

#include "gui/h_Window.h"

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
//: h_Instance_Base(a_Descriptor)
  {
    m_Host = a_Host;
    m_Descriptor = a_Descriptor;
    m_EditorRect = m_Descriptor->m_EditorRect;// h_Rect(0,0,320,240);
    m_EditorIsOpen = false;
  }

//----------

h_Instance::~h_Instance()
  {
  }


//----------

void h_Instance::initParameters(void)
  {
  }

//----------

void h_Instance::prepareParameters(void)
  {
  }


//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------

h_Format::h_Format()
  {
  }

//----------

h_Format::~h_Format()
  {
  }

//----------------------------------------

h_Descriptor* h_Format::getDescriptor(void)
  {
    return &H_DESCRIPTOR;
  }

//----------

h_Instance* h_Format::createInstance(h_Host* a_Host, h_Descriptor* a_Descriptor)
  {
    return new H_INSTANCE(a_Host,a_Descriptor);
  }

//----------------------------------------

int h_Format::entrypoint(void* a_Ptr)
  {

    h_Host* host = new h_Host();
    h_Descriptor* descriptor = getDescriptor();
    h_Instance* instance = createInstance(host,descriptor);
    if (descriptor->m_Flags & df_HasEditor)
    {
      h_Window* win = (h_Window*)instance->do_OpenEditor(a_Ptr);
      win->eventLoop();
      //instance->eventLoop();
      instance->do_CloseEditor();
    }
    delete instance;
    delete host;
    return 0;
  }

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main(void)
{
  static_Debug.initialize();
  static_Core.initialize();
  return static_Core.m_Format->entrypoint(H_NULL);
}

//----------------------------------------------------------------------
#endif
#endif
