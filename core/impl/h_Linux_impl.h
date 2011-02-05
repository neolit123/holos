/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_Linux_impl_included
#define h_Linux_impl_included
#ifdef h_Linux_included
//----------------------------------------------------------------------
/*

// http://www.remlab.net/op/xlib.shtml

.."XInitThreads() is the most important, and is required for the later two
functions to work anyway. XLockDisplay() and XUnlockDisplay() provide a
recursive lock for the unlikely event that a single display is shared across
multiple threads, which is generally a bad idea and not terribly useful..."

... the last part there, 'generally a bad idea'..

*/
//----------------------------------------------------------------------

h_Platform::h_Platform()
  {
    #ifndef H_NOGUI
      // The XInitThreads() function initializes Xlib support for concurrent threads.
      // This function must be the first Xlib function a multi-threaded program calls,
      // and it must complete before any other Xlib call is made.
      XInitThreads();
    #endif
  }

//----------

h_Platform::~h_Platform()
  {
  }

//--------------------------------------------------

Display* h_Platform::openDisplay(void)
  {
    // Specifies the hardware display name, which determines the display and
    // communications domain to be used. On a POSIX-conformant system, if the
    // display_name is NULL, it defaults to the value of the DISPLAY
    // environment variable.
    // After a successful call to XOpenDisplay() all of the screens in the
    // display can be used by the client.
    return XOpenDisplay(NULL);
  }

//----------

void h_Platform::closeDisplay(Display* a_Display)
  {
    // The XCloseDisplay() function closes the connection to the X server for
    // the display specified in the Display structure and destroys all windows,
    // resource IDs (Window, Font, Pixmap, Colormap, Cursor, and GContext), or
    // other resources that the client has created on this display, unless the
    // close-down mode of the resource has been changed (see XSetCloseDownMode)
    XCloseDisplay(a_Display);
  }

//----------------------------------------------------------------------
#endif
#endif

    /*
    m_WinDisplay   = XOpenDisplay(NULL);
    // XSync???
    // "..it must complete before any other Xlib call is made.."
    m_WinRoot      = XDefaultRootWindow(m_WinDisplay);            // the root window for the default screen
    m_WinScreen    = XDefaultScreen(m_WinDisplay);                // the default screen number referenced by the XOpenDisplay() function
    m_WinVisual    = XDefaultVisual(m_WinDisplay,m_WinScreen);    // the default visual type for the specified screen
    m_WinDepth     = XDefaultDepth(m_WinDisplay,m_WinScreen);     // the depth (number of planes) of the default root window for the specified screen
    m_WinColormap  = XDefaultColormap(m_WinDisplay,m_WinScreen);  // the default colormap ID for allocation on the specified screen
    */
