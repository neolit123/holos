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
#ifndef h_Editor_cpp_included
#define h_Editor_cpp_included
#ifdef h_Editor_included
//----------------------------------------------------------------------

h_Editor::h_Editor(h_Instance* a_Instance, h_Rect a_Rect, void* a_Parent)
: h_Window(this,a_Rect,a_Parent)
  {
    m_Instance = a_Instance;
  }

//----------

h_Editor::~h_Editor()
  {
    #ifndef H_NOAUTODELETE
      deleteConnections();
    #endif
  }

//----------

void h_Editor::connect(h_Parameter* a_Parameter, h_Widget* a_Widget)
  {
    int conn = m_Connections.size();
    //trace("  connect: " << conn);
    a_Widget->setConnect(conn);
    a_Parameter->setConnect(conn);
    m_Connections.append( new h_Connection(a_Parameter,a_Widget) );
  }

//----------

void h_Editor::deleteConnections(void)
  {
    for (int i=0; i<m_Connections.size(); i++) delete m_Connections[i];
  }

//----------

// called by host [via instance]
// check if we need to update a widget

void h_Editor::notifyParameter_fromInstance(h_Parameter* a_Parameter)
  {
    int conn = a_Parameter->getConnect();
    if (conn>=0)
    {
      //trace("h_Editor.notifyParameter_fromInstance");
      //h_Widget* wdg = m_Connections[index]->m_Widget;
      // redraw widget..
    }
  }

//----------

// widget has changed/tweaked.. (from editor)
// if this is connected to a parameter, notify instance

void h_Editor::on_Change(h_Widget* a_Widget)
  {
    //trace("h_Editor.on_Change(wdg)");
    int conn = a_Widget->getConnect();
    if (conn>=0)
    {
      h_Parameter* par = m_Connections[conn]->m_Parameter;
      par->setInternal( a_Widget->getValue() );

      // in vst instance (impl)
      m_Instance->notifyParameter_fromEditor(par);
    }
    //if (m_Listener) m_Listener->on_Change(a_Widget);
    //h_Window::on_Change(a_Widget);
  }

//----------------------------------------------------------------------
#endif
#endif

