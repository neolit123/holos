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
#ifndef h_Skin_included
#define h_Skin_included
//----------------------------------------------------------------------

//#include "core/h_Parameter.h"
#include "src/h_Window.h"

//----------

/*
  note that a skin doesn't need to overload all methods, we could for example
  have a skin with just a knob drawing function... if we attach this skin
  only to a knob widget, all is fine...
  or a skin inheriting from another one, but override just some methods,
  or the default colors, etc..
  perhaps read from .ini file
  draw bitmaps
  opengl
  nice fif we need different looking widgets for different parts of a
  plugin editor... (sliders, knobs, buttons...)

*/

class h_Skin
{

  public:

    //h_Skin() {}
    //virtual ~h_Skin() {}

    // the basic primitives used by the widgets

    virtual void drawBackground(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode) {}
    virtual void drawPanel(     h_Painter* a_Painter, h_Rect a_Rect, int a_Mode) {}
    virtual void drawLabel(     h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, h_String a_Text) {}
    virtual void drawButton(    h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, h_String a_Text) {}
    virtual void drawValue(     h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value) {}
    virtual void drawSlider(    h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value) {}
    virtual void drawKnob(      h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value) {}

    // connected to parameters:

    virtual void drawValueExt(  h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value, h_String a_Label, h_String a_Disp) {}
    virtual void drawSliderExt( h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value, h_String a_Label, h_String a_Disp) {}
    virtual void drawKnobExt(   h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value, h_String a_Label, h_String a_Disp) {}

};

//----------------------------------------------------------------------
#endif


