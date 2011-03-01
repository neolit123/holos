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
#ifndef h_Core_included
#define h_Core_included
//----------------------------------------------------------------------

class h_Platform;
class h_Format;

//----------

class h_Core
{
  private:
    bool  m_Initialized;
  public:
    h_Platform* m_Platform;
    h_Format*   m_Format;
  public:
    h_Core();
    ~h_Core();
    void initialize(void);
};

//----------

/*
  since this is static, it will be shared among all instances of our plugin,
  and the constructor will be called just after the dll/exe is loaded, during
  static variable initialization, before any other functions is called
  (like main,DllMain,..)
  and the desctructor is called when it's unloading from memory.. after
  everything else.
  and it's available anywhere, and gives access to the platfomr and format
  specific classes for the rest of the library code..
*/

static h_Core static_Core;

//----------------------------------------------------------------------
#endif

