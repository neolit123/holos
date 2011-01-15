#ifndef h_Core_cpp_included
#define h_Core_cpp_included
//----------------------------------------------------------------------

#ifndef H_DESCRIPTOR
  #define H_DESCRIPTOR h_Descriptor
#endif

#ifndef H_INSTANCE
  #define H_INSTANCE h_Instance
#endif

#ifndef H_EDITOR
  #define H_EDITOR h_Editor
#endif

//----------------------------------------------------------------------

void h_Core::initialize(void)
  {
    if (!m_Initialized)
    {
      m_Platform  = new h_Platform();
      m_Interface = new h_Interface();
      m_Format    = new h_Format();
      m_Initialized = true;
    }
  }

//----------------------------------------

h_Descriptor_Base* h_Core::createDescriptor(void)
  {
    return new H_DESCRIPTOR();
  }

//----------

h_Instance_Base* h_Core::createInstance(h_Descriptor_Base* a_Descriptor)
  {
    return new H_INSTANCE(a_Descriptor);
  }

//----------

h_Editor_Base* h_Core::createEditor(h_Instance_Base* a_Instance, h_Rect a_Rect, void* a_Parent)
  {
    return new H_EDITOR(a_Instance,a_Rect,a_Parent);
  }

//----------------------------------------------------------------------
#endif
