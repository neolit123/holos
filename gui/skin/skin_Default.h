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

class skin_Default : public h_Skin
{
  private:
    h_SkinSettings m_Settings;

  public:
    skin_Default()
    //: h_Skin(&m_Settings)
      {
        m_Settings.m_BackColor = H_RGB(128,128,128);
        m_Settings.m_FillColor = H_RGB(112,112,112);
        m_Settings.m_DrawColor = H_RGB(144,144,144);
        m_Settings.m_TextColor = H_RGB(160,160,160);
      }

    virtual void drawBackground(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        a_Painter->setFillColor(m_Settings.m_BackColor);
        a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h);
      }

    virtual void drawButton(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        switch (a_Mode)
        {
          case 0: // off
          case 1: // off-hover
          case 2: // off-down
          case 3: // on
          case 4: // on-hover
          case 5: // on-down
            a_Painter->setFillColor(m_Settings.m_FillColor);
            a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h);
        } // switch
      }

};

//----------------------------------------------------------------------
#endif
