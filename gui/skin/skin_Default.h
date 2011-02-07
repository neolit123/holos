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
#ifndef skin_Default_included
#define skin_Default_included
//----------------------------------------------------------------------

#include "gui/h_Skin.h"

// F fill
// B border
// T text

struct h_SkinData
{
  h_Color m_FillColor;
  h_Color m_BorderColor;
  h_Color m_TextColor;
  h_Color m_ActiveColor;
};

//----------

class skin_Default : public h_Skin
{
  private:
    h_SkinData  m_BackSkin;
    h_SkinData  m_PanelSkin;
    h_SkinData  m_ButtonSkin;

  public:
    skin_Default()
    //: h_Skin(&m_Settings)
      {
        m_BackSkin.m_FillColor      = H_RGB(128,128,128);

        m_PanelSkin.m_FillColor     = H_RGB(128,128,128);
        m_PanelSkin.m_BorderColor   = H_RGB( 96, 96, 96);

        m_ButtonSkin.m_FillColor    = H_RGB(112,112,112);
        m_ButtonSkin.m_BorderColor  = H_RGB( 96, 96, 96);
        m_ButtonSkin.m_TextColor    = H_RGB(192,192,192);
        m_ButtonSkin.m_ActiveColor  = H_RGB( 96, 96, 96);

      }

    //--------------------------------------------------

    virtual void drawBackground(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        a_Painter->setFillColor(m_BackSkin.m_FillColor);
        a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h);
      }

    //----------

    virtual void drawPanel(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        switch (a_Mode)
        {
          case 0: // up
            a_Painter->setFillColor(m_PanelSkin.m_FillColor);
            a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            a_Painter->setDrawColor(m_PanelSkin.m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case 1: // down
            break;
        }
      }

    //----------

    virtual void drawButton(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, h_String a_Text)
      {
        switch (a_Mode)
        {
          case 0: // normal
            a_Painter->setFillColor(m_ButtonSkin.m_FillColor);
            a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            a_Painter->setTextColor(m_ButtonSkin.m_TextColor);
            a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Text.ptr(), ta_Center);
            break;
          case 1: // click
            a_Painter->setFillColor(m_ButtonSkin.m_ActiveColor);
            a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            a_Painter->setTextColor(m_ButtonSkin.m_TextColor);
            a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Text.ptr(), ta_Center);
            break;
          case 2: // hover on
            a_Painter->setDrawColor(m_ButtonSkin.m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case 3: // hover off
            a_Painter->setDrawColor(m_ButtonSkin.m_FillColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
        } // switch
      }

};

//----------------------------------------------------------------------
#endif
