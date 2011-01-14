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
#ifndef h_Descriptor_Exe_Included
#define h_Descriptor_Exe_Included
//----------------------------------------------------------------------

class h_Descriptor_Exe
{
  friend class h_Instance_Exe;
  private:
    int m_Result;
  protected:
    const char* m_Name;
    const char* m_Author;
    const char* m_Product;
    int         m_Flags;
    h_Rect      m_EditorRect;
  public:
    void* entrypoint(void) { m_Result=0; return &m_Result; }
};

typedef h_Descriptor_Exe h_Descriptor;

//----------------------------------------------------------------------
#endif
