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
#include "build/build_all.h"

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:
    my_Descriptor(h_Host* a_Host)
    //: h_Descriptor(a_Host)
      {
        m_Name   = "holos";
        m_Author = "ccernn";
        m_Name   = "test plugin";
        m_Flags |= df_HasEditor;
        m_EditorRect = h_Rect(256,256);
      }
    //virtual ~my_Descriptor()
    //  {
    //  }
};

//------------------------------

class my_Instance : public h_Instance
{
  public:
    my_Instance(h_Descriptor* a_Descriptor)
    : h_Instance(a_Descriptor)
      {
        trace("hello world!");

//        h_Pattern pattern;
//        pattern.setPattern("*.png");
//        bool res = pattern.match("test1.png");
//        trace( "match: " << (res?"true":"false") );

      }
    virtual ~my_Instance()
      {
      }
};

//------------------------------

class my_Editor : public h_Editor
{
  private:
  public:
    my_Editor(h_Instance* a_Instance)
    : h_Editor(a_Instance)
      {
        //h_Bitmap* bmp1 = new h_Bitmap(64,64,24,H_NULL);
        //h_Bitmap* bmp2 = new h_Bitmap(64,64,24);
        //delete bmp2;
        //delete bmp1;
        h_Color black = H_RGBA(0xde,0xad,0xbe,0xef);
        trace(hex << black << dec);
//        h_Painter* painter = getWindow()->getPainter();
//        trace("painter: " << painter);
//        painter->setPenColor(H_RGB(255,0,0));
//        painter->drawLine(10,10,100,100);
      }
    virtual ~my_Editor()
      {
      }
    virtual void open(void* a_Parent)
      {
        trace("open");
        h_Editor::open(a_Parent);
      }

};

//----------------------------------------------------------------------

H_PLUGIN(my_Descriptor,my_Instance,my_Editor)
#include H_MAIN

