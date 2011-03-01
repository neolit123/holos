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
#ifndef h_WdgKnob_included
#define h_WdgKnob_included
//----------------------------------------------------------------------

//#include "src/h_Color.h"
//#include "src/h_Widget.h"
#include "src/h_WdgValue.h"

class h_WdgKnob : public h_WdgValue
{

  public:

    h_WdgKnob(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, float a_Value=0)
    : h_WdgValue(a_Listener,a_Rect,a_Align,a_Value)
      {
        //m_DragVertical = false;
        //m_DragSensitivity = 1;
      }

    virtual ~h_WdgKnob()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        if (m_Parameter)
        {
          h_String text = m_Parameter->getName();
          m_Parameter->getDisplay(m_DispBuf);
          float value = m_Parameter->getInternal();
          m_Skin->drawKnobExt(a_Painter,m_Rect,a_Mode,value,text,m_DispBuf); //TODO
        }
        else m_Skin->drawKnob(a_Painter,m_Rect,a_Mode,m_Value);
      }

};

//----------------------------------------------------------------------
#endif
