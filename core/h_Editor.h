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
#ifndef h_Editor_included
#define h_Editor_included
//----------------------------------------------------------------------

#include "gui/h_Window.h"

struct h_Connection
{
  h_Parameter*  m_Parameter;
  h_Widget*     m_Widget;
  h_Connection(h_Parameter* a_Parameter, h_Widget* a_Widget)
    {
      m_Parameter = a_Parameter;
      m_Widget    = a_Widget;
    }
};

typedef h_Array<h_Connection*> h_Connections;

//----------------------------------------------------------------------

class h_Editor : public h_Window
{
  private:
    h_Instance*   m_Instance;
    h_Connections m_Connections;

  public:

    h_Editor(h_Instance* a_Instance, h_Rect a_Rect, void* a_Parent);
    virtual ~h_Editor();
    virtual void connect(h_Parameter* a_Parameter, h_Widget* a_Widget);
    virtual void deleteConnections(void);
    virtual void notifyParameter_fromInstance(h_Parameter* a_Parameter);
    virtual void on_Change(h_Widget* a_Widget);

};

//----------------------------------------------------------------------
#endif
