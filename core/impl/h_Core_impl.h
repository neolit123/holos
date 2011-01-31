#ifndef h_Core_impl_included
#define h_Core_impl_included
#ifdef h_Core_included
//----------------------------------------------------------------------

h_Core::h_Core()
  {
    m_Initialized = false;
    m_Platform  = H_NULL;
    m_Format = H_NULL;
  }

//----------

h_Core::~h_Core()
  {
    if (m_Platform)  delete m_Platform;
    if (m_Format) delete m_Format;
  }

//----------

void h_Core::initialize(void)
  {
    if (!m_Initialized)
    {
      m_Platform  = new h_Platform();
      m_Format = new h_Format();
      m_Initialized = true;
    }
  }

//----------------------------------------------------------------------
#endif
#endif
