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

// default/simple skin
// for testing/developing purposes mainly, but might be fleshed out
// to a proper skin sometime (and renamed... h_SkinSimple perhaps?

//#include "gui/h_Skin.h"
#include "src/h_Skin.h"

/*
  a_Mode:
  0 : default
  1 : active
  2 : hover on
  3 : hover off
  todo: fix these...
*/

class skin_Default : public h_Skin
{
  private:
    h_Color m_BackColor;
    h_Color m_PanelColor;
    h_Color m_WidgetColor;
    h_Color m_TextColor;
    h_Color m_ValueColor;
    h_Color m_ActiveTextColor;
    h_Color m_ActiveValueColor;

    h_Color m_BorderColor;
    h_Color m_HoverColor;

  public:

    skin_Default(h_Editor* a_Editor)
    //: h_Skin(&m_Settings)
      {
        m_BackColor         = H_RGB(120,120,120);
        m_PanelColor        = H_RGB(128,128,128);
        m_WidgetColor       = H_RGB(136,136,136);

        m_TextColor         = H_RGB( 96, 96, 96);
        m_ValueColor        = H_RGB(160,160,160);
        m_ActiveTextColor   = H_RGB(  0,  0,  0);
        m_ActiveValueColor  = H_RGB(176,176,176);

        m_BorderColor       = H_RGB(160,160,160);
        m_HoverColor        = H_RGB(192,192,192);
      }

    virtual void drawBackground(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        a_Painter->setFillColor(m_BackColor);
        a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h);
      }

    //----------

    virtual void drawPanel(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        switch (a_Mode)
        {
          case dm_Normal:
            a_Painter->setFillColor(m_PanelColor);
            a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          //case dm_Active:
          //  break;
        }
      }

    //----------

    virtual void drawLabel(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, h_String a_Text)
      {
        a_Painter->setFillColor(m_WidgetColor);
        a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
        a_Painter->setTextColor(m_TextColor);
        a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Text.ptr(), ta_Center);
      }

    //----------

    virtual void drawButton(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, h_String a_Text)
      {
        switch (a_Mode)
        {
          case dm_Normal:
            a_Painter->setFillColor(m_WidgetColor);
            a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            a_Painter->setTextColor(m_TextColor);
            a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Text.ptr(), ta_Center);
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case dm_Active:
            a_Painter->setFillColor(m_WidgetColor);
            a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            a_Painter->setTextColor(m_ActiveTextColor);
            a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Text.ptr(), ta_Center);
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case dm_Enter: // hover on
            a_Painter->setDrawColor(m_HoverColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case dm_Leave: // hover off
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
        } // switch
      }

    //----------

    virtual void drawValue(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value)
      {
        char buffer[256];
        switch (a_Mode)
        {
          case dm_Normal:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              h_Ftoa(buffer,a_Value);
              a_Painter->setTextColor(m_TextColor);
              a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), buffer, ta_Center);
              a_Painter->setDrawColor(m_BorderColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            }
            break;
          case dm_Active:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              h_Ftoa(buffer,a_Value);
              a_Painter->setTextColor(m_ActiveTextColor);
              a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), buffer, ta_Center);
              //a_Painter->setTextColor(m_TextColor);
              //a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2(), buffer, ta_Center);
              a_Painter->setDrawColor(m_HoverColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            }
            break;
          case dm_Enter:
            a_Painter->setDrawColor(m_HoverColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case dm_Leave:
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
        }
      }

    //----------

    virtual void drawSlider(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value)
      {
        switch (a_Mode)
        {
          case dm_Normal:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              int len = (int) (a_Value * (float)a_Rect.w);
              int x2 = a_Rect.x + len - 1;
              a_Painter->setFillColor(m_ValueColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, x2, a_Rect.y2());
              a_Painter->setDrawColor(m_BorderColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            }
            break;
          case dm_Active:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              int len = (int) (a_Value * (float)a_Rect.w);
              int x2 = a_Rect.x + len - 1;
              a_Painter->setFillColor(m_ActiveValueColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, x2, a_Rect.y2());
              a_Painter->setDrawColor(m_WidgetColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            }
            break;
          case dm_Enter:
            a_Painter->setDrawColor(m_HoverColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case dm_Leave:
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
        }
      }

    virtual void drawKnob(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value)
      {
        switch (a_Mode)
        {
          case dm_Normal:
          case dm_Active:
            {
              a_Rect.add(2,2,-4,-4);
              int x  = a_Rect.x;
              int y  = a_Rect.y;
              int size = h_Mini(a_Rect.w,a_Rect.h);
              a_Painter->setDrawColor(m_WidgetColor);
              a_Painter->setPenWidth(5);
              a_Painter->drawArc(x+2,y+2,x+(size-2),y+(size-2),-0.35,0.7);
              a_Painter->resetPen();
              a_Painter->setDrawColor(m_ValueColor);
              a_Painter->setPenWidth(5);
              a_Painter->drawArc(x+2,y+2,x+(size-2),y+(size-2),-0.35,a_Value*0.7);
              a_Painter->resetPen();
//              int th = a_Painter->textHeight("Xj");
//              if (a_Rect.h >= (2*th))
//              {
//                a_Painter->setTextColor(m_TextColor);
//                a_Painter->drawText(x+size+8,y,   a_Rect.x2(),a_Rect.y2(),a_Name,ta_Top|ta_Left);
//                a_Painter->drawText(x+size+8,y+th,a_Rect.x2(),a_Rect.y2(),a_Disp,ta_Top|ta_Left);
//              //a_Painter->drawText(x+size+5,y,   aRect.x2(),aRect.y2(),aDisp,ta_Bottom|ta_Left);
//              }
//              else
//              {
//                a_Painter->setTextColor(mTextColor);
//                a_Painter->drawText(x+size+5,y,aRect.x2(),aRect.y2(),aDisp,ta_Left);
//              }
            }
            break;
          //case dm_Active:
          //  {
          //  }
          //  break;
          case dm_Enter:
            break;
          case dm_Leave:
            break;
        }
      }

    //--------------------------------------------------

    virtual void drawValueExt(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value, h_String a_Label, h_String a_Disp)
      {
        switch (a_Mode)
        {
          case dm_Normal:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              a_Painter->setTextColor(m_TextColor);
              a_Painter->drawText( a_Rect.x+5, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Label.ptr(), ta_Left);
              a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2()-5, a_Rect.y2(), a_Disp.ptr(), ta_Right);
              a_Painter->setDrawColor(m_BorderColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              //a_Painter->setTextColor(m_ValueSkin.m_TextColor);
            }
            break;
          case dm_Active:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              a_Painter->setTextColor(m_ActiveTextColor);
              a_Painter->drawText( a_Rect.x+5, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Label.ptr(), ta_Left);
              a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2()-5, a_Rect.y2(), a_Disp.ptr(), ta_Right);
              a_Painter->setDrawColor(m_BorderColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            }
            break;
          case dm_Enter:
            a_Painter->setDrawColor(m_HoverColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case dm_Leave:
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
        }
      }

    //----------

    virtual void drawSliderExt(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value, h_String a_Label, h_String a_Disp)
      {
        switch (a_Mode)
        {
          case dm_Normal:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              int len = (int) (a_Value * (float)a_Rect.w);
              int x2 = a_Rect.x + len - 1;
              a_Painter->setFillColor(m_ValueColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, x2, a_Rect.y2());
              a_Painter->setTextColor(m_TextColor);
              a_Painter->drawText( a_Rect.x+5, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Label.ptr(), ta_Left);
              a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2()-5, a_Rect.y2(), a_Disp.ptr(), ta_Right);
              a_Painter->setDrawColor(m_BorderColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            }
            break;
          case dm_Active:
            {
              a_Painter->setFillColor(m_WidgetColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
              int len = (int) (a_Value * (float)a_Rect.w);
              int x2 = a_Rect.x + len - 1;
              a_Painter->setFillColor(m_ActiveValueColor);
              a_Painter->fillRect(a_Rect.x, a_Rect.y, x2, a_Rect.y2());
              a_Painter->setTextColor(m_ActiveTextColor);
              a_Painter->drawText( a_Rect.x+5, a_Rect.y, a_Rect.x2(), a_Rect.y2(), a_Label.ptr(), ta_Left);
              a_Painter->drawText( a_Rect.x, a_Rect.y, a_Rect.x2()-5, a_Rect.y2(), a_Disp.ptr(), ta_Right);
              a_Painter->setDrawColor(m_BorderColor);
              a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            }
            break;
          case dm_Enter:
            a_Painter->setDrawColor(m_HoverColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
          case dm_Leave:
            a_Painter->setDrawColor(m_BorderColor);
            a_Painter->drawRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
            break;
        }
      }

    //----------




};

//----------------------------------------------------------------------
#endif
