#ifndef h_Ladspa_Format_impl_included
#define h_Ladspa_Format_impl_included
#ifdef h_Ladspa_included
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

h_Instance* h_Format::createInstance(h_Descriptor* a_Descriptor)
  {
    return new H_INSTANCE(a_Descriptor);
  }

//----------------------------------------

int h_Format::entrypoint(void* a_Ptr)
  {

    h_Descriptor* descriptor = getDescriptor();
    h_Instance* instance = createInstance(descriptor);
//    if (descriptor->m_Flags & df_HasEditor)
//    {
//      instance->do_OpenEditor();
//      //instance->eventLoop();
//      instance->do_CloseEditor();
//    }
    delete instance;
    return 0;
  }


//----------------------------------------------------------------------
#endif
#endif

