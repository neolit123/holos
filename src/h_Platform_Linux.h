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
#ifndef h_Platform_Linux_included
#define h_Platform_Linux_included
//----------------------------------------------------------------------

#include "src/h_String.h"

#ifndef H_NOGUI
  #include <X11/Xlib.h>
#endif

//----------------------------------------------------------------------

class h_Platform
{
  public:
    h_Platform();
    ~h_Platform();
    h_String getName(void);
    //#ifndef H_NOGUI
    //Display* openDisplay(void);
    //void     closeDisplay(Display* a_Display);
    //#endif
};

//Display* openDisplay(void) { return XOpenDisplay(NULL); }
//void closeDisplay(Display* disp) { XCloseDisplay(disp); }

//----------------------------------------------------------------------
#endif


/*

http://www.equalizergraphics.com/documentation/parallelOpenGLFAQ.html

X11 is also not thread safe by default. You either have to call XInitThreads
during initialization, or to use one Display connection per thread. In the
second case you can't use the glXContext from one Display connection with
another connection.

*/

