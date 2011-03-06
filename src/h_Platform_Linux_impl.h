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
#ifndef h_Platform_Linux_impl_included
#define h_Platform_Linux_impl_included
//#ifdef h_Linux_included
//----------------------------------------------------------------------

h_Platform::h_Platform()
  {
    #ifndef H_NOGUI

      // The XInitThreads() function initializes Xlib support for concurrent threads.
      // This function must be the first Xlib function a multi-threaded program calls,
      // and it must complete before any other Xlib call is made.

      /*
        // http://www.remlab.net/op/xlib.shtml
        .."XInitThreads() is the most important, and is required for the later two
        functions to work anyway. XLockDisplay() and XUnlockDisplay() provide a
        recursive lock for the unlikely event that a single display is shared across
        multiple threads, which is generally a bad idea and not terribly useful..."
      */

      XInitThreads();

    #endif
  }

h_Platform::~h_Platform()
  {
    //trace("platform dying...");
  }

h_String h_Platform::getName(void)
  {
    return "linux";
  }

/*

  these are currently here because i wasn't sure if we wanted to have one
  display connection, and share this among multiple instances, or if each
  plugin instance should have its own display connection.

  currently each plugin has its own connection

*/

//#ifndef H_NOGUI
//
//Display* h_Platform::openDisplay(void)
//  {
//    /*
//      Specifies the hardware display name, which determines the display and
//      communications domain to be used. On a POSIX-conformant system, if the
//      display_name is NULL, it defaults to the value of the DISPLAY
//      environment variable.
//      After a successful call to XOpenDisplay() all of the screens in the
//      display can be used by the client.
//    */
//    return XOpenDisplay(NULL);
//  }
//
//void h_Platform::closeDisplay(Display* a_Display)
//  {
//    /*
//      The XCloseDisplay() function closes the connection to the X server for
//      the display specified in the Display structure and destroys all windows,
//      resource IDs (Window, Font, Pixmap, Colormap, Cursor, and GContext), or
//      other resources that the client has created on this display, unless the
//      close-down mode of the resource has been changed (see XSetCloseDownMode)
//
//      XCloseDisplay() performs a final XSync() operation
//    */
//
//    /*
//      http://tronche.com/gui/x/xlib/display/close-operation.html
//
//    */
//
//    XCloseDisplay(a_Display);
//  }
//
//#endif // !H_NOGUI

//----------------------------------------------------------------------
//#endif
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







/*

much of the linux/gui implementation is based on this post:
http://www.anticore.org/jucetice/forums/viewtopic.php?pid=619#p619

Here's how I've implemented linux VST plugins - I've had working
prototypes in various hosts (energyXT, renoise, qtractor - apart from a window
sizing bug which is now fixed in my build  - and JOST).

I don't use any toolkits e,g, Qt or GTK - I've written my own AudioFX GUI engine
to use the smallest subset of XLib necessary. (Beware XLib programming can drive
you insane!) sounds like this might be a similar principle to your axonlib.

I don't call XInitThreads and I don't assume that the host does (in fact
qtractor explicitly doesn't and my plugins work fine in that too) I am sure that
the main purpose of XInitThreads is to stop different threads trying to talk
down the same display connection to X which is where the real problems lie.
XLib seems to be thread safe as long as you make sure you have a separate
display connection for each thread.  The problems occur if two threads talk on
the 'wire' at the same time, then because the X protocol is not atomic you get
garbled messages.  This is the main reason to use a separate display* to
reference each thread connection to X.

If you try and use XInitThreads you also need XLockDisplay and its counterpart,
you would also need to be sure that the host played nicely in this respect which
again is not guaranteed.

I start a GUI thread for each plugin instance (this may seem extravagant, there
is a limit to the number of threads that can run concurrently but in practice
you are more likely to run out of other CPU resources before this happens).

All access to the GUI elements in that plugin instance must then go through that
thread.  This takes some care to engineer, for example you must not update the
GUI directly in response to a setParameter, but instead mark the control as
'dirty' in some fashion and rely on an Idle callback in the GUI thread to do
the house-keeping.  This can be the same Idle routine that handles meter updates
etc.

All the controls are rendered to a background pixel buffer parts of which are
then copied to the visible window in response to normal expose events (or forced
expose events if / when a control needs to be updated e.g. if we know its value
has changed).

There were some concerns raised in another post about re-parenting windows - you
can access any window through any display connection, they are not tied together
- the XWindow ID is allocated by the Xserver, it doesn't matter how you get to
it, so you can still reparent your plugin GUI into the host even though the host
is on a different display, just so long as you don't mix up the display
connections when you do the re-parenting otherwise the plugin GUI thread will
attempt to talk down the hosts connection to X and cause problems.

The other thing that made me try running the plugin GUI in its own thread rather
than the method in jorgens plugins was that (I think) the pointer to the Xevent
handler is passed to the host as an atom used as a window property on the
plugins window.  In the implementation I looked at, this was a 32Bit value which
limited the mechanism to only working on 32Bit hosts.  I wanted to try and come
up with a method that would work on either 32 or 64Bit architectures.

I hope this is of some help - I can provide more detail if necessary - use the
contact button on my website at http://www.linuxdsp.co.uk

*/
