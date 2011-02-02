#ifndef h_Exe_Instance_impl_included
#define h_Exe_Instance_impl_included
#ifdef h_Exe_included
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
#endif
#endif
