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
#ifndef h_Widget_included
#define h_Widget_included
//----------------------------------------------------------------------
/*

TODO: consider if we should split the widget into two classes:
      - a simple widget without children
      - a container with children

TODO: multiple listeners?


*/

#include "src/h_Rect.h"
#include "src/h_Painter.h"
#include "src/h_Skin.h"
#include "src/h_Parameter.h"

//----------

// widget flags
#define wf_None       0
#define wf_Active     ( 1 << 0 )
#define wf_Visible    ( 1 << 1 )
#define wf_Timer      ( 1 << 8 )
#define wf_Clipping   ( 1 << 9 )
#define wf_Align      ( 1 << 10 )
#define wf_Capture    ( 1 << 11 )
#define wf_Alpha      ( 1 << 12 )

// widget alignment
#define wa_None         0
#define wa_Client       1
#define wa_Left         2
#define wa_Right        3
#define wa_Top          4
#define wa_Bottom       5
#define wa_LeftTop      6
#define wa_RightTop     7
#define wa_LeftBottom   8
#define wa_RightBottom  9
#define wa_TopLeft      10
#define wa_TopRight     11
#define wa_BottomLeft   12
#define wa_BottomRight  13
//#define wa_Stacked      14
#define wa_StackedHoriz 15
#define wa_StackedVert  16

//----------

class h_Widget;

//#include "lib/h_Array.h"
#include "src/h_Array.h"
typedef h_Array<h_Widget*> h_Widgets;

//----------------------------------------------------------------------

// draw mode
#define dm_Normal 0
#define dm_Active 1
#define dm_Enter  2
#define dm_Leave  3

class h_WidgetBase
{
  public:
    virtual void do_Timer(void)                                               { trace("h_WidgetBase.do_Timer"); }
    virtual void do_SetPos(int x, int y)                                      { trace("h_WidgetBase.do_SetPos"); }
    virtual void do_SetSize(int w, int h)                                     { trace("h_WidgetBase.do_SetSize"); }
    virtual void do_Realign(void)                                             { trace("h_WidgetBase.do_Realign"); }
    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode=0)  { trace("h_WidgetBase.do_Paint"); }
    virtual void do_Enter(h_Widget* a_Widget)                                 { trace("h_WidgetBase.do_Enter"); }
    virtual void do_Leave(h_Widget* a_Widget)                                 { trace("h_WidgetBase.do_Leave"); }
    virtual void do_MouseDown(int x, int y, int b, int s)                     { trace("h_WidgetBase.do_MouseDown"); }
    virtual void do_MouseUp(int x, int y, int b, int s)                       { trace("h_WidgetBase.do_MouseUp"); }
    virtual void do_MouseMove(int x, int t, int s)                            { trace("h_WidgetBase.do_MouseMove"); }
    virtual void do_KeyDown(int k, int s)                                     { trace("h_WidgetBase.do_KeyDown"); }
    virtual void do_KeyUp(int k, int s)                                       { trace("h_WidgetBase.do_KeyUp"); }
};

//----------------------------------------------------------------------

class h_WidgetListener
{
  public:
    virtual void on_Change(h_Widget* a_Widget)                                        { trace("h_WidgetListener.on_Change"); }
    virtual void on_Redraw(h_Widget* a_Widget, int a_Mode)                            { trace("h_WidgetListener.on_Redraw"); }
    virtual void on_Cursor(int a_Cursor)                                              { trace("h_WidgetListener.on_Cursor"); }
    virtual void on_Hint(h_String a_Text)                                             { trace("h_WidgetListener.on_Hint"); }
    virtual void on_Size(h_Widget* a_Widget, int a_DeltaX, int a_DeltaY, int a_Mode)  { trace("h_WidgetListener.on_Size"); }
    virtual void on_Modal(bool a_Modal, h_Widget* a_Widget)                           { trace("h_WidgetListener.on_Modal"); }


};

//typedef h_Array<h_WidgetListener*> h_WidgetListeners;


//----------

//class h_WidgetOwner // aka window
//{
//  public:
//    virtual void on_Redraw(void) {}
//    virtual void on_Redraw(h_Rect a_Rect) {}
//    virtual void on_Redraw(h_Widget* a_Widget) {}
//    virtual void on_SetCursor(int a_Cursor) {}
//};

//----------------------------------------------------------------------

// TODO. sort (and documanet) member variables by feature/functionality,
// perhaps rename the variables depending on function..
/*
  visual:        rect,skin
  alignment:     alignment,min/max width/height,margin,padding,content,orig
  interactivity: captured,hover,modal widgets
  hierarchy:     listener, parent, children, index, connect,
  options:       flags
*/

class h_Widget : public h_WidgetBase,
                 public h_WidgetListener
{
  friend class h_Window;
  private:
  //protected:
    h_Rect    m_Content;               // rect encapsulating all sub-widgets (updated in doRealign)
    h_Rect    m_Orig;
    int       m_Alignment;
    int       m_MinWidth, m_MinHeight;
    int       m_MaxWidth, m_MaxHeight;
    int       m_MarginX,  m_MarginY;    // container inner space (between outer border & widgets)
    int       m_PaddingX, m_PaddingY;   // space between wal_Stacked widgets
    int       m_StackedX, m_StackedY;
    h_Widget* m_CapturedWidget;
    h_Widget* m_HoverWidget;
    h_Widget* m_ModalWidget;
  //h_Rect    m_Client;

  protected:
    h_WidgetListener* m_Listener;
    h_Widget*         m_Parent;
    int               m_Index;
    int               m_Connect;
    h_Parameter*      m_Parameter;
    int               m_Flags;
    h_Widgets         m_Children;
    h_Rect            m_Rect;
    h_Skin*           m_Skin;

  public:

    //inline h_Widget* getParent(void) { return m_Parent; }
    inline int        getFlags(void)                  { return m_Flags; }
    inline int        hasFlag(int a_Flag)             { return (m_Flags&a_Flag); }
    inline void       setFlag(int a_Flag)             { m_Flags|=a_Flag; }
    inline void       clearFlag(int a_Flag)           { m_Flags&=~a_Flag; }
    inline h_Rect     getRect(void)                   { return m_Rect; }
    inline h_Rect     getContent(void)                { return m_Content; }
    inline int        getNumChildren(void)            { return m_Children.size(); }
    inline h_Widget*  getChild(int a_Index)           { return m_Children[a_Index]; }
    inline void       setIndex(int a_Index)           { m_Index=a_Index; }
    inline int        getIndex(void)                  { return m_Index; }
    inline void       setConnect(int a_Connect)       { m_Connect=a_Connect; }
    inline int        getConnect(void)                { return m_Connect; }
    inline void       setCapture(h_Widget* a_Widget)  { m_CapturedWidget = a_Widget; }
    inline h_Widget*  getCapture(void)                { return m_CapturedWidget; }

    virtual void      setInternal(float a_Value)      { }
    virtual float     getInternal(void)               { return 0; }

    inline void       setParameter(h_Parameter* a_Parameter) { m_Parameter = a_Parameter; }

  public:

    h_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect, int a_Alignment=wa_None)
      {
        m_Listener        = a_Listener;
        m_Parent          = H_NULL;
        m_Index           = -1;
        m_Connect         = -1;
        m_Parameter       = H_NULL;
        m_Flags           = wf_Active|wf_Visible|wf_Capture|wf_Align;
        m_Rect            = a_Rect;
        m_Skin            = H_NULL;

        m_Content         = m_Rect;//H_NULL_RECT;
        m_Orig            = m_Rect;
        m_Alignment       = a_Alignment;
        m_MinWidth        = 0;
        m_MinHeight       = 0;
        m_MaxWidth        = 999999;
        m_MaxHeight       = 999999;
        m_MarginX         = 0;
        m_MarginY         = 0;
        m_PaddingX        = 0;
        m_PaddingY        = 0;
        m_StackedX        = 0;
        m_StackedY        = 0;

        m_CapturedWidget  = H_NULL; // which widget is receiving follow mouse move messages
        m_HoverWidget     = this;   // sub-widget mouse hovers over, auume ourselves
        m_ModalWidget     = H_NULL; // send events only to widget, if modal

        //m_Client          = m_Rect;
        //m_Value          = 0;
        //m_Connection     = -1;
        //m_Parameter      = NULL;
        //m_ModalIndex     = -1;
        //m_Id             = 0;
        //m_Ptr            = NULL;
      }

    //----------

    virtual ~h_Widget()
      {
        #ifndef H_NOAUTODELETE
          deleteWidgets();
        #endif
      }

    //----------------------------------------
    // children
    //----------------------------------------

    virtual
    void appendWidget(h_Widget* a_Widget)
      {
        int index = m_Children.size();
        a_Widget->m_Index = index;
        a_Widget->m_Parent = this;
        a_Widget->applySkin(m_Skin,true,true);
        m_Children.append(a_Widget);
      }

    //----------

    virtual
    void removeWidget(int a_Index)
      {
        m_Children.remove(a_Index);
      }

    //----------

    virtual
    void deleteWidgets(void)
      {
        for (int i=0; i<m_Children.size(); i++) delete m_Children[i];
        //m_Children.clear();
      }

    //----------------------------------------
    // layout
    //----------------------------------------

    inline
    bool intersects(h_Rect a_Rect)
      {
        return m_Rect.intersects(a_Rect);
      }

    inline
    bool contains(int a_Xpos, int a_Ypos)
      {
        return m_Rect.contains(a_Xpos,a_Ypos);
      }

    //----------

    virtual
    void setBorders(int a_MarginX, int a_MarginY, int a_PaddingX=0, int a_PaddingY=0)
      {
        m_MarginX  = a_MarginX;
        m_MarginY  = a_MarginY;
        m_PaddingX = a_PaddingX;
        m_PaddingY = a_PaddingY;
      }

    //----------

    virtual
    void setLimits(int a_MinW, int a_MinH, int a_MaxW=999999, int a_MaxH=999999)
      {
        m_MinWidth   = a_MinW;
        m_MinHeight  = a_MinH;
        m_MaxWidth   = a_MaxW;
        m_MaxHeight  = a_MaxH;
      }

    //----------

    virtual
    void applySkin(h_Skin* a_Skin, bool a_Sub=false, bool a_OnlyIfNull=false)
      {
        if (a_OnlyIfNull)
        {
          if (!m_Skin) m_Skin=a_Skin;
        }
        else m_Skin = a_Skin;
        if (a_Sub)
        {
          for (int i=0; i<m_Children.size(); i++)
            m_Children[i]->applySkin(a_Skin,a_Sub,a_OnlyIfNull);
        }
      }


    //----------

    //virtual void setPainter(h_Painter* a_Painter)
    //  {
    //    m_Painter = a_Painter;
    //    for (int i=0; i<m_Children.size(); i++) { setPainter(a_Painter); }
    //  }

    //----------

    // find first widget that contains x,y
    // or 'self' is no sub-widgets found or hit
    //
    // start searching from end of list, so that widgets that are painted last
    // (topmost) are found first.

    virtual
    h_Widget* findWidget(int a_Xpos, int a_Ypos)
      {
        h_Widget* widget = this;
        int num = m_Children.size();
        if (num>0)
        {
          for (int i=num-1; i>=0; i--)
          {
            h_Widget* w = m_Children[i];
            if (w->getFlags()&wf_Active)
            {
              if (w->contains(a_Xpos,a_Ypos))
              {
                widget = w->findWidget(a_Xpos,a_Ypos);
                if (widget!=w) return widget;
                else return w;
              } //contains
            } //active
          } //for num
        } //num>0
        return widget;
      }

    //----------------------------------------
    // widget base/handler
    //----------------------------------------

    virtual void do_Timer(void)
      {
        for (int i=0; i<m_Children.size(); i++)
        {
          h_Widget* widget = m_Children[i];
          if (widget->hasFlag(wf_Timer))
          {
            widget->do_Timer();
          }
        }
      }

    //----------

    virtual void do_SetPos(int a_Xpos, int a_Ypos)
      {
        int deltax = a_Xpos - m_Rect.x;
        int deltay = a_Ypos - m_Rect.y;
        for (int i=0; i<m_Children.size(); i++)
        {
          h_Widget* wdg = m_Children[i];
          int wx = wdg->getRect().x;
          int wy = wdg->getRect().y;
          wdg->do_SetPos( wx+deltax, wy+deltay );
        }
        m_Rect.setPos(a_Xpos,a_Ypos);
      }

    //----------

    virtual void do_SetSize(int a_Width, int a_Height/*, bool a_Realign=false*/)
      {
        if (a_Width  < m_MinWidth)  a_Width  = m_MinWidth;
        if (a_Width  > m_MaxWidth)  a_Width  = m_MaxWidth;
        if (a_Height < m_MinHeight) a_Height = m_MinHeight;
        if (a_Height > m_MaxHeight) a_Height = m_MaxHeight;
        m_Rect.setSize(a_Width,a_Height);
        //if (a_Realign) do_Realign();
      }

    //----------

    // move sub-widgets only

    virtual void do_Scroll(int dX, int dY)
      {
        for( int i=0;i<m_Children.size(); i++ )
        {
          h_Widget* wdg = m_Children[i];
          int wx = wdg->getRect().x;
          int wy = wdg->getRect().y;
          wdg->do_SetPos( wx + dX, wy + dY );
        }
      }

    //----------

    // realign sub-widgets according to their alignment setting.
    // also, it keeps track of a mContent rectangle, that encapsulates all the sub-widgets

    //TODO. update stackx,stacky when we update the available client area
    //      so we can interleave them...

    /*

    M = margin
    P = padding (between widgets when stacked)

     ________________________________________ _ _          _ ____
    |          ^                                                  |
    |          M                                                  |
    |      ____v___       ________       ____ _            _      |
    |     |        |     |        |     |                   |     |
    |<-M->|        |<-P->|        |<-P->|                   |<-M->|
    |     |________|     |________|     |______ _       _ __|     |
    |          ^                                                  :
    |          P
    |      ____v____      ___ _
    |     |         |    |
    |     :         .
    |

    TODO:
    - skip widget if not enough space for it?
      (null or negative space left)
    - break up this (too large) function into smaller pieces
      to make it easier to follow and see any overview...

    */

    virtual void do_Realign(void)
      {
        if (m_Flags&wf_Align)
        {
          h_Rect parent = m_Rect;
          parent.add( m_MarginX, m_MarginY, -(m_MarginX*2), -(m_MarginY*2) );
          h_Rect client = parent;
          m_Content.set( m_Rect.x, m_Rect.y,0,0);
          int stackx   = client.x;
          int stacky   = client.y;
          int largestw = 0;
          int largesth = 0;

          for( int i=0; i<m_Children.size(); i++ )
          {
            h_Widget* wdg = m_Children[i];
            int ww = wdg->getRect().w;  // current widget width
            int wh = wdg->getRect().h;  // height

            switch (wdg->m_Alignment)
            {

              //  _____
              // |  _  |
              // | |_| |
              // |_____|
              //

              case wa_None:

                wdg->do_SetPos(wdg->m_Orig.x+parent.x, wdg->m_Orig.y+parent.y);
                break;

              //   _____
              //  |     |
              //  |     |
              //  |_____|
              //

              case wa_Client:

                wdg->do_SetPos(  client.x, client.y );
                wdg->do_SetSize( client.w, client.h );
                break;

    //----------

              //  _____
              // |  |
              // |  |
              // |__|__
              //

              case wa_Left:

                wdg->do_SetPos( client.x, client.y );
                wdg->do_SetSize( ww, client.h );
                client.x += (ww+m_PaddingX);
                client.w -= (ww+m_PaddingX);
                stackx    = client.x;
                stacky    = client.y;
                largesth  = 0;

                break;

              //  ______
              //     |  |
              //     |  |
              //  __ |__|
              //

              case wa_Right:

                wdg->do_SetPos( client.x2()-ww+1, client.y );
                wdg->do_SetSize( ww, client.h );
                client.w -= (ww + m_PaddingX);
                break;

              //  ______
              // |______|
              // |      |
              //

              case wa_Top:

                wdg->do_SetPos( client.x, client.y );
                wdg->do_SetSize( client.w, wh );
                client.y += (wh+m_PaddingY);
                client.h -= (wh+m_PaddingY);
                stackx    = client.x;
                stacky    = client.y;
                largestw  = 0;
                break;

              //
              // |      |
              // |______|
              // |______|
              //

              case wa_Bottom:

                wdg->do_SetPos( client.x, client.y2()-wh+1 );
                wdg->do_SetSize( client.w, wh );
                client.h -= (wh+m_PaddingY);
                break;

    //----------

              //  __________
              // |    |
              // |____|
              // |    :
              //      .

              case wa_LeftTop:

                wdg->do_SetPos( client.x, client.y );
                client.x += (ww + m_PaddingX);
                client.w -= (ww + m_PaddingX);
                stackx = client.x;
                stacky = client.y;
                break;

              //  __________
              //      |     |
              //      |_____|
              //      :     |
              //      .

              case wa_RightTop:

                wdg->do_SetPos( client.x2()-ww+1, client.y );
                client.w -= (ww + m_PaddingX);
                break;

              //   __________
              //  |    .     |
              //  |____:     |
              //  |    |     |
              //  |____|_____|
              //

              case wa_LeftBottom:

                wdg->do_SetPos( client.x, client.y2()-wh+1 );
                client.x += (ww + m_PaddingX);
                client.w -= (ww + m_PaddingX);
                stackx = client.x;
                stacky = client.y;
                break;

              //  __________
              //      .     |
              //      :_____|
              //      |     |
              //  ____|_____|
              //

              case wa_RightBottom:

                wdg->do_SetPos( client.x2()-ww+1, client.y2()-wh+1 );
                //wdg->doSetSize( ww, client.h );
                client.w -= (ww + m_PaddingX);
                break;

    //----------

              //  __________
              // |    |
              // |____|.....
              // |
              //

              case wa_TopLeft:

                wdg->do_SetPos( client.x, client.y );
                client.y += (wh + m_PaddingY);
                client.h -= (wh + m_PaddingY);
                stackx = client.x;
                stacky = client.y;
                break;

              //  __________
              //        |   |
              //   .....|___|
              //            |
              //

              case wa_TopRight:

                wdg->do_SetPos( client.x2()-ww+1, client.y );
                //wdg->doSetSize( ww, client.h );
                client.y += (wh + m_PaddingY);
                client.h -= (wh + m_PaddingY);
                stackx = client.x;
                stacky = client.y;
                break;

              //   __________
              //  |          |
              //  |___ ...   |
              //  |   |      |
              //  |___| _____|
              //

              case wa_BottomLeft:

                wdg->do_SetPos( client.x, client.y2()-wh+1 );
                //client.y += (wh + mPaddingY);
                client.h -= (wh + m_PaddingY);
                break;

              //  __________
              //      .     |
              //      :_____|
              //      |     |
              //  ____|_____|
              //

              case wa_BottomRight:

                wdg->do_SetPos( client.x2()-ww+1, client.y2()-wh+1 );
                //wdg->doSetSize( ww, client.h );
                client.h -= (wh + m_PaddingY);
                break;

    //----------

              //  __________________
              // |    |    |   |
              // |____|____|___|...... .
              // |
              //

              case wa_StackedHoriz:

                {
                  int remain = client.x2() - stackx + 1;
                  if (remain >= ww)
                  {
                    // enough space
                    wdg->do_SetPos(stackx,stacky);
                    stackx += (ww+m_PaddingX);
                    if (wh>largesth) largesth = wh;
                    int prevy = client.y;
                    int curry = stacky + (largesth+m_PaddingY);
                    int diff  = curry-prevy;
                    client.y  = curry;
                    if (diff>0) client.h -= diff;
                  }
                  else
                  {
                    // not enougb space
                    stackx    = client.x;
                    stacky   += (largesth+m_PaddingY);
                    largesth  = wh;
                    wdg->do_SetPos(stackx,stacky);
                    client.y += (largesth+m_PaddingY);
                    client.h -= (largesth+m_PaddingY);
                    stackx += (ww+m_PaddingX);
                  }
                }
                break;

              //  ________
              // |___|
              // |___|
              // |___|
              // |   |
              // |   :
              //     .

              case wa_StackedVert:

                {
                  int remain = client.y2() - stacky + 1;
                  if (remain >= wh)
                  {
                    // enough space
                    wdg->do_SetPos(stackx,stacky);
                    stacky += (wh+m_PaddingY);
                    if (ww>largestw) largestw = ww;
                    int prevx = client.x;
                    int currx = stackx + (largestw+m_PaddingX);
                    int diff  = currx-prevx;
                    client.x  = currx;
                    if (diff>0) client.w -= diff;
                  }
                  else
                  {
                    // not enougb space
                    stackx   += (largestw+m_PaddingX);
                    stacky    = client.y;
                    largestw  = ww;
                    wdg->do_SetPos(stackx,stacky);
                    client.x += (largestw+m_PaddingX);
                    client.w -= (largestw+m_PaddingX);
                    stacky   += (wh+m_PaddingY);
                  }
                }
                break;

              //
              //
              //
              //
              //

            } // switch alignment
            m_Content.combine( wdg->getRect() ); // keep track of outer boundary
            wdg->do_Realign();
          } // for all widgets
          //mContent.add(-mMarginX,-mMarginY,mMarginX*2,mMarginY*2);
        } // if align
      }

    //----------

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        if (m_Flags&wf_Visible) // self
        {
          if (m_Rect.intersects(a_Rect)) // self
          {
            if (m_Flags&wf_Clipping) // self
            {
              a_Painter->setClipRect(m_Rect.x,m_Rect.y,m_Rect.x2(),m_Rect.y2());
            }
            for (int i=0; i<m_Children.size(); i++)
            {
              h_Widget* wdg = m_Children[i];
              if (wdg->m_Flags&wf_Visible)
              {
                if (/*wdg->intersects(aRect) &&*/ wdg->intersects(m_Rect)) // ??? a_Rect ?
                  wdg->do_Paint(a_Painter,a_Rect,a_Mode);
              }
            } //for
            if (m_Flags&wf_Clipping) a_Painter->clearClipRect(); // resetClipRect();
          } //intersect
        } //visible
      }

    //----------

    virtual void do_Enter(h_Widget* a_Widget)
      {
      }

    //----------

    virtual void do_Leave(h_Widget* a_Widget)
      {
      }

    //----------

    virtual void do_MouseDown(int a_Xpos, int a_Ypos, int a_Button, int a_State)
      {
        //if (mModalWidget) mModalWidget->doMouseDown(aXpos,aYpos,aButton);
        //else
        if (m_CapturedWidget) m_CapturedWidget->do_MouseDown(a_Xpos,a_Ypos,a_Button,a_State);
        else
        {
          if (m_Flags&wf_Active)
          {
            h_Widget* hover = findWidget(a_Xpos,a_Ypos);
            if (hover!=this)
            {
              if (m_Flags&wf_Capture) m_CapturedWidget = hover;
              hover->do_MouseDown(a_Xpos,a_Ypos,a_Button,a_State);
            } // !hover
          } // active
        } // !capture
      }

    //----------

    virtual void do_MouseUp(int a_Xpos, int a_Ypos, int a_Button, int a_State)
      {
        //if (mModalWidget) mModalWidget->doMouseUp(aXpos,aYpos,aButton);
        //else
        if (m_CapturedWidget)
        {
          m_CapturedWidget->do_MouseUp(a_Xpos,a_Ypos,a_Button,a_State);
          m_CapturedWidget = NULL;
        } //capture
        else
        {
          if (m_Flags&wf_Active)
          {
            h_Widget* hover = findWidget(a_Xpos,a_Ypos);
            if (hover!=this) hover->do_MouseUp(a_Xpos,a_Ypos,a_Button,a_State);
          } //active
        }
      }

    //----------

    virtual void do_MouseMove(int a_Xpos, int a_Ypos, int a_State)
      {
        //if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
        //else
        if (m_CapturedWidget) m_CapturedWidget->do_MouseMove(a_Xpos,a_Ypos,a_State);
        else
        {
          h_Widget* hover = findWidget(a_Xpos,a_Ypos);
          if (hover!=m_HoverWidget)
          {
            m_HoverWidget->do_Leave(m_CapturedWidget);
            m_HoverWidget = hover;
            m_HoverWidget->do_Enter(m_CapturedWidget);
          }
          if (hover!=this) hover->do_MouseMove(a_Xpos,a_Ypos,a_State);
        }
      }

    //----------

    virtual void do_KeyDown(int a_Key, int a_State)
      {
        //if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
        //else
        if (m_CapturedWidget) m_CapturedWidget->do_KeyDown(a_Key,a_State);

      }

    //----------

    virtual void do_KeyUp(int a_Key, int a_State)
      {
        //if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
        //else
        if (m_CapturedWidget) m_CapturedWidget->do_KeyUp(a_Key,a_State);
      }

    //----------------------------------------
    // widget listener
    //----------------------------------------

    virtual void on_Change(h_Widget* a_Widget)
      {
        if (m_Listener) m_Listener->on_Change(a_Widget);
      }

    //----------

    virtual void on_Redraw(h_Widget* a_Widget, int a_Mode)
      {
        if (m_Listener) m_Listener->on_Redraw(a_Widget,a_Mode);
      }

    //----------

    virtual void on_Cursor(int a_Cursor/*=DEF_PENWIDTH*/)
      {
        if (m_Listener) m_Listener->on_Cursor(a_Cursor);
      }

    //----------

    virtual void on_Hint(h_String a_Text)
      {
        if (m_Listener) m_Listener->on_Hint(a_Text);
      }

    //----------

    // called from wdgSizer
    virtual void on_Size(h_Widget* a_Widget, int a_DeltaX, int a_DeltaY, int a_Mode)
      {
        int w = m_Rect.w + a_DeltaX;
        int h = m_Rect.h + a_DeltaY;
        h_Widget::do_SetSize(w,h);
        if (m_Listener) m_Listener->on_Size(a_Widget,a_DeltaX,a_DeltaY,a_Mode);
      }

    //----------

    virtual void on_Modal(bool a_Modal, h_Widget* a_Widget)
      {
        if (m_Listener) m_Listener->on_Modal(a_Modal,a_Widget);
      }


};

//----------------------------------------------------------------------
#endif
