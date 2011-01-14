///*
//  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al
//
//  This file is part of the Holos Library.
//  http://holos.googlecode.com
//
//  the Holos Library is free software: you can redistribute it and/or modify
//  it under the terms of the Holos Library License, either version 1.0
//  of the License, or (at your option) any later version.
//
//  the Holos Library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See LICENSE_HOLOS for more details.
//
//  You should have received a copy of the Holos Library License
//  If not, see <http://holos.googlecode.com/>.
//*/
////----------------------------------------------------------------------
//#ifndef h_Container_included
//#define h_Container_included
////----------------------------------------------------------------------
//
//#include "gui/h_Widget.h"
//
////----------------------------------------------------------------------
//
//class h_Container : public h_Widget
//{
//  protected:
//    h_Widget*         m_Parent;
//    h_Widgets         m_Children;
//
//  public:
//
//    h_Container(h_WidgetListener* a_Listener, h_Rect a_Rect)
//    : h_Widget(a_Listener,a_Rect)
//      {
//        m_Parent = H_NULL;
//        m_Children.clear();
//      }
//
//    //----------
//
//    virtual ~h_Container()
//      {
//        #ifndef H_NOAUTODELETE
//          deleteWidgets();
//        #endif
//      }
//
//    //----------------------------------------
//
//    virtual void appendWidget(h_Widget* a_Widget)
//      {
//        //int index = m_Children.size();
//        //a_Widget->m_Index  = index;
//        //a_Widget->setPainter(m_Painter);
//        m_Children.append(a_Widget);
//      }
//
//    //----------
//
//    virtual void deleteWidgets(void)
//      {
//        for (int i=0; i<m_Children.size(); i++) delete m_Children[i];
//      }
//
//    //----------------------------------------
//    //
//    //----------------------------------------
//
////    virtual void do_Timer(void) {}
////    virtual void do_SetPos(int x, int y) {}
////    virtual void do_SetSize(int w, int h) {}
////    virtual void do_Realign(void) {}
////    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect) {}
////    virtual void do_Enter(void) {}
////    virtual void do_Leave(void) {}
////    virtual void do_MouseDown(int x, int y, int b, int s) {}
////    virtual void do_MouseUp(int x, int y, int b, int s) {}
////    virtual void do_MouseMove(int x, int t, int s) {}
////    virtual void do_KeyDown(int k, int s) {}
////    virtual void do_KeyUp(int k, int s) {}
//
//    //----------------------------------------
//
//    virtual void do_Timer(void)
//      {
//        for (int i=0; i<m_Children.size(); i++)
//        {
//          h_Widget* widget = m_Children[i];
//          //if (widget->m_Flags & wf_Timer)
//          if (widget->hasFlag(wf_Timer))
//          {
//            widget->do_Timer();
//          }
//        }
//      }
//
//    //----------
//
//    virtual void do_SetPos(int x, int y)
//      {
//        m_Rect.x = x;
//        m_Rect.y = y;
//      }
//
//    //----------
//
//    virtual void do_SetSize(int w, int h)
//      {
//        m_Rect.w = w;
//        m_Rect.h = h;
//        // realign?
//      }
//
//    //----------
//
//    virtual void do_Realign(void)
//      {
//      }
//
//    //----------
//
//    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
//      {
//        //paint self
//        for (int i=0; i<m_Children.size(); i++)
//        {
//          do_Paint(a_Painter,a_Rect);
//        }
//      }
//
//    //----------
//
//    virtual void do_Enter(void)
//      {
//      }
//
//    //----------
//
//    virtual void do_Leave(void)
//      {
//      }
//
//    //----------
//
//    virtual void do_MouseDown(int x, int y, int b, int s)
//      {
//        for (int i=0; i<m_Children.size(); i++)
//        {
//          do_MouseDown(x,y,b,s);
//        }
//      }
//
//    //----------
//
//    virtual void do_MouseUp(int x, int y, int b, int s)
//      {
//        for (int i=0; i<m_Children.size(); i++)
//        {
//          do_MouseUp(x,y,b,s);
//        }
//      }
//
//    //----------
//
//    virtual void do_MouseMove(int x, int y, int s)
//      {
//        for (int i=0; i<m_Children.size(); i++)
//        {
//          do_MouseMove(x,y,s);
//        }
//      }
//
//    //----------
//
//    virtual void do_KeyDown(int k, int s)
//      {
//        for (int i=0; i<m_Children.size(); i++)
//        {
//          do_KeyDown(k,s);
//        }
//      }
//
//    //----------
//
//    virtual void do_KeyUp(int k, int s)
//      {
//        for (int i=0; i<m_Children.size(); i++)
//        {
//          do_KeyUp(k,s);
//        }
//      }
//
//    //----------------------------------------
//    //
//    //----------------------------------------
//
//};
//
////----------------------------------------------------------------------
//#endif
//
