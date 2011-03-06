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
#ifndef h_Window_included
#define h_Window_included
//----------------------------------------------------------------------

#ifdef H_WIN32
  #include "src/h_Window_Win32_impl.h"
#endif

#ifdef H_LINUX
  #include "src/h_Window_Linux_impl.h"
#endif

//----------------------------------------------------------------------
// buffer, cursor

class h_Window : public h_Window_Impl
{

  public:

    h_Window(h_WidgetListener* a_Listener, h_Rect a_Rect, void* a_Parent)
    : h_Window_Impl(a_Listener,a_Rect,a_Parent)
      {
      }

    virtual ~h_Window()
      {
      }

    //----------------------------------------
    // do...
    //----------------------------------------

    virtual void do_SetSize(int a_Width, int a_Height)
      {
        //trace("setsize: " << a_Width << "," << a_Height);
        m_Rect.setSize(a_Width,a_Height);
        if (m_Flags&wf_Align) do_Realign();
        paintBuffer(m_Rect);
        //blitBuffer(m_Rect);
      }

    //----------------------------------------
    // on...
    //----------------------------------------

    // we're tweaking a widget, so redraw it directly
    // and if buffered, blit to screen

    // also called from h_Editor.notifyParameter

    virtual void on_Redraw(h_Widget* a_Widget, int a_Mode)
      {
        //h_Rect rect = a_Widget->getRect();
        //trace("redraw: " << rect.x << "," << rect.y << ", " << rect.w << "," << rect.h );
        h_Painter* painter = getSurface()->getPainter();
        h_Widget* wdg = a_Widget;
        //while ( (wdg->getFlags()&wf_Alpha) && (wdg->m_Parent) ) wdg = wdg->m_Parent;
        if (wdg->getFlags()&wf_Alpha)
        {
          if (wdg->m_Parent) wdg = wdg->m_Parent;
          on_Redraw(wdg,a_Mode);
        }
        else
        {
          h_Rect rect = a_Widget->getRect();
          wdg->do_Paint(painter,rect,a_Mode);
          //beginPaint();
          //trace("blitbuffer: " << rect.x << "," << rect.y << ", " << rect.w << "," << rect.h);
          blitBuffer( rect );
          //endPaint();
        }
      }

    // a widget want to change the mouse cursor
    virtual void on_Cursor(int a_Cursor)
      {
        //if (a_Cursor==-2)      hideCursor();
        //else if (a_Cursor==-3) showCursor();
        //else
          setCursor(a_Cursor);
      }

    //virtual void on_Hint(h_String aHint)
    //  {
    //  }

    //virtual void on_Size(h_Widget* a_Widget, int a_DeltaX, int a_DeltaY, int a_Mode)
    //  {
    //    if (m_Flags&wf_Align) do_Realign();
    //    //if (mFlags&wf_Visible)
    //      redrawAll();
    //  }

    //virtual void on_Modal(bool a_Modal, h_Widget* a_Widget)
    //  {
    //    if (a_Modal) goModal(a_Widget);
    //    else unModal();
    //  }

};

//----------------------------------------------------------------------
#endif

/*

double buffering:
- the window keeps a h_Surface as a background buffer. we draw directly into
  this whenever we need to, and either blit this part directly to the screen,
  or invalidate the area, so we get a paint message later..

resizing the window means that we have to resize the h_Surface too, and doing
this per pixel could be slow (?),
so we might keep a larger surface than we really need, and just use the part
we need to fill the window/editor size..
(a bit like the h_Array, but for two dimensions)

*/
