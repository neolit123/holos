#ifndef h_Linux_impl_included
#define h_Linux_impl_included
#ifdef h_Linux_included
//----------------------------------------------------------------------

h_Platform::h_Platform()
  {
    #ifndef H_NOGUI
    XInitThreads();
    m_WinDisplay   = XOpenDisplay(NULL);
    m_WinRoot      = XDefaultRootWindow(m_WinDisplay);
    m_WinScreen    = XDefaultScreen(m_WinDisplay);
    m_WinVisual    = XDefaultVisual(m_WinDisplay,m_WinScreen);
    m_WinDepth     = XDefaultDepth(m_WinDisplay,m_WinScreen);
    m_WinColormap  = XDefaultColormap(m_WinDisplay,m_WinScreen);
    #endif
  }

//----------

h_Platform::~h_Platform()
  {
    #ifndef H_NOGUI
    XCloseDisplay(m_WinDisplay);
    #endif
  }

//----------------------------------------------------------------------
#endif
#endif
