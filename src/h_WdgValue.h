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
#ifndef h_WdgValue_included
#define h_WdgValue_included
//----------------------------------------------------------------------

#include "src/h_Color.h"
#include "src/h_Widget.h"

#define H_MAX_DISPLAYSTRING 32

class h_WdgValue : public h_Widget
{
  private:
    float m_OrigValue;
    int   m_ClickedX;
    float m_ClickedY;
    bool  m_IsDragging;
  protected:
    char  m_DispBuf[H_MAX_DISPLAYSTRING];
    float m_Value;
    bool  m_DragVertical;
    float m_DragSensitivity;

  public:

    h_WdgValue(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, float a_Value=0)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        m_Value  = a_Value;
        m_OrigValue  = m_Value;
        m_ClickedX = 0;
        m_ClickedY = 0;
        m_IsDragging = false;
        m_DragVertical = true;
        m_DragSensitivity = 0.1;
      }

    virtual ~h_WdgValue()
      {
      }

    //----------

    virtual void setInternal(float a_Value)
      {
        m_Value=a_Value;
      }

    virtual float getInternal(void)
      {
        return m_Value;
      }

    //----------

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        if (m_Parameter)
        {
          h_String text = m_Parameter->getName();
          m_Parameter->getDisplay(m_DispBuf);
          float value = m_Parameter->getValue();
          m_Skin->drawValueExt(a_Painter,m_Rect,a_Mode,value,text,m_DispBuf);
        }
        else m_Skin->drawValue(a_Painter,m_Rect,a_Mode,m_Value);
      }

    virtual void do_Enter(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,dm_Enter); // draw border
        if (m_DragVertical) m_Listener->on_Cursor(cu_ArrowUpDown);
        else m_Listener->on_Cursor(cu_ArrowLeftRight);
      }

    virtual void do_Leave(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,dm_Leave); // remove border
        m_Listener->on_Cursor(H_DEF_CURSOR);
      }

    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        m_IsDragging = true;
        m_ClickedX = x;
        m_ClickedY = y;
        m_OrigValue = m_Value;
        m_Listener->on_Redraw(this,dm_Active);
        m_Listener->on_Redraw(this,dm_Enter);
      }

    virtual void do_MouseUp(int x, int y, int b, int s)
      {
        m_IsDragging = false;
        m_OrigValue = m_Value;
        //m_Listener->on_Change(this);
        bool inside = m_Rect.contains(x,y);
        m_Listener->on_Redraw(this,dm_Normal); // active
        if (inside) m_Listener->on_Redraw(this,dm_Enter);
      }

    // m_Rect.w/h and m_DragSensitivity must be > 0

    virtual void do_MouseMove(int x, int y, int s)
      {
        if (m_IsDragging)
        {
          float dist;
          if (m_DragVertical)
          { // |
            dist = m_ClickedY - y;
            dist *= 1.0f / (float)m_Rect.h * m_DragSensitivity;
          }
          else
          { // --
            dist = x - m_ClickedX;
            dist *= 1.0f / (float)m_Rect.w * m_DragSensitivity;
          }
          m_Value = m_OrigValue + dist;
          if (m_Value>1) m_Value=1;
          if (m_Value<0) m_Value=0;
          m_Listener->on_Change(this);
          m_Listener->on_Redraw(this,dm_Active); // active
          m_Listener->on_Redraw(this,dm_Enter); // border on
        }
      }

};

//----------------------------------------------------------------------
#endif

