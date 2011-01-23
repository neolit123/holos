#ifndef h_Exe_impl_included
#define h_Exe_impl_included
#ifdef h_Exe_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// instance
//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
//: h_Instance_Base(a_Descriptor)
  {
  }

//----------

h_Instance::~h_Instance()
  {
  }


//----------------------------------------------------------------------
// format
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
      instance->do_OpenEditor();
      //instance->eventLoop();
      instance->do_CloseEditor();
    }
    delete instance;
    delete host;
    return 0;
  }

//----------------------------------------------------------------------
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
