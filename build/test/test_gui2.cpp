
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

//#define H_DEBUG
//#undef H_DEBUG

//#define H_DEBUG_LOG "holos_win.log"
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE
//#define H_DEBUG_MEM
//#define H_DEBUG_NEW

#include "holos.h"

#include "gui/wdg/wdg_Background.h"

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:
    my_Descriptor(h_Host* a_Host)
      {
        m_Name   = "test_gui2";
        m_Author = "ccernn";
        m_Name   = "holos test plugin";
        m_Flags |= df_HasEditor;
        m_EditorRect = h_Rect(256,256);
      }
};

//------------------------------

//class my_Editor;
class my_Editor : public h_Editor//,
                  //public h_WidgetListener
{
  private:
    wdg_Background* back;

  public:
    my_Editor(h_Instance* a_Instance)
    : h_Editor(a_Instance)
      {
        //back = new wdg_Background(this,H_DARK_RED);
      }
    virtual ~my_Editor()
      {
        //delete back;
      }
    virtual void do_Open(h_Window* a_Window)
      {
        //a_Window->setRoot(back);
        //a_Window->show();
      }
    // widget listener
    virtual void on_Change(h_Widget* a_Widget)
      {
      }
    virtual void on_Hint(h_String a_Text)
      {
        trace(a_Text);
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
      }

};

//------------------------------

class my_Instance : public h_Instance
{
  private:
    h_Editor* m_Editor;
  public:
    my_Instance(h_Descriptor* a_Descriptor)
    : h_Instance(a_Descriptor)
      {
      }
    virtual ~my_Instance()
      {
      }
    virtual void* do_OpenEditor(void* a_Parent)
      {
        m_Editor = new my_Editor(this);
        m_Editor->open(a_Parent);
        return (void*)m_Editor;
      }
    virtual void do_CloseEditor(void)
      {
        m_Editor->close();
        delete m_Editor;
        m_Editor = H_NULL;
      }

};

//----------------------------------------------------------------------

H_PLUGIN(my_Descriptor,my_Instance,my_Editor)
#include H_MAIN

